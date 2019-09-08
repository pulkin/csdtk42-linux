require "base.rb"
require "help.rb"
require "lod.rb"
require "ramrun.rb"

require "scanf"

require "benchmark.rb"


module FastPf
    
    NOBOOTSECTORFASTPF = 0
    BOOTSECTORFASTPF = 1
    FULLFASTPF = 2

    USE_FLASHPROG_FPCBUFFERSIZE = 1
    
    begin
        @@C = CH__flash_programmer_globals
        # @@intensiveVerify holds the RAM checking verify during fastpf.
        @@fastpfIntensiveVerify = false
    rescue
        puts "*** WARNING!! The flash programmer globals could not be loaded. This plugin won't work without the flash programmer xmd! ***"
    end
    
    @@fpcBufferSize = @@C::FPC_BUFFER_SIZE
    @@eraseSectorList = nil
    
    class FlashProgrammerNotReady < Exception
    end
    
    class FlashCommandUnacknowledged < Exception
    end 
    
    class FlashCommandTriggeredError < Exception
    end
    
    class FastpfVerifyFailed < Exception
    end
    
    class FastpfRAMVerifyFailed < FastpfVerifyFailed
    end
    
    class FastpfFLASHVerifyFailed < FastpfVerifyFailed
    end
    
    class FastpfVersionCheckError < Exception
    end

    def PACK32(uint32)
        [(uint32)&0xFF,(uint32>>8)&0xFF,(uint32>>16)&0xFF,(uint32>>24)&0xFF]
    end
    
public

    def waitCommandResult(connection,cmdsRunning,currentCmd,timeout=5)
        if(cmdsRunning[currentCmd])
            result = connection.waitMutlipleEvents([@@C::EVENT_FLASH_PROG_READY+currentCmd, @@C::EVENT_FLASH_PROG_ERROR+currentCmd, @@C::EVENT_FLASH_PROG_UNKNOWN+currentCmd],timeout)
            if(!result)
                raise FlashCommandUnacknowledged,"Event aknowledging end of command never received!"

            elsif(result==@@C::EVENT_FLASH_PROG_ERROR+currentCmd)
                raise FlashCommandTriggeredError, ("Received error event 0x%02x from target for command 0x%02x" % [result, @@C::EVENT_FLASH_PROG_READY+currentCmd] )
                
            elsif(result==@@C::EVENT_FLASH_PROG_UNKNOWN+currentCmd)
                raise FlashCommandTriggeredError, ("Received unknown command event 0x%02x from target for command 0x%02x" % [result, @@C::EVENT_FLASH_PROG_READY+currentCmd] )

            end
            #else it's ok.
        end    
    end
    
    
    def filterLodPackets(codeFileLodPackets,writeBootSector)
        # When fastpfing over USB, remove boot sector in flash list.
        case writeBootSector
            when NOBOOTSECTORFASTPF
                c1 = codeFileLodPackets.size
                codeFileLodPackets.reject!{ |i| i.address==$CS0.address}
                puts "Rejected boot sector (%d data packets)." % (c1 - codeFileLodPackets.size)
            when BOOTSECTORFASTPF
                codeFileLodPackets.reject!{ |i| i.address!=$CS0.address}
                puts "Warning: erasing the boot sector! (and discarding all other data, %d sectors to burn)." % (codeFileLodPackets.size)
            else # FULLFASTPF
                # Do nothing.
=begin
                # Tell a warning message if the boot sector is changed.
                codeFileLodPackets.each { |i|
                    if (i.address==$CS0.address)
                        puts "Warning: erasing the boot sector!"
                    end
                }
