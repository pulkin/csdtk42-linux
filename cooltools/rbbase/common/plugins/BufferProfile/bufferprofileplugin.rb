require "base.rb"
require "BufferProfile/parseprofiler.rb"
include ParseProfiler
require "BufferProfile/bufferprofileicons.rb"

PARSE_ALL_PROFILE = 0
PARSE_XCPU_PROFILE_ONLY = 1
PARSE_BCPU_PROFILE_ONLY = 2
PARSE_WCPU_PROFILE_ONLY = 3

module CWTraceprofiler

begin
    cwAddScriptButton("BufferProfileToolbar", "parseProfilerChoosePrf()", BufferProfilePluginIcons::OPEN, "Choose save PRF file...")
    cwAddScriptButton("BufferProfileToolbar", "parseProfilerGo(PARSE_ALL_PROFILE)", BufferProfilePluginIcons::GO, "Start dump and parsing..." )
    
    cwAddMenuCommand("BufferProfile", "Choose save PRF file...", "parseProfilerChoosePrf()", 0)
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Pause buffer profile", "parseProfilerPause()", 0)
    cwAddMenuCommand("BufferProfile", "Resume buffer profile", "parseProfilerResume()", 0)
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Start dump and parsing...", "parseProfilerGo(PARSE_ALL_PROFILE)", 0)
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Start XCPU dump and parsing...", "parseProfilerGo(PARSE_XCPU_PROFILE_ONLY)", 0)
    cwAddMenuCommand("BufferProfile", "Start BCPU dump and parsing...", "parseProfilerGo(PARSE_BCPU_PROFILE_ONLY)", 0)
    
    if(!$wd_mailbox.nil?)
        cwAddMenuCommand("BufferProfile", "Start WCPU dump and parsing...", "parseProfilerGo(PARSE_WCPU_PROFILE_ONLY)", 0)
    end
    
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Add configuration Watch", "cwAddWatch($map_table.>.hal_access.>.profileControl)", 0)
rescue #No CoolWatcher.
end

end


def parseProfilerChoosePrf()
    lastParsePrfForRecord  = cwGetProfileEntry("lastParsePrfForRecord","")
    file = cwGetSaveFileName("Choose Saver PRF file for parsing",lastParsePrfForRecord,"*.prf")
    if(file=="?")
        return
    end
    lastParsePrfForRecord = file
    cwSetProfileEntry("lastParsePrfForRecord",lastParsePrfForRecord)
    puts "html>Prf file for parse cool profile set to: <br><i>%s</i>" % lastParsePrfForRecord
end

def parseProfilerPause()
    $map_table.>.hal_access.>.profileControl.config.w(1)
end

def parseProfilerResume()
    $map_table.>.hal_access.>.profileControl.config.w(0)
end

