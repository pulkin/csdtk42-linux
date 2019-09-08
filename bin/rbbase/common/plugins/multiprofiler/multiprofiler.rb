
cwRegisterPlugin(File.dirname(__FILE__)+"/multiprofiler.dll")

class Array
    def each_pair()
           raise "each_pair requires an even sized array" if(self.size % 2 !=0)
           0.step(self.size-1,2) { |i| yield(self[i],self[i+1]) }
    end
end

module MultiProfilerModule
# Set PRF file to latest recorded.
lastMultiPrfForRecord = cwGetProfileEntry("lastMultiPrfForRecord","")
mpSetPrfNameLineEdit(lastMultiPrfForRecord)

def profilerMallocRamBuffer(size=ProfilerControl::DEFAULT_BUFFER_SIZE, connection=$CURRENTCONNECTION)
    raise "Pure Virtual Method Not Overloaded."
end

def profilerFreeRamBuffer(connection=$CURRENTCONNECTION)
    raise "Pure Virtual Method Not Overloaded."
end

def profilerEnableProfiling(enable, mode=ProfilerControl::PROFILE_IN_RAM, connection=$CURRENTCONNECTION)
    raise "Pure Virtual Method Not Overloaded."
end

def profilerDumpRamBuffer(connection=$CURRENTCONNECTION)
    raise "Pure Virtual Method Not Overloaded."
end

def profilerDisplayControl()
    raise "Pure Virtual Method Not Overloaded."
end

def profilerGetStatus(connection=$CURRENTCONNECTION)
    raise "Pure Virtual Method Not Overloaded."
end

class MultiProfilerException < Exception
end

class MultiProfilerExceptionBufferAlreadyAllocated < MultiProfilerException
end

class MultiProfilerExceptionBufferAlreadyFreed < MultiProfilerException
end

class MultiProfilerExceptionSystemCrashed < MultiProfilerException
end

begin
    require "profiler_control.rb"
rescue LoadError
    puts "**** Warning, this chip does not have a profiler_control.rb script, the MultiProfiler will not work."
end

@@MultiprofilerThread = nil;

def stopMultiprofiling(filename, mode=ProfilerControl::PROFILE_IN_RAM)
    puts "html> Stop profiling ..."
    # Stop profiling
    profilerEnableProfiling(false, mode)
    
    # Kill the thread if any (end profile on Trace)
    @@MultiprofilerThread.kill if (@@MultiprofilerThread)

    # Dump file if record in a RAM buffer
    if (mode == ProfilerControl::PROFILE_IN_RAM)
        data = profilerDumpRamBuffer(mpForceHostDump?())
        dumpMultiprofiling(filename, data)

        # Free Buffer
        profilerFreeRamBuffer()
    end
end

# Do not much for RAM.
# Do much for profiling.
def startMultiprofiling(filename, bufferSize=ProfilerControl::DEFAULT_BUFFER_SIZE, mode=ProfilerControl::PROFILE_IN_RAM)

    puts "html> Start Profiling ..."
    case mode
    when ProfilerControl::PROFILE_IN_RAM
        # Allocate Buffer
        profilerMallocRamBuffer(bufferSize)

        # Start profiling
        profilerEnableProfiling(true, ProfilerControl::PROFILE_IN_RAM)
        
        # We dump the whole buffer in the end, exit here
        return

    when  ProfilerControl:: PROFILE_ON_TRACE
        # Start profiling
        profilerEnableProfiling(true, ProfilerControl:: PROFILE_ON_TRACE)

        # Start a thread to dump in real-time in the file.
        @@MultiprofilerThread = Thread.new {
        myfile=File.open(filename, "wb")

        myfile << "DBGP"
        myfile << [0,1,0,1].pack("c*")

        firstTs = nil
        lastpfTsWrap = 0

        mybypass = CHBPConnection.new($CURRENTCONNECTION, [0x94])
        mybypass.open(false)
        begin

            loop {
                begin
                    a = mybypass.getPacket(0)
                rescue CHBPConnectionTimeout
                    sleep 0.01 
                    #Sleep when there ain't no packets  
                    redo
                end
                CRVERBOSE(a.inspect, 2)
                data = a[1]
                ts = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0]
                # tag = (data[5] << 8) | data[4]
                # puts "@%12d: %04x" % [ts,tag]
                if ( firstTs.nil? ) then
                    firstTs = ts;
                end
                relTs=ts-firstTs;
                # trace timestamp is based on Greenstone uptime @16384Hz on 32bits
                # profiling format is defined with 1Mhz timespamp on 13 bits
                pfTs=relTs*1000000/16384

                while ((pfTs - lastpfTsWrap) > (1<<13)) do
                    myfile << [0xff,0xff,0xff,0x7f].pack("c*")
                    lastpfTsWrap += (1<<13)
                end

                pfts = (pfTs - lastpfTsWrap)
                data[6] = pfts & 0xff
                data[7] = (pfts >> 8) & 0xff

                myfile << data[4..7].pack("c*")

            }
        rescue Exception => e
            errputs "Exception happened in trace profiling thread."
            CRExceptionPrint e
        ensure
            myfile << [0xff,0xff,0xfe,0x7f].pack("c*")
            myfile.close

            mybypass.close
            CRVERBOSE("Thread's dead",2)
        end
    }
    end # case mode