=end
        end
    end
        
    def extractBootSectorAddress(lod_filename)
        pat = /BOOT_SECTOR=([x0-9a-fA-F]+)/
        File.open(lod_filename, "r").each do |line|
            m = pat.match(line)
            if m
                return m[1].to_i(16)
            end
        end
        return 0
    end
    
    def sendFastpfCommand(connection,cmdStruct,ramAddr,size,flashAddr,fcs,cmd)
        
        #Prepare command structure locally
        cmdStruct.ramAddr.setl          ramAddr                    
        cmdStruct.size.setl             size
        cmdStruct.flashAddr.setl        flashAddr
        cmdStruct.cmd.setl              @@C::FPC_NONE
        cmdStruct.fcs.setl              fcs
        
        #Write the CMD structure in just one Write Block
        connection.writeBlock(cmdStruct.address, cmdStruct.memgetl)        
        cmdStruct.cmd.write(connection, cmd)
    end
    
    def checkHostCRCStatus(connection)
        crcstatus   = $INT_REG_DBG_HOST.CRC.CRC.read(connection)
        if(crcstatus != 0)
            return " ERROR!! The Host reported some transfer errors! Check your cable."
        else
            return " INFORMATION : No transfer errors were reported by the Host."
        end
    end
    
    def getFPCBufferSize(connection,flashProgAccess)
        if (USE_FLASHPROG_FPCBUFFERSIZE == 0)
            @@fpcBufferSize = @@C::FPC_BUFFER_SIZE
            # Checking that the FPC buffer sizes of the script and of the
            # flash programmer are the same.
            begin
                fpcSizeFlashProg = flashProgAccess.fpcSize.read(connection)
    
                if !((fpcSizeFlashProg == 64*1024) || (fpcSizeFlashProg == 32*1024) || (fpcSizeFlashProg == 4*1024))
                    puts "You are not using the latest version of the flash programmer."
                    # Assume that the FPC buffer of the flash programmer is the same as the script one.
                    fpcSizeFlashProg = @@fpcBufferSize
                end
            rescue
                puts "You are not using the latest version of the XMD."
                # Assume that the FPC buffer of the flash programmer is the same as the script one.
                fpcSizeFlashProg = @@fpcBufferSize
            end
            # (When the new version of the flash programmer will be in use,
            # we will be able to use fpcSizeFlashProg in the script, instead
            # of @@C::FPC_BUFFER_SIZE.)
            if (@@fpcBufferSize != fpcSizeFlashProg)
                sleep(0.5)
                crestart(connection, false)
                raise "FPC buffer size mismatch: script=%d, fp=%d" % [@@fpcBufferSize, fpcSizeFlashPro]
            end
        else
            # Getting the FPC buffer sizes from the flash programmer.
            begin
                @@fpcBufferSize = flashProgAccess.fpcSize.read(connection)
            rescue
                sleep(0.5)
                crestart(connection, false)
                raise "Failed to get the FPC buffer size from the flash programmer!"
            end
            if ((@@fpcBufferSize & 0x3) != 0)
                raise "FPC buffer size should be aligned in 4-byte boundary: %d !" % @@fpcBufferSize
            end
            puts "Got FPC buffer size: %d" % @@fpcBufferSize
        end
        if (@@fpcBufferSize > 32*1024)
            @@fpcBufferSize = 32*1024
        end
        puts "Use FPC buffer size: %d" % @@fpcBufferSize
    end
    
    def patchLodPacket(p,address,word)
        #Patches a lod packet with a 32-bit word
        if(p.address <= address && address < (p.address+p.data.size))
            offset = (address - p.address)
            p.data[4*offset] = word & 0xFF
            p.data[4*offset+1] = (word>>8) & 0xFF
            p.data[4*offset+2] = (word>>16) & 0xFF
            p.data[4*offset+3] = (word>>24) & 0xFF
            #Recalculate FCS
            p.rehash
            return true
        end
        return false
    end

    def dosetFastpfIntensiveVerify(enabled)
        @@fastpfIntensiveVerify = enabled
    end    
    
    def dofastpfXfer(connection, flash_programmer_filename, lod_filename, writeBootSector, verify)
        
        #We want the events 
        connection.enableEvents(true) 
        #A bit of cleaning is always welcomed
        connection.flushEvents() 
        
#######################################
#####           RAMRUN            #####
#######################################      
        
        flashProgrammerLodPackets = LOD_Read( flash_programmer_filename )
        
        #Ramrun the flash programmer
        puts "Ramrunning the flash programmer..."
        ramrunXfer(connection, flashProgrammerLodPackets, false)
     