class BufferProfileAddrInfo
    attr_reader :start, :pos, :size
    attr_reader :bcpuStart, :bcpuPos, :bcpuSize
    attr_reader :wcpuStart, :wcpuPos, :wcpuSize, :wcpuConfig
    attr_reader :rfdigStart, :rfdigPos, :rfdigSize

    # BCPU profile size should be less than 16K items, or 64K bytes
    MAX_BCPU_PROFILE_SIZE = 16*1024
 
    def initialize()
        if($PROJNAME.split[0].casecmp("Greenstone") == 0)
            @start = $map_table.>.hal_access.>.ProfileOnBuffer.buffer_start.R
            @pos = $map_table.>.hal_access.>.ProfileOnBuffer.buffer_pos.R
            @size =  $map_table.>.hal_access.>.ProfileOnBuffer.buffer_size.R
        else # Gallite or later
            @start = $map_table.>.hal_access.>.profileControl.startAddr.R
            @pos = $map_table.>.hal_access.>.profileControl.writePointer.R
            @pos = (@pos - @start) / 4
            @size =  $map_table.>.hal_access.>.profileControl.size.R
            @size = @size / 4
        end

        @bcpuStart = @bcpuPos = @bcpuSize = 0

        if( $PROJNAME.split[0].casecmp("Greenstone") == 0 ||
            $PROJNAME.split[0].casecmp("Gallite") == 0 ||
            $PROJNAME.split[0].casecmp("8808") == 0
          )
            # BCPU profile is reusing hal_version
            bcpu_profile_buf_access = $map_table.>.hal_version.>.date.R
            
            if(isAddressInRam(bcpu_profile_buf_access))
                bcpuStart = R(bcpu_profile_buf_access)
                bcpuPos = R(bcpu_profile_buf_access+4)
                bcpuSize = R(bcpu_profile_buf_access+8)
                # Integration check
                if (isAddressInRam(bcpuStart) && bcpuPos < bcpuSize && bcpuSize < MAX_BCPU_PROFILE_SIZE)
                    @bcpuStart = bcpuStart
                    @bcpuPos = bcpuPos
                    @bcpuSize = bcpuSize
                end
            end
        else # 8809 or later
            profileControlPtr = $map_table.>.spal_access.>.profileControlPtr
            # Integration check
            if(isAddressInRam(profileControlPtr.RU))
                bcpuStart = profileControlPtr.>.buffer_start.RU
                bcpuPos = profileControlPtr.>.buffer_pos.RU
                bcpuSize = profileControlPtr.>.buffer_size.RU
                # Integration check
                if(isAddressInRam(bcpuStart) && bcpuPos < bcpuSize && bcpuSize < MAX_BCPU_PROFILE_SIZE)
                    @bcpuStart = bcpuStart
                    @bcpuPos = bcpuPos
                    @bcpuSize = bcpuSize
                else
                    puts "Invalidate bcpu Start Address = 0x%x" % bcpuStart  
                end
            end
        end

        @wcpuStart = @wcpuPos = @wcpuSize = @wcpuConfig = 0

        if(!$wd_mailbox.nil?)
            profileControlPtr = $wd_mailbox.WDDbg_profileControlPtr
            # Integration check
            if(isAddressInRam(profileControlPtr.RU))
                wcpuStart = profileControlPtr.>.buffer_start.RU
                wcpuPos = profileControlPtr.>.buffer_pos.RU
                wcpuSize = profileControlPtr.>.buffer_size.RU
                wcpuConfig = profileControlPtr.>.config.RU
                # Integration check
                if(isAddressInRam(wcpuStart) && wcpuPos < wcpuSize)
                    @wcpuStart = wcpuStart
                    @wcpuPos = wcpuPos
                    @wcpuSize = wcpuSize
                    @wcpuConfig = wcpuConfig
                else
                    puts "Invalidate wcpu Start Address = 0x%x" % wcpuStart  

                end
            else
                 puts "Invalidate WDDbg_profileControlPtr Address = 0x%x" % profileControlPtr.RU          
            end
        end

        @rfdigStart = @rfdigPos = @rfdigSize = 0
        begin
            if (!$map_table.>.hal_access.>.rfdigProfileControl.nil?)
                @rfdigStart = $map_table.>.hal_access.>.rfdigProfileControl.>.startAddr.R
                @rfdigPos = $map_table.>.hal_access.>.rfdigProfileControl.>.writePosition.R
                @rfdigSize = $map_table.>.hal_access.>.rfdigProfileControl.>.size.R

                @rfdigPos = @rfdigPos / 4
                @rfdigSize = @rfdigSize / 4
            end
        rescue
            # rfdigProfileControl may not exist
        end
    end
end