end

# Dump a buffer filled alternatively of timestamps then PXTS tags
# (Each code on 32 bits, though PXTS tags only fill half of their
# 32 bit word)
def dumpMultiprofiling(filename, data)
    dataToFile=[]

    myfile=File.open(filename, "wb")

    myfile << "DBGP"
    myfile << [0,1,0,1].pack("c*")

    firstTs = nil
    lastpfTsWrap = 0

    status = profilerGetStatus()
    # Indexes to browse through the file
    # startIndex is the beginning of the buffer
    # endIndex is the beginning of the buffer
    # wrapIndex is the index where time is wrapping, ie
    # after this happened before what is before the wrap index
    # ...
    startIndex  = 0
    wrapIndex   = status[:wrapIndex]
    endIndex    = data.size() - 1

    if (status[:overflowed])
        puts"html>WARNING !! OVERFLOW DETECTED !!"
    end

    # ... So do we put data in the file from wrap to end, and then 
    # from start to wrap, is there is a wrap, ore in just one step
    # otherwise.
    if (status[:wrapped] && startIndex != wrapIndex)
        steps = [data[wrapIndex .. endIndex], data[startIndex .. (wrapIndex-1)]]
    else
        steps = [data[startIndex .. endIndex]]
    end
    
    begin
        steps.each { |step|
            step.each_pair {|ts, tag|
                if ( firstTs.nil? ) then
                    firstTs = ts;
                end
                relTs=ts-firstTs;
                # trace timestamp is based on Greenstone uptime @16384Hz on 32bits
                # profiling format is defined with 1Mhz timespamp on 13 bits
                pfTs=relTs*1000000/16384

                while ((pfTs - lastpfTsWrap) > (1<<13)) do
                    myfile << [0xff,0xff,0xff,0x7f].pack("c*")
                    lastpfTsWrap += (1<<13)
                end

                pfts = (pfTs - lastpfTsWrap)

                dataToFile = [tag & 0xff, (tag >> 8) & 0xff,
                            pfts & 0xff, (pfts >> 8) & 0xff]

                myfile << dataToFile.pack("c*")
            }
        }

    ensure
        myfile << [0xff,0xff,0xfe,0x7f].pack("c*")
        myfile.close

    end
end

@@mpRecordingDisplayThread = nil

def mpStartProfilingButtonCallback()
    file =  mpGetPrfName()
    if(file=="")
        return
    end
    
    if (!((file =~ /\.prf$/i)))
        file+=".prf"
        mpSetPrfNameLineEdit(file)
    end

    cwSetProfileEntry("lastMultiPrfForRecord",file)
    
    puts "html>Prf file for recording set to: <br><i>%s</i>" % file
    
	# Disable auto-polling for faster loading.
	cwDisableAutoPoll()
	sleep(0.3) # Time for previous command to take effect

    # Configuration
    cfg = mpRamMode?()
    if (cfg[0] == true)
        mode = ProfilerControl::PROFILE_IN_RAM
    else
        mode = ProfilerControl::PROFILE_ON_TRACE
    end

    size = cfg[1]


	# Start Profiling.
    startMultiprofiling(file, size, mode)
    
    # Fill progress bar with an infinite moving stuff.
    @@mpRecordingDisplayThread = Thread.new {
        i = 0
        mpSetProgressText("Recording ...")
        loop {
            mpSetProgress(i)
            sleep(0.05)
            i=i+1
            i = 0 if (i > 100)
        }
     }
rescue MultiProfilerExceptionBufferAlreadyAllocated
    wputs "Profiling already in progress!"
end

@@stopInProgress = false

def mpStopProfilingButtonCallback()
    if(@@stopInProgress)
       wputs "Stopping of the profiler already in progress!"
       return
    end

    @@stopInProgress = true
    begin
        if (@@mpRecordingDisplayThread)
            @@mpRecordingDisplayThread.kill
            @@mpRecordingDisplayThread = nil
        end

        # Reset Pogress Bar Test
        mpSetProgress(0)
        mpSetProgressText("Dumping %p%")
           
        lastMultiPrfForRecord = cwGetProfileEntry("lastMultiPrfForRecord","")
        if (lastMultiPrfForRecord == "")
             puts "No file chosen for recording!"
           return
        end

        # Disable auto-polling for faster loading.
        cwDisableAutoPoll()
        sleep(0.3) # Time for previous command to take effect
        
        # Configuration
        cfg = mpRamMode?()
        if (cfg[0] == true)
            mode = ProfilerControl::PROFILE_IN_RAM
        else
            mode = ProfilerControl::PROFILE_ON_TRACE
        end
        
        # Stop profiling.
        stopMultiprofiling(lastMultiPrfForRecord, mode)
        
        mpSetProgressText("Dumping done!")
    rescue MultiProfilerExceptionBufferAlreadyFreed
        # Do nothing
    rescue Exception => e
        puts "Error in MultiProfiler"
        CRExceptionPrint(e)
        puts "MultiProfiler stopped on error."

        # Reset 
        profilerControlReset()
        @@stopInProgress = false
        return

    ensure 
        @@stopInProgress = false
    end
end


end