puts Benchmark.measure{                  
        if(!connection.waitEvent(@@C::EVENT_FLASH_PROG_READY, 15))
            raise FlashProgrammerNotReady, "The flash programmer has not sent his READY event!"
        end
}
        
        #Store an image of the 2 cmds in memory
        flashProgAccess = $map_table.>(connection).flash_prog_access.>(connection)
        cmdClass = flashProgAccess.commandDescr[0].class
        cmd0addr = flashProgAccess.commandDescr[0].address
        cmd1addr = flashProgAccess.commandDescr[1].address
        cmds = [cmdClass.new(cmd0addr),cmdClass.new(cmd1addr)]
        
        #Read embedded versions of the protocol
        major = flashProgAccess.protocolVersion.major.read(connection)
        minor = flashProgAccess.protocolVersion.minor.read(connection)
        
        #Check versions
        if(major != @@C::FPC_PROTOCOL_MAJOR)
            raise  FastpfVersionCheckError, "Version mismatch between XMD and embedded flash programmer! (embedded version: %x.%x vs script version: %x.%x)" % [major, minor, @@C::FPC_PROTOCOL_MAJOR, @@C::FPC_PROTOCOL_MINOR]
        end
        if(minor != @@C::FPC_PROTOCOL_MINOR)
            puts "Version mismatch between XMD and embedded flash programmer! (embedded version: %x.%x vs script version: %x.%x)" % [major, minor, @@C::FPC_PROTOCOL_MAJOR, @@C::FPC_PROTOCOL_MINOR]
            puts "It should be compatible, but it is recommended to match XMD and embedded flash programmer"
        end
        
        puts "Verify enabled: " + verify.to_s
        puts "Fastpf Protocol Version: %d.%d" % [@@C::FPC_PROTOCOL_MAJOR & 0xFF,@@C::FPC_PROTOCOL_MINOR & 0xFF]

        getFPCBufferSize(connection,flashProgAccess)
        
        if (lod_filename == nil)
            raise "Sector list cannot be nil for erasion" if (@@eraseSectorList == nil)
            
            # Array of LODPackets representing the sectors to erase.
            codeFileLodPackets = [];
            
            @@eraseSectorList.each { |sectorAddress|
                p = LODPacket.new
                p.address = sectorAddress.to_i
                p.data = []
                p.fcsChunkSize = @@fpcBufferSize
                p.rehash
                codeFileLodPackets << p
            }
        else
            codeFileLodPackets = LOD_Read( lod_filename, @@fpcBufferSize )
        end

        bootSectorAddress = extractBootSectorAddress(lod_filename)
        puts "Boot sector address: 0x%x" % bootSectorAddress
        
        # Filter lod packets depending of the writeBootSector mode
        filterLodPackets(codeFileLodPackets,writeBootSector)          
        
        totaldata = 0           
        codeFileLodPackets.each { |p| 
            totaldata += p.data.size
        }
        
        #Store the 2 cmds running states (initially, it's false, then it will always be true)
        cmdsRunning = [false,false]

        #Store the buffer addresses, we will rotate on all of these
        buffers =   [flashProgAccess.dataBufferA.read(connection),
                     flashProgAccess.dataBufferB.read(connection),
                     flashProgAccess.dataBufferC.read(connection)]
                  
        currentCmd      = 0
        currentBuffer   = 0
        datadone        = 0

        if (bootSectorAddress != 0)
            sendFastpfCommand(connection,cmds[currentCmd],
                                0,
                                0,
                                bootSectorAddress,
                                0,
                                @@C::FPC_SET_BOOT_SECTOR)

            cmdsRunning[currentCmd]         = true
            waitCommandResult(connection, cmdsRunning,currentCmd)
            cmdsRunning[currentCmd]         = false
            currentCmd                      = (currentCmd + 1 )%2
        end
        
#######################################
#####           BURN              #####
#######################################        

        puts "Fastpfing..."
