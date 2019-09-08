require "base.rb"
require "bufferProfile/parseprofiler.rb"
include ParseProfiler
require "bufferProfile/bufferprofileicons.rb"
#cwRegisterPlugin(File.dirname(__FILE__)+"/bufferprofile.dll")
PARSE_XCPU_BCPU_PROFILE = 0
PARSE_XCPU_PROFILE_ONLY = 1
PARSE_BCPU_PROFILE_ONLY = 2

module CWTraceprofiler

begin
    cwAddScriptButton("BufferProfileToolbar", "ParseprofilerChoosePrf()", BufferProfilePluginIcons::OPEN, "Choose save PRF file...")
    cwAddScriptButton("BufferProfileToolbar", "ParseprofilerGo(PARSE_XCPU_BCPU_PROFILE)", BufferProfilePluginIcons::GO, "Start dump and parsing..." )
    
    cwAddMenuCommand("BufferProfile", "Choose save PRF file...", "ParseprofilerChoosePrf()", 0)
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Start dump and parsing...", "ParseprofilerGo(PARSE_XCPU_BCPU_PROFILE)", 0)
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Start XCPU dump and parsing...", "ParseprofilerGo(PARSE_XCPU_PROFILE_ONLY)", 0)
    cwAddMenuCommand("BufferProfile", "Start BCPU dump and parsing...", "ParseprofilerGo(PARSE_BCPU_PROFILE_ONLY)", 0)
    cwAddMenuCommand("BufferProfile","","",0)
    cwAddMenuCommand("BufferProfile", "Add configuration Watch", "cwAddWatch($map_table.>.hal_access.>.ProfileOnTrace)", 0)
rescue #No CoolWatcher.
end

end


def ParseprofilerChoosePrf()
    lastParsePrfForRecord  = cwGetProfileEntry("lastParsePrfForRecord","")
    file = cwGetSaveFileName("Choose Saver PRF file for parsing",lastParsePrfForRecord,"*.prf")
    if(file=="?")
        return
    end
    lastParsePrfForRecord = file
    cwSetProfileEntry("lastParsePrfForRecord",lastParsePrfForRecord)
    puts "html>Prf file for parse cool profile set to: <br><i>%s</i>" % lastParsePrfForRecord
end

class BufferProfileAddrInfo
    attr_reader :start, :pos, :size
    attr_reader :bcpuStart, :bcpuPos, :bcpuSize
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
            bcpuStart = profileControlPtr.>.buffer_start.R
            bcpuPos = profileControlPtr.>.buffer_pos.R
            bcpuSize = profileControlPtr.>.buffer_size.R
            # Integration check
            if (isAddressInRam(bcpuStart) && bcpuPos < bcpuSize && bcpuSize < MAX_BCPU_PROFILE_SIZE)
                @bcpuStart = bcpuStart
                @bcpuPos = bcpuPos
                @bcpuSize = bcpuSize
            end
        end
    end
end

def ParseprofilerGo(profile)
    lastParsePrfForRecord = cwGetProfileEntry("lastParsePrfForRecord","")
    if(lastParsePrfForRecord == "")
        puts "No file chosen for recording!"
        return
    end
    xcpuPrfForRecord = lastParsePrfForRecord

    addrInfo = BufferProfileAddrInfo.new()

    if(profile == PARSE_XCPU_BCPU_PROFILE || profile == PARSE_XCPU_PROFILE_ONLY)
    
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
        startParseprofiling(xcpu_dumpbin_filename, xcpuPrfForRecord, size, pos)
    end

    if(addrInfo.bcpuStart != 0 && (profile == PARSE_XCPU_BCPU_PROFILE || profile == PARSE_BCPU_PROFILE_ONLY))
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
        startParseprofiling(bcpu_dumpbin_filename, bcpuPrfForRecord, size, pos)

        if(profile == PARSE_XCPU_BCPU_PROFILE)
            # Combine xcpu and bcpu profile data
            allPrfForRecord = lastParsePrfForRecord
            allPrfForRecord = allPrfForRecord[0, allPrfForRecord.length-4] +
                                    "_all" + allPrfForRecord[allPrfForRecord.length-4, 4];
    
            puts "Combine and parse both XCPU and BCPU profile ..."
            CombineAndParseProfileData(allPrfForRecord,
                                       xcpu_dumpbin_filename, addrInfo.size, addrInfo.pos,
                                       bcpu_dumpbin_filename, addrInfo.bcpuSize, addrInfo.bcpuPos)
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
    
    re = Regexp.new('^\s*0[xX][fF]{8}[8aA]0([0-9a-fA-F]{6})\s+' + varName + '\s*$')
    
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
            @start = @start & 0x1FFFFFFF
        else # Gallite or later
            file.seek(xcpuOffset+0x14, IO::SEEK_SET)
            xcpuBufStruct = file.read(0x24)
            @start = xcpuBufStruct[4] | xcpuBufStruct[5]<<8 |
                            xcpuBufStruct[6]<<16 | xcpuBufStruct[7]<<24
            @pos = xcpuBufStruct[12] | xcpuBufStruct[13]<<8 |
                            xcpuBufStruct[14]<<16 | xcpuBufStruct[15]<<24
            @size = xcpuBufStruct[8] | xcpuBufStruct[9]<<8 |
                            xcpuBufStruct[10]<<16 | xcpuBufStruct[11]<<24
            @start = @start & 0x1FFFFFFF
            @pos = @pos & 0x1FFFFFFF
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

def getBufferProfileInMemFile(lodMapFile=DEFAULT_LOD_MAP_FILE, sramBinFile=DEFAULT_SRAM_BIN_FILE)
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

    begin
        file = File.new(sramBinFile, "r")
    rescue => detail
        puts "Failed to open %s: %s" % [ sramBinFile, detail.to_str() ]
        return
    end
    begin
        file.binmode()

        addrInfo = BufferProfileAddrInfoInMemFile.new(file, halMapAccessOffsetInSram, bcpuBufProfileOffsetInSram)

        bufAddr = addrInfo.start
        bufPos = addrInfo.pos
        bufSize = addrInfo.size

        file.seek(bufAddr, IO::SEEK_SET)
        buf = file.read(bufSize<<2)
    rescue => detail
        file.close()
        puts "Failed to read %s: %s" % [ sramBinFile, detail.to_str() ]
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
        return
    end
    myfile.close()

    puts "Parse buffer profile data ..."
    startParseprofiling(memory_dumpbin_filename, lastParsePrfForRecord, bufSize, bufPos)
end

def getBufferProfileInMemFile0()
    getBufferProfileInMemFile()
end