def parseProfilerGo(profile)
    lastParsePrfForRecord = cwGetProfileEntry("lastParsePrfForRecord","")
    if(lastParsePrfForRecord == "")
        puts "No file chosen for recording!"
        return
    end
    xcpuPrfForRecord = lastParsePrfForRecord

    addrInfo = BufferProfileAddrInfo.new()
    bufferAll = Array.new(0)

    if(profile == PARSE_ALL_PROFILE || profile == PARSE_XCPU_PROFILE_ONLY)
        address = addrInfo.start
        pos = addrInfo.pos    
        size = addrInfo.size 
       
        puts "XCPU Buffer Address = 0x%x" % address
        puts "XCPU Record Position = 0x%x" % pos
        puts "XCPU Record Number = 0x%x" % size
    
        xcpu_dumpbin_filename = xcpuPrfForRecord[0, xcpuPrfForRecord.length-3]
        xcpu_dumpbin_filename = xcpu_dumpbin_filename + "bin"
        #xcpu_dumpbin_filename = "C:/CSDTK/cooltools/pxts_buffer_test.bin"
    
        puts "Dump XCPU profile ..."
        dump(xcpu_dumpbin_filename, address, size)
    
        #myfile = File.open("www.dat", "ab")
        #a = Array.new(4)
        #a[0]= size&0xff
        #a[1] = (size&0xff00)>>8
        #a[2] = (size&0xff0000)>>16
        #a[3] = (size&0xff0000)>>24
        #myfile.write a[0..3].pack("c*")
        #myfile.close
    
        myfile = File.open(xcpu_dumpbin_filename, "ab")
        myfile.puts "XCPU Buffer Address = 0x%x" % address
        myfile.puts "XCPU Record Position = 0x%x" % pos
        myfile.puts "XCPU Record Number = 0x%x" % size
        myfile.close
    
        sleep(0.3) # Time for previous command to take effect
    
        puts "Parse XCPU profile ..."
        buffer = startParseProfiling(xcpu_dumpbin_filename, xcpuPrfForRecord, size, pos)
        bufferAll = combineProfileData(bufferAll, buffer)
    end

    if(addrInfo.bcpuStart != 0 && (profile == PARSE_ALL_PROFILE || profile == PARSE_BCPU_PROFILE_ONLY))
        puts "BCPU buffer profile exists!"

        address = addrInfo.bcpuStart
        pos = addrInfo.bcpuPos
        size = addrInfo.bcpuSize

        puts "BCPU Buffer Address = 0x%x" % address
        puts "BCPU Record Position = 0x%x" % pos
        puts "BCPU Record Number = 0x%x" % size

        bcpuPrfForRecord = lastParsePrfForRecord
        bcpuPrfForRecord = bcpuPrfForRecord[0, bcpuPrfForRecord.length-4] +
                                "_bcpu" + bcpuPrfForRecord[bcpuPrfForRecord.length-4, 4];

        bcpu_dumpbin_filename = bcpuPrfForRecord[0, bcpuPrfForRecord.length-3]
        bcpu_dumpbin_filename = bcpu_dumpbin_filename + "bin"

        puts "Dump BCPU profile ..."
        dump(bcpu_dumpbin_filename, address, size)

        # 8809 profile workaround:
        # Fix overflow issue when saving IFC2 symbol number
        if($PROJNAME.split[0].casecmp("8809") == 0)
            bcpu_org_8809_filename = bcpu_dumpbin_filename + "_org"
            File.rename(bcpu_dumpbin_filename, bcpu_org_8809_filename)

            bcpuOrgFile = File.open(bcpu_org_8809_filename, "rb")
            bcpuOrgData = bcpuOrgFile.read
            bcpuOrgFile.close

            myfile = File.open(bcpu_org_8809_filename, "ab")
            myfile.puts "BCPU Buffer Address = 0x%x" % address
            myfile.puts "BCPU Record Position = 0x%x" % pos
            myfile.puts "BCPU Record Number = 0x%x" % size
            myfile.close

            bcpuOrgDataLen = bcpuOrgData.length
            i = 0
            while (i+4 <= bcpuOrgDataLen)
                bcpuEvent = (bcpuOrgData[i+1]<<8)|bcpuOrgData[i]
                # IFC2 start address ranges from 0xa1981400 to 0xa1983260.
                # The low half word of ((0-Ifc2StartAddr)/4+0x7000) ranges from 0x6b00 to 0x6368.
                if (bcpuEvent >= 0x6368 && bcpuEvent <= 0x6b00)
                    bcpuOrgData[i] = 0xff
                    bcpuOrgData[i+1] = 0x73
                end
                i += 4
            end

            myfile = File.open(bcpu_dumpbin_filename, "wb")
            myfile.write(bcpuOrgData)
            myfile.close
        end

        myfile = File.open(bcpu_dumpbin_filename, "ab")
        myfile.puts "BCPU Buffer Address = 0x%x" % address
        myfile.puts "BCPU Record Position = 0x%x" % pos
        myfile.puts "BCPU Record Number = 0x%x" % size
        myfile.close

        sleep(0.3) # Time for previous command to take effect

        puts "Parse BCPU profile ..."
        buffer = startParseProfiling(bcpu_dumpbin_filename, bcpuPrfForRecord, size, pos)
        bufferAll = combineProfileData(bufferAll, buffer)
    end

    if(addrInfo.rfdigStart != 0 && profile == PARSE_ALL_PROFILE)
        puts "RFDIG buffer profile exists!"

        address = addrInfo.rfdigStart
        pos = addrInfo.rfdigPos
        size = addrInfo.rfdigSize

        puts "RFDIG Buffer Address = 0x%x" % address
        puts "RFDIG Record Position = 0x%x" % pos
        puts "RFDIG Record Number = 0x%x" % size

        prfForRecord = lastParsePrfForRecord
        prfForRecord = prfForRecord[0, prfForRecord.length-4] + "_rfdig" + prfForRecord[prfForRecord.length-4, 4];

        dumpbin_filename = prfForRecord[0, prfForRecord.length-3]
        dumpbin_filename = dumpbin_filename + "bin"

        puts "Dump RFDIG profile ..."
        dump(dumpbin_filename, address, size)

        myfile = File.open(dumpbin_filename, "ab")
        myfile.puts "RFDIG Buffer Address = 0x%x" % address
        myfile.puts "RFDIG Record Position = 0x%x" % pos
        myfile.puts "RFDIG Record Number = 0x%x" % size
        myfile.close

        sleep(0.3) # Time for previous command to take effect

        puts "Parse RFDIG profile ..."
        buffer = startParseProfiling(dumpbin_filename, prfForRecord, size, pos)
        bufferAll = combineProfileData(bufferAll, buffer)
    end

    if(profile == PARSE_ALL_PROFILE)
        allPrfForRecord = lastParsePrfForRecord
        allPrfForRecord = allPrfForRecord[0, allPrfForRecord.length-4] +
                                "_all" + allPrfForRecord[allPrfForRecord.length-4, 4];

        puts "Combine all ..."
        writeParsedProfiling(allPrfForRecord, bufferAll)
    end

    if(addrInfo.wcpuStart != 0 && (profile == PARSE_ALL_PROFILE || profile == PARSE_WCPU_PROFILE_ONLY))
        puts "WCPU buffer profile exists!"

        address = addrInfo.wcpuStart
        pos = addrInfo.wcpuPos
        size = addrInfo.wcpuSize
        config = addrInfo.wcpuConfig
        freq = config>>16
        
        puts "WCPU Buffer Address = 0x%x" % address
        puts "WCPU Record Position = 0x%x" % pos
        puts "WCPU Record Number = 0x%x" % size
				puts "WCPU Record config = 0x%x" % config

        wcpuPrfForRecord = lastParsePrfForRecord
        wcpuPrfForRecord = wcpuPrfForRecord[0, wcpuPrfForRecord.length-4] +
                            "_wcpu" + wcpuPrfForRecord[wcpuPrfForRecord.length-4, 4];

        wcpu_dumpbin_filename = wcpuPrfForRecord[0, wcpuPrfForRecord.length-3]
        wcpu_dumpbin_filename = wcpu_dumpbin_filename + "bin"

        puts "Dump WCPU profile ..."
        dump(wcpu_dumpbin_filename, address, size)

        myfile = File.open(wcpu_dumpbin_filename, "ab")
        myfile.puts "WCPU Buffer Address = 0x%x" % address
        myfile.puts "WCPU Record Position = 0x%x" % pos
        myfile.puts "WCPU Record Number = 0x%x" % size
        myfile.close

        sleep(0.3) # Time for previous command to take effect

        puts "Parse WCPU profile ..."
        if(freq == 0 || freq == 0xffff)
        	startParseProfiling(wcpu_dumpbin_filename, wcpuPrfForRecord, size, pos)
        else
           puts "WCPU freq =0x%x" % freq
        	startParseProfiling(wcpu_dumpbin_filename, wcpuPrfForRecord, size, pos,freq*4096)
        end
    end