puts Benchmark.measure {
    codeFileLodPackets.each { |packet|

            #Wait for the cmd slot to be free
            waitCommandResult(connection, cmdsRunning,currentCmd)
            
            CRVERBOSE("Sector : 0x%08X (cmd:%d)" % [(packet.address - $CS0.address), currentCmd],2)
            
            # Send the ERASE SECTOR command
            # Address used by the flash programmer, relative to the flash base.
            # addresses stored in .lod files are absolute address (ie start with 0x01 ...)
            # so here we remove the address base of the Flash chip Select
            sendFastpfCommand(connection,cmds[currentCmd],
                                0,
                                packet.data.size,
                                packet.address - $CS0.address,
                                0,
                                @@C::FPC_ERASE)
            
            #Switch command
            cmdsRunning[currentCmd]         = true
            currentCmd                      = (currentCmd + 1 )%2
            
            dataStart   = 0
            fcsNum = 0
            while( dataStart < packet.data.size )
                
                #'slice' will take care of uncomplete data packets
                data = packet.data.slice(dataStart,@@fpcBufferSize)
                
                CRVERBOSE("Writing at 0x%08X (cmd:%d)" % [(packet.address-$CS0.address+dataStart), currentCmd], 2)
                
                #write packet in RAM 
                connection.writeBlock(buffers[currentBuffer], data)
                
                begin
                    waitCommandResult(connection, cmdsRunning, currentCmd)     # Wait for cmd slot to be free
                rescue FlashCommandTriggeredError => e
                    # Don't look further if we're not in intensive verify
                    raise e if(!(@@fastpfIntensiveVerify && verify))
                    
                    # Else, investigate
                    status = cmds[currentCmd].cmd.read(connection)
                    if(status == @@C::FPC_FCS_ERROR)
                        errmsg = "RAM verify failed during fastpf operation with intensive verify procedure!"
                        errmsg += checkHostCRCStatus(connection)
                        raise FastpfRAMVerifyFailed, errmsg
                    elsif(status == @@C::FPC_FLASH_NOT_AT_FF)
                        errmsg = "FLASH is not in proper state before writing inside. Not all bytes were at 0xFF!"
                        errmsg += checkHostCRCStatus(connection)
                        raise FastpfFLASHVerifyFailed, errmsg
                    else
                        #puts status
                        errmsg = "FLASH DRIVER ERROR: " + e.message
                        raise e.class, errmsg
                    end
                end
                
                # Slot is free. Send command.
                sendFastpfCommand(connection,cmds[currentCmd],
                                    buffers[currentBuffer],
                                    data.size,
                                    packet.address-$CS0.address+dataStart,
                                    packet.hashes[fcsNum] | ( (@@fastpfIntensiveVerify & verify)?(0x8000_0000):(0) ),
                                    @@C::FPC_PROGRAM)
                
                # Update progress. Call progress bar in given code block
                datadone += data.size
                yield(datadone*1.0/totaldata)   if(block_given?())
                
                cmdsRunning[currentCmd] = true                  # Set Command is in pipe.
                currentCmd              = (currentCmd + 1 )%2   # Switch command
                currentBuffer           = (currentBuffer + 1)%3 # Switch buffer

                dataStart               += @@fpcBufferSize
                fcsNum                  += 1
            end  
        }
} #Bench     

#######################################
#####          VERIFY             #####
#######################################

        if(verify)
            #Wait for the next command to be free
            waitCommandResult(connection, cmdsRunning, currentCmd)   # Wait for cmd slot to be free
        
 puts Benchmark.measure {
           # Do the verify
            verifyList = []
            codeFileLodPackets.each{ |p| 
                datadone = 0
                datalen = 0
                i = 0
                while(datadone < p.data.size)
                    #Cut into packets of 32K (because some flashes do not support bigger sectors)
                    datalen = (p.data.size - datadone>@@fpcBufferSize)?(@@fpcBufferSize):(p.data.size - datadone)
                    verifyList << (p.address+datadone-$CS0.address)
                    verifyList << (datalen)
                    verifyList << (p.hashes[i])
                    i += 1
                    datadone += datalen
                end
            }
            blockCount = (verifyList.length)/3
            
            puts "Verifying (%d blocks)..." % blockCount
            #write verifyList in RAM 
            connection.writeBlock(buffers[currentBuffer], verifyList.from32to8bits())
           
            #Launch check command
            sendFastpfCommand(connection,cmds[currentCmd], 
                                buffers[currentBuffer],
                                blockCount, 
                                0, 
                                0,
                                @@C::FPC_CHECK_FCS)
            
            cmdsRunning[currentCmd]         = true                  # Set Command is in pipe.
            
            #Wait for the other command to be finished
            currentCmd                      = (currentCmd + 1 )%2   # Switch command (since we executed one command)
            waitCommandResult(connection, cmdsRunning, currentCmd)  # Wait for cmd slot to be free
            cmdsRunning[currentCmd]         = false                 # Set Command is NOT in pipe.
            
            #Go back to verify command, wait for the result
            currentCmd                      = (currentCmd + 1 )%2   # Switch command
            begin
                waitCommandResult(connection, cmdsRunning,currentCmd,5)            # Set Command is in pipe.
            rescue FlashCommandTriggeredError
                failedBlock = cmds[currentCmd].ramAddr.read(connection)
                puts "First Failed Block : %d" % failedBlock
                failedSAdd  = verifyList[3*failedBlock]
                failedSize  = verifyList[3*failedBlock+1]  
                failedEAdd  = failedSAdd + failedSize         
                errmsg = "Fastpf Verify Failed between 0x%08X and 0x%08X! If you need more information, run the fastpfVerify command." % [failedSAdd, failedEAdd]
                errmsg += checkHostCRCStatus(connection)
                raise FastpfFLASHVerifyFailed, errmsg
            end

            #Go to the other command
            cmdsRunning[currentCmd]     = false                 # Set Command is NOT in pipe.
            currentCmd                  = (currentCmd + 1 )%2   # Switch command (since we executed one command)
            
            puts "Verify succeeded."   
} #Bench         
        end