end

def getGlobalVarOffset(mapfile, varName)
    begin
        file = File.new(mapfile, "r")
    rescue => detail
        puts "Failed to open #{mapfile}: %s" % detail.to_str()
        return -1
    end
    
    re = Regexp.new('^\s*0[xX][fF]{8}[8aA][0-9a-fA-F]([0-9a-fA-F]{6})\s+' + varName + '\s*$')
    
    file.each { |line|
        offset = line[re, 1]
        if (offset != nil)
            file.close()
            return offset.hex()
        end
    }
    
    file.close()
    return -1
end

DEFAULT_LOD_MAP_FILE = "sram.map"
DEFAULT_SRAM_BIN_FILE = "sram.bin"

class BufferProfileAddrInfoInMemFile
    attr_reader :start, :pos, :size
    attr_reader :bcpuStart, :bcpuPos, :bcpuSize
    
    def initialize(file, xcpuOffset, bcpuOffset)
        if($PROJNAME.split[0].casecmp("Greenstone") == 0)
            file.seek(xcpuOffset+0x1C, IO::SEEK_SET)
            xcpuBufStruct = file.read(0xC)
            @start = xcpuBufStruct[0] | xcpuBufStruct[1]<<8 |
                            xcpuBufStruct[2]<<16 | xcpuBufStruct[3]<<24
            @pos = xcpuBufStruct[4] | xcpuBufStruct[5]<<8 |
                            xcpuBufStruct[6]<<16 | xcpuBufStruct[7]<<24
            @size = xcpuBufStruct[8] | xcpuBufStruct[9]<<8 |
                            xcpuBufStruct[10]<<16 | xcpuBufStruct[11]<<24
            @start = @start & 0x00FFFFFF
        else # Gallite or later
            file.seek(xcpuOffset+0x18, IO::SEEK_SET)
            xcpuBufStruct = file.read(0x24)
            @start = xcpuBufStruct[4] | xcpuBufStruct[5]<<8 |
                            xcpuBufStruct[6]<<16 | xcpuBufStruct[7]<<24
            @pos = xcpuBufStruct[12] | xcpuBufStruct[13]<<8 |
                            xcpuBufStruct[14]<<16 | xcpuBufStruct[15]<<24
            @size = xcpuBufStruct[8] | xcpuBufStruct[9]<<8 |
                            xcpuBufStruct[10]<<16 | xcpuBufStruct[11]<<24
            @start = @start & 0x00FFFFFF
            @pos = @pos & 0x00FFFFFF
            @pos = (@pos - @start) / 4
            @size = @size / 4
        end
        
        @bcpuStart = @bcpuPos = @bcpuSize = 0
        if(bcpuOffset != -1)
            file.seek(bcpuOffset, IO::SEEK_SET)
            bcpuBufStruct = file.read(12)
            
            @bcpuStart = bcpuBufStruct[0] | bcpuBufStruct[1]<<8 |
                            bcpuBufStruct[2]<<16
            @bcpuPos = bcpuBufStruct[4] | bcpuBufStruct[5]<<8 |
                            bcpuBufStruct[6]<<16 | bcpuBufStruct[7]<<24
            @bcpuSize = bcpuBufStruct[8] | bcpuBufStruct[9]<<8 |
                            bcpuBufStruct[10]<<16 | bcpuBufStruct[11]<<24
        end
    end
end

addHelpEntry("chip", "getProfileInMemFile", "sramBinFile, lodMapFile", "",
    "Get the buffer profile from a dumped memory binary file. 'sramBinFile' defaults to \"#{DEFAULT_SRAM_BIN_FILE}\"; 'lodMapFile' defaults to \"#{DEFAULT_LOD_MAP_FILE}\".")
def getProfileInMemFile(sramBinFile=DEFAULT_SRAM_BIN_FILE, lodMapFile=DEFAULT_LOD_MAP_FILE)
    lastParsePrfForRecord = cwGetProfileEntry("lastParsePrfForRecord","")
    if (lastParsePrfForRecord == "")
        puts "No file chosen for recording!"
        return
    end

    halMapAccessOffsetInSram = getGlobalVarOffset(lodMapFile, "g_halMapAccess")
    if (halMapAccessOffsetInSram == -1)
        raise "Failed to get g_halMapAccess offset from #{lodMapFile}"
        return
    end
    puts "halMapAccessOffsetInSram = 0x%08x" % halMapAccessOffsetInSram

    begin
        file = File.new(sramBinFile, "r")
    rescue => detail
        puts "Failed to open %s: %s" % [ sramBinFile, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    begin
        file.binmode()

        addrInfo = BufferProfileAddrInfoInMemFile.new(file, halMapAccessOffsetInSram, -1)

        bufAddr = addrInfo.start
        bufPos = addrInfo.pos
        bufSize = addrInfo.size

        file.seek(bufAddr, IO::SEEK_SET)
        buf = file.read(bufSize<<2)
    rescue => detail
        file.close()
        puts "Failed to read %s: %s" % [ sramBinFile, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    file.close()

    puts "Read buffer profile data ..."
    puts "\t0x%04x bytes @ offset 0x%08x" % [ buf.length(), bufAddr ]

    memory_dumpbin_filename = lastParsePrfForRecord[0, lastParsePrfForRecord.length-3]
    memory_dumpbin_filename = memory_dumpbin_filename + "bin"

    begin
        myfile=File.new(memory_dumpbin_filename, "wb")
    rescue => detail
        puts "Failed to open %s: %s" % [ memory_dumpbin_filename, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    begin
        myfile.syswrite(buf)

        myfile.syswrite("Buffer Address = 0x%x\n" % bufAddr)
        myfile.syswrite("Record Position = 0x%x\n" % bufPos)
        myfile.syswrite("Record Number = 0x%x\n" % bufSize)
    rescue => detail
        file.close()
        puts "Failed to write %s: %s" % [ memory_dumpbin_filename, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    myfile.close()

    puts "Parse buffer profile data ..."
    startParseProfiling(memory_dumpbin_filename, lastParsePrfForRecord, bufSize, bufPos)
end

addHelpEntry("chip", "getBbProfileInMemFile", "sramBinFile, lodMapFile", "dumpbin",
    "Get BB buffer profile from a dumped memory binary file. 'sramBinFile' defaults to \"#{DEFAULT_SRAM_BIN_FILE}\"; 'lodMapFile' defaults to \"#{DEFAULT_LOD_MAP_FILE}\", lastParsePrfForRecord default to default.prf.")
def getBbProfileInMemFile(sramBinFile=DEFAULT_SRAM_BIN_FILE, lodMapFile=DEFAULT_LOD_MAP_FILE, lastParsePrfForRecord="default.prf")

    spalMapAccessOffsetInSram = getGlobalVarOffset(lodMapFile, "g_spalProfileControl") - 0x80000
    if (spalMapAccessOffsetInSram == -1)
        raise "Failed to get spalMapAccessOffsetInSram offset from #{lodMapFile}"
        return
    end
    puts "spalMapAccessOffsetInSram = 0x%08x" % spalMapAccessOffsetInSram
    
    begin
        file = File.new(sramBinFile, "r")
    rescue => detail
        puts "Failed to open %s: %s" % [ sramBinFile, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    
    begin
        file.binmode()
	
	 file.seek(spalMapAccessOffsetInSram, IO::SEEK_SET)
        bcpuBufStruct = file.read(16)
        
        bufAddr = (bcpuBufStruct[4] | bcpuBufStruct[5]<<8 | bcpuBufStruct[6]<<16)  - 0x80000
        bufPos = (bcpuBufStruct[8] | bcpuBufStruct[9]<<8 )
        bufSize = ( bcpuBufStruct[10] | bcpuBufStruct[11]<<8)

        puts "bufAddr=0x%X" %bufAddr
        puts "bufPos=0x%X" %bufPos
        puts "bufSize=0x%X" %bufSize

        
        file.seek(bufAddr, IO::SEEK_SET)
        buf = file.read(bufSize<<2)
    rescue => detail
        file.close()
        puts "Failed to read %s: %s" % [ sramBinFile, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    
    file.close()
    
    puts "Read buffer profile data ..."
    puts "\t0x%04x bytes @ offset 0x%08x" % [ buf.length(), bufAddr ]

    memory_dumpbin_filename = lastParsePrfForRecord[0, lastParsePrfForRecord.length-3]
    memory_dumpbin_filename = memory_dumpbin_filename + "bin"
    
    begin
        myfile=File.new(memory_dumpbin_filename, "wb")
    rescue => detail
        puts "Failed to open %s: %s" % [ memory_dumpbin_filename, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    
    begin
        myfile.write(buf)

        myfile.write("address=0x%x\n" % bufAddr)
        myfile.write("pos=0x%x\n" % bufPos)
        myfile.write("size=0x%x\n" % bufSize)
    rescue => detail
        file.close()
        puts "Failed to write %s: %s" % [ memory_dumpbin_filename, detail.to_str() ]
        puts detail.backtrace().join("\n")
        return
    end
    myfile.close()
    puts "Parse buffer profile data ..."
    startParseProfiling(memory_dumpbin_filename, lastParsePrfForRecord, bufSize, bufPos)
end

addHelpEntry("chip", "getProfileInMemFile0", "", "",
    "Get the buffer profile from the dumped memory binary file, \"#{DEFAULT_SRAM_BIN_FILE}\", based on the map file \"${DEFAULT_LOD_MAP_FILE}\".")
def getProfileInMemFile0()
    getProfileInMemFile()
end