#######################################
#####          FINALIZE           #####
#######################################
                  
        # Wait for the two commands in the pipe to be finished
        # (if we went through the verify, this is already done,
        # the waitCommandResult will return immediately)
        
        waitCommandResult(connection, cmdsRunning, currentCmd)  # Wait for cmd slot to be free
        cmdsRunning[currentCmd]         = false                 # Set Command is NOT in pipe.
        currentCmd                      = (currentCmd + 1 )%2   # Switch command        
        waitCommandResult(connection, cmdsRunning, currentCmd)  # Wait for cmd slot to be free
        cmdsRunning[currentCmd]         = false                 # Set Command is NOT in pipe.
        currentCmd                      = (currentCmd + 1 )%2   # Switch command
        
        puts "Finalizing..."
        sendFastpfCommand(connection,cmds[currentCmd],
                            0,
                            0,
                            0,
                            0,
                            @@C::FPC_END)

#######################################
#####          RESTART            #####
#######################################
                  
        cmdsRunning[currentCmd]         = true                  # Set Command is in pipe.
        waitCommandResult(connection, cmdsRunning, currentCmd)  # Wait for cmd slot to be free
        cmdsRunning[currentCmd]         = false                 # Set Command is NOT in pipe.
        currentCmd                      = (currentCmd + 1 )%2   # Switch command

        puts "Resetting the chip..."

        sendFastpfCommand(connection,cmds[currentCmd],
                            0,
                            0,
                            0,
                            0,
                            @@C::FPC_RESTART)

        cmdsRunning[currentCmd]         = true                  # Set Command is NOT in pipe.
        currentCmd                      = (currentCmd + 1 )%2   # Switch command        
        waitCommandResult(connection, cmdsRunning, currentCmd)  # Wait for cmd slot to be free
        cmdsRunning[currentCmd]         = false                 # Set Command is NOT in pipe.
        currentCmd                      = (currentCmd + 1 )%2   # Switch command        
        waitCommandResult(connection, cmdsRunning, currentCmd)  # Wait for cmd slot to be free

        puts "Fastpf done."
    end

    
    def dofastpfVerifyOnlyXfer(connection, flash_programmer_filename, lod_filename, writeBootSector, indepth)
        
        #We want the events 
        connection.enableEvents(true) 
        #A bit of cleaning is always welcomed
        connection.flushEvents() 

#######################################
#####           RAMRUN            #####
#######################################      

        flashProgrammerLodPackets = LOD_Read( flash_programmer_filename )

        #Ramrun the flash programmer
        puts "Ramrunning the flash programmer..."
        ramrunXfer(connection, flashProgrammerLodPackets, false)
                  
        if(!connection.waitEvent(@@C::EVENT_FLASH_PROG_READY, 15))
            raise FlashProgrammerNotReady, "The flash programmer has not sent his READY event!"
        end
        
        #Store an image of the 2 cmds in memory
        flashProgAccess = $map_table.>(connection).flash_prog_access.>(connection)
        cmdClass = flashProgAccess.commandDescr[0].class
        cmd0addr = flashProgAccess.commandDescr[0].address
        cmd1addr = flashProgAccess.commandDescr[1].address
        cmds = [cmdClass.new(cmd0addr),cmdClass.new(cmd1addr)]
        
        #Read embedded versions of the protocol
        major = flashProgAccess.protocolVersion.major.read(connection)
        minor = flashProgAccess.protocolVersion.minor.read(connection)
        
        #Check versions
        if(major != @@C::FPC_PROTOCOL_MAJOR || minor != @@C::FPC_PROTOCOL_MINOR)
            raise  FastpfVersionCheckError, "Version mismatch between XMD and embedded flash programmer! (embedded version: %x.%x vs script version: %x.%x)" % [major, minor, @@C::FPC_PROTOCOL_MAJOR, @@C::FPC_PROTOCOL_MINOR]
        end
        
        puts "Fastpf Protocol Version: %d.%d" % [@@C::FPC_PROTOCOL_MAJOR & 0xFF,@@C::FPC_PROTOCOL_MINOR & 0xFF]
        
        getFPCBufferSize(connection,flashProgAccess)
        
        if (lod_filename == nil)
            raise "LOD file name must be specified for verification!"
        else
            codeFileLodPackets = LOD_Read( lod_filename, @@fpcBufferSize )
        end
        
        # Filter lod packets depending of the writeBootSector mode
        filterLodPackets(codeFileLodPackets,writeBootSector)          
        
        totaldata = 0           
        codeFileLodPackets.each { |p| 
            totaldata += p.data.size
        }
        
        #Store the 2 cmds running states (initially, it's false, then it will always be true)
        cmdsRunning = [false,false]

        #Store the buffer addresses, we will rotate on all of these
        buffers =   [flashProgAccess.dataBufferA.read(connection),
                     flashProgAccess.dataBufferB.read(connection),
                     flashProgAccess.dataBufferC.read(connection)]
                  
        currentCmd      = 0
        currentBuffer   = 0
        datadone        = 0
        
#######################################
#####          VERIFY             #####
#######################################
    
puts Benchmark.measure {
       #Start to retrieve FINALIZE information
       puts "Retrieving finalize information..."
       sendFastpfCommand(connection, cmds[currentCmd], 
                            buffers[currentBuffer], 
                            0, 
                            0,
                            0, 
                            @@C::FPC_GET_FINALIZE_INFO) 
                            
       cmdsRunning[currentCmd]         = true                     # Set Command is in pipe.
       #Wait for the result.
       waitCommandResult(connection, cmdsRunning,currentCmd,5)

       magicSectorCount = cmds[currentCmd].size.read(connection)
       puts "Magic sector count found: %d" % magicSectorCount
       #Read info which is (address,magic)*magicSectorCount = (2*4)*magicSectorCount
       magicSectorInfo = connection.readBlock(buffers[currentBuffer],magicSectorCount*2*4).from8to32bits()
       
       #Patch the packets with the finalize information
       codeFileLodPackets.each{ |p|
            0.step(magicSectorInfo.size-1,2) { |m|
                if(patchLodPacket(p,magicSectorInfo[m]+$CS0.address,magicSectorInfo[m+1]))
                    puts "Patching LOD at %08X with finalize magic number %08X" % [magicSectorInfo[m],magicSectorInfo[m+1]]
                end
            }
       }
 
       cmdsRunning[currentCmd]         = false                 # Command has been treated      
       currentCmd                      = (currentCmd + 1 )%2   # Switch command
       currentBuffer                   = (currentBuffer + 1)%3 # Switch buffer
   
       # Do the verify
        verifyList = []
        codeFileLodPackets.each{ |p| 
            datadone = 0
            datalen = 0
            i = 0
            while(datadone < p.data.size)
                #Cut into packets of 32K (because some flashes do not support bigger sectors)
                datalen = (p.data.size - datadone>@@fpcBufferSize)?(@@fpcBufferSize):(p.data.size - datadone)
                verifyList << (p.address+datadone-$CS0.address)
                verifyList << (datalen)
                verifyList << (p.hashes[i])
                i += 1
                datadone += datalen
            end
        }
        blockCount = (verifyList.length)/3
        
        puts "Verifying (%d blocks)..." % blockCount
        
        #write verifyList in RAM 
        connection.writeBlock(buffers[currentBuffer], verifyList.from32to8bits() )
           
        #Launch check command
        sendFastpfCommand(connection,cmds[currentCmd], 
                            buffers[currentBuffer],
                            blockCount, 
                            0,
                            0, 
                            @@C::FPC_CHECK_FCS) 
   
        cmdsRunning[currentCmd]         = true                          # Set Command is in pipe.
        verifyBadBlocks = []
        begin
            waitCommandResult(connection, cmdsRunning,currentCmd,5)             # Set Command is in pipe.
        rescue FlashCommandTriggeredError
            # Reread ram buffer.
            puts "Hardware detected some errors!"
            verifyListValidated = connection.readBlock(buffers[currentBuffer], blockCount * 3 * 4).from8to32bits()
                        
            blockCount.times { |i|
                if( verifyListValidated[3*i + 2] != verifyList[3*i + 2])
                    failedSAdd  = verifyList[3*i]
                    failedSize  = verifyList[3*i+1]  
                    failedEAdd  = failedSAdd + failedSize
                    puts "Verify failed for block %d from address: 0x%08X to address: 0x%08X" % [i,failedSAdd,failedEAdd]
                    verifyBadBlocks << i
                end
            }
        end
        
        if(indepth && verifyBadBlocks.size()>0)
            puts "Doing in-depth check..."
            block = 0
            codeFileLodPackets.each{ |p| 
                datadone = 0
                datalen = 0
    
                while(datadone < p.data.size)
                    #Cut into packets of 32K (because some flashes do not support bigger sectors)
                    datalen = (p.data.size - datadone>@@fpcBufferSize)?(@@fpcBufferSize):(p.data.size - datadone)
    
                    if(verifyBadBlocks.index(block))
                        #Uh oh this block was marked as bad. Do a complete verify.
                        puts "Looking deeply into block %d :" % block
                        
                        dataontarget = connection.readBlock(p.address+datadone,datalen).from8to32bits()
                        dataonpc     = p.data[datadone..datadone+datalen-1].from8to32bits()
                        i = 0
                        dataonpc.each{ |e|
                            if(dataontarget[i] != e)
                                puts "Data at %08X is %08X and should be %08X" % [p.address+datadone-$CS0.address+4*i,dataontarget[i],e]
                            end
                            i += 1
                        }
                    end
                    
                    datadone += datalen               
                    block += 1
                end
            }
        end
  
        if(verifyBadBlocks.size() == 0)
            puts "html><font color=green>[VERIFY OK]</font>"
        else
            puts "html><font color=red>[VERIFY CONTAINED SOME ERRORS]</font>"
        end

        puts "Verify terminated."   
} #Bench         

    end


    def dofastpf(flash_programmer_filename, lod_filename, writeBootSector, verify)
        if ($CURRENTCONNECTION.implementsUsb)
            puts "html><b>Fastpf V2 over USB</b>"
        else
            puts "html><b>Fastpf V2 over Host</b>"
        end
        
        puts "html>Loading the lod files: <br><i>%s</i>" %File.basename(lod_filename)
        puts "html>Using flash programmer: <br><i>%s</i>" %File.basename(flash_programmer_filename)
        
        begin
            #Don't use directly the $CURRENTCONNECTION, it could polute other apps
            connection = $CURRENTCONNECTION.copyConnection()
            connection.open(false)
            dofastpfXfer(connection, flash_programmer_filename, lod_filename, writeBootSector, verify) { |prog|
                yield(prog) if(block_given?())       
            }
        ensure
            connection.close()
        end
    end
    
    def dofastpfVerifyOnly( flash_programmer_filename , lod_filename, writeBootSector, indepth )
        if ($CURRENTCONNECTION.implementsUsb)
            puts "html><b>Fastpf verify V2 over USB</b>"
        else
            puts "html><b>Fastpf verify V2 over Host</b>"
        end        

        puts "html>Loading the lod files: <br><i>%s</i>" %File.basename(lod_filename)
        puts "html>Using flash programmer: <br><i>%s</i>" %File.basename(flash_programmer_filename)

        begin
            #Don't use directly the $CURRENTCONNECTION, it could polute other apps
            connection = $CURRENTCONNECTION.copyConnection()
            connection.open(false)
            dofastpfVerifyOnlyXfer(connection, flash_programmer_filename, lod_filename, writeBootSector, indepth)
        ensure
            connection.close()
        end         
      
    end
    
end

def fastEraseProgress(prog)
    begin 
      cwSetProgress(prog*100,100,"%p% (erase all flash)")
        rescue Exception
        puts( prog*100 )
    end
end

def fastpfProgress(prog)
    begin 
        cwSetProgress(prog*100,100,"%p% (burning)")
    rescue Exception
        puts( prog*100 )
    end
end

addHelpEntry("fastpf", "setFastpfIntensiveVerify", "true_or_false","", "Changes the behaviour of fastpf by enabling/disabling the RAM checking step just after writing each block into RAM.")
def setFastpfIntensiveVerify(enabled)
    include FastPf
    dosetFastpfIntensiveVerify(enabled)
end

addHelpEntry("fastpf", "fastpfXfer", "connection, flash_programmer_filename, lod_filename, writeBootSector, verify","", "Low level fastpf, its purpose is to be integrated into script, and not launched in an interpreter. The 'connection' parameter should be a CHHostConnection or any heir of this class (see 'rbbase.rb'). 'flashProgrammerPackets' are LODPackets, and should be loaded with the 'LOD_Read()' function, as so as 'codeFilePackets'. They are respectively ruby objects representing the LOD files of the flashprogrammer, and the code to be fastpfed. If writeBootSector is 'true', the boot sector will be squashed. Else, it will be preserved.")
def fastpfXfer(connection, flash_programmer_filename, lod_filename, writeBootSector, verify)
    include FastPf
    FastPf.dofastpfXfer(connection, flash_programmer_filename, lod_filename, writeBootSector, verify) { |prog|
        yield(prog) if(block_given?())
    }
end
  
addHelpEntry("fastpf", "fastpf", "flash_programmer_filename, writeBootsector=FastPf::FULLFASTPF, verify=true, disable_event_sniffer=true","", "Do a fastpf from files. 'flash_programmer_filename' is the path of the compiled flash programmer (compiled as a ramrun for the particular flash type you are going to use). 'lod_filename' is the path to the compiled source code to be 'fastpfed'. Possible values for writeBootSector are FastPf::NOBOOTSECTORFASTPF, FastPf::BOOTSECTORFASTPF, and FastPf::FULLFASTPF.")
def fastpf(flash_programmer_filename, lod_filename, writeBootsector=FastPf::FULLFASTPF , verify=true, disable_event_sniffer=true )
    include FastPf

    begin
        enforce{ cwDisableAutoPoll}
        # The event sniffer is a coolwatcher feature only
        enforce{ dontSniffEvents if(disable_event_sniffer) }
            
        dofastpf(flash_programmer_filename, lod_filename, writeBootsector, verify) { |prog|
            if(block_given?())
                yield(prog)
            else 
                fastpfProgress(prog)
            end
        }

    ensure 
        enforce{ flushEvents() }
        enforce{ sniffEvents() }
    end
end

addHelpEntry("fastpf", "fastpfVerify", "flash_programmer_filename, lod_filename, writeBootsector=FastPf::FULLFASTPF, indepth=true, disable_event_sniffer = true","", "Do a fastpf verify with a specified flash programmer and lod file.")
def fastpfVerify( flash_programmer_filename, lod_filename, writeBootsector=FastPf::FULLFASTPF, indepth=true, disable_event_sniffer = true )
    include FastPf

    begin
        enforce{ cwDisableAutoPoll}
        # The event sniffer is a coolwatcher feature only
        enforce{ dontSniffEvents if(disable_event_sniffer) }
        
        dofastpfVerifyOnly(flash_programmer_filename, lod_filename, writeBootsector, indepth)
    ensure
        enforce{ flushEvents() }
        enforce{ sniffEvents() }
    end
end

addHelpEntry("fastpf", "fastSectorEraser", "flash_programmer_filename, sector_list, disable_event_sniffer = true","", "Do a fastpf to erase all the sectors whose address present in the integer array 'sector_list'.")
def fastSectorEraser( flash_programmer_filename, sector_list, disable_event_sniffer = true )
    
    @@eraseSectorList = sector_list    
    
    fastpf(flash_programmer_filename, nil, FastPf::FULLFASTPF, disable_event_sniffer) { |prog|
        yield(prog) if(block_given?())
    }
    
    @@eraseSectorList = nil
end

#########################################################
#                 FOR DEBUGGING PURPOSE                 #
#########################################################

def fastpfHeavyTest( flash_prog, lod_file, trycount )
    succ = 0
    fail = 0
    trycount.times{ |i|
        begin
            puts "Launching fastpf number: %d." % i
            fastpf(flash_prog, lod_file)
            succ += 1
        rescue Exception => e
            puts "Woops, that one failed. Detail:"
            puts e
            puts e.backtrace
            fail += 1
        end    
    }
    
    puts "Succeeded: %d. Failed: %d." % [succ,fail]
end
