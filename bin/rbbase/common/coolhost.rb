#!/usr/bin/env ruby
include CoolHost

require 'help.rb'

addHelpEntry("CoolHost","CHST_Open","comport, baudrate, flowmode","comhandle","Opens a communication handle with a CoolHost launched on COM port 'comport' with baud rate 'baudrate' and with flow control mode 'flowmode' (if such a CoolHost does not exist, it is launched automatically). If successful, sets the '$CURRENTCOMHANDLE' variable to the number attributed handle, and returns the number of this handle. Otherwise, raises an exception. CAUTION!! If you are using Jade, use CHST_JadeOpen instead.",false)

addHelpEntry("CoolHost","CHST_JadeOpen","comport, baudrate","comhandle","Same as CHST_Open, but specific for the Jade Chip.",false)

addHelpEntry("CoolHost","CHST_BypassOpen","comport, baudrate, flowmode, flowid_array","comhandle","Opens a bypass communication on a CoolHost launched on COM port 'comport' with baud rate 'baudrate' aand with flow control mode 'flowmode' (if such a CoolHost does not exist, it is launched automatically). The flowids must be between 0 and 255, the array represents the flowids used in the SXS communication protocol between the PC and the board. All incoming messages using these flowids will be redirected on the bypass handle. They can be checked with CHST_GetBypassPacket, and you can send these through the handle by using CHST_SendBypassPacket.",false)

addHelpEntry("CoolHost","CHST_Close","handle","","Closes the handle 'handle', opened either with CHST_Open, CHST_JadeOpen, or CHST_BypassOpen.",false)

addHelpEntry("CoolHost","CHST_GetBypassPacket","handle","[flowid,[bytes]]","Gets the next packet incoming on this handle. The handle must have been open with CHST_BypassOpen. Returns [flowid,[bytes]] containing the content of the packet. Header, flowid and CRC are removed from [bytes], that only contains the content data of the packet.",false)

addHelpEntry("CoolHost","CHST_SendBypassPacket","handle,flowID, byte_array","","Sends a packet on this handle. The handle must have been open with CHST_BypassOpen. byte_array is the content of the packet. The SXS header, flowid, and CRC will be added automatically.",false)

addHelpEntry("CoolHost","CHST_Read","handle, address","value","Reads a 32-bit unsigned word on 'handle', at address 'address'.",false)
addHelpEntry("CoolHost","CHST_Read32","handle, address","value","Reads a 32-bit unsigned word on 'handle', at address 'address'.",false)
addHelpEntry("CoolHost","CHST_Read16","handle, address","value","Reads a 16-bit unsigned word on 'handle', at address 'address'.",false)
addHelpEntry("CoolHost","CHST_Read8","handle, address","value","Reads a 8-bit unsigned word on 'handle', at address 'address'.",false)

addHelpEntry("CoolHost","CHST_Write","handle, address, val","","Writes the 32-bit unsigned word 'val' on 'handle', at address 'address'.",false)
addHelpEntry("CoolHost","CHST_Write32","handle, address, val","","Writes the 32-bit unsigned word 'val' on 'handle', at address 'address'.",false)
addHelpEntry("CoolHost","CHST_Write16","handle, address, val","","Writes the 16-bit unsigned word 'val' on 'handle', at address 'address'.",false)
addHelpEntry("CoolHost","CHST_Write8","handle, address, val","","Writes the 8-bit unsigned word 'val' on 'handle', at address 'address'.",false)

addHelpEntry("CoolHost","CHST_ReadInternal","handle, address","value","Reads a 8-bit unsigned word on 'handle', at internal address 'address'.",false)
addHelpEntry("CoolHost","CHST_WriteInternal","handle, address, val","","Writes the 8-bit unsigned word 'val' on 'handle', at internal address 'address'.",false)

addHelpEntry("CoolHost","CHST_ReadBlock","handle, address, bytes_to_read","[array_of_bytes]","Reads a block of data on COM handle 'handle', at address 'address'. The block of data is a sequence of 'bytes_to_read' bytes. Returns this sequence as an array of bytes.",false)
addHelpEntry("CoolHost","CHST_WriteBlock","handle, address,[array_of_bytes]","","Writes a block of data on COM handle 'handle', at address 'address'. The block of data is a sequence of bytes. It must be given as an array of bytes, 'array_of_bytes'.",false)

addHelpEntry("CoolHost","CHST_EnableEvents","handle, enable_bool","","Enables events on the handle 'handle' if 'enable_bool' is true, or disables them otherwise. When events are enabled, the associated CoolHost will keep for this handle a dedicated FIFO of all arriving events. This FIFO can be checked by 'CHST_PopEvent' and flushed by 'CHST_FlushEvents'.",false)
addHelpEntry("CoolHost","CHST_PopEvent","handle","value","Pops an event from the CoolHost event FIFO associated to this handle. The function raises a 'NoEventError' if no event could be popped, or returns the event number otherwise.",false)
addHelpEntry("CoolHost","CHST_FlushEvents","handle","","Flushes the CoolHost event FIFO associated to this handle.",false)

addHelpEntry("CoolHost","CHST_LoadLod","handle, filename_str, verify_bool","bool_success","Loads the lod file 'filename_str' to memory using COM handle 'handle'. If 'verify_bool' is true, a check is made afterwards to check if the content of the written memory matches the content of the lod file.",false)
addHelpEntry("CoolHost","CHST_VerifyLod","handle, filename_str","bool_success","Veryfies if the content of the memory matches the content of the lod file 'filename_str', using the COM handle 'handle'.",false)
addHelpEntry("CoolHost","CHST_DumpLod","handle, filename_str, address, nbwords, lodorbin_bool","","Dumps 'nbwords' from the memory, starting at address 'address' into file 'filename_str' and using COM handle 'handle'. If 'lodorbin_bool' is true, the format used is lod, otherwise, the dump is binary.",false)

#addHelpEntry("CoolHost","CHST_RamRun","handle, filename_str","Ramruns file 'filename_str'.")
#addHelpEntry("CoolHost","CHST_FastPf","handle, ramrunfilename_str, lodfilename_str","Fastpfs 'lodfilename_str' using prerequisited load code 'ramrunfilename_str'.")

addHelpEntry("CoolHost","CHST_RestartChip","handle, cpufreeze_bool, powerstayon_bool,reenableuart_bool","","Restarts the chip. If 'cpufreeze_bool' is true, it is frozen until a call to 'unfreeze'. If 'powerstayon_bool' is true, the power is kept on during the restart. If 'bool_reEnableUart' is true, the Uart will be reenabled after restart.",false)
addHelpEntry("CoolHost","CHST_XcpuUnfreeze","handle","","Unfreezes the chip.",false)
addHelpEntry("CoolHost","CHST_XcpuForceBreakpoint","handle","","XCPU Force Breakpoint.",false)
addHelpEntry("CoolHost","CHST_XcpuReleaseBreakPoint","handle","","XCPU Release Breakpoint.",false)
addHelpEntry("CoolHost","CHST_XcpuCheckBreakpoint","handle","bool","XCPU Check Breakpoint.",false)
addHelpEntry("CoolHost","CHST_BcpuForceBreakpoint","handle","","BCPU Force Breakpoint.",false)
addHelpEntry("CoolHost","CHST_BcpuReleaseBreakPoint","handle","","BCPU Release Breakpoint.",false)
addHelpEntry("CoolHost","CHST_BcpuCheckBreakpoint","handle","bool","BCPU Check Breakpoint.",false)

addHelpEntry("CoolHost","CHST_LaunchTargetAutoBauding","handle","bool","Launches auto bauding on CoolHost connected to handle.")

addHelpEntry("CoolHost","CHST_IsCoolhostConnected?","handle","bool","Returns true if Coolhost is connected to the communication device, false otherwise",false)

addHelpEntry("CoolHost","CHST_CleanCoolhost","handle","","Cleans Coolhost output.",false)

addHelpEntry("CoolHost","CHST_KillCoolhost","handle","","Kills Coolhost.",false)



def CHST_UpdateIncomingDataAndRescheduleRuby(handle)
    CHST_UpdateIncomingData(handle)
    sleep(0)
end

addHelpEntry("CoolHost","CHST_TimeOutPopEvent","handle,timeout_s","event","Pop an event from the event queue, or wait for one until 'timeout' (s) is reached. Raises NoEventError if no events could be popped.")
def CHST_TimeOutPopEvent(handle,timeout)
    if(timeout == 0)
        CHST_UpdateIncomingDataAndRescheduleRuby(handle)
        return CHST_PopEvent(handle)
    end
    
    Timeout::timeout(timeout) {
        while(true)
            # Trick: go into one-shot mode if timeout==0
            raise Timeout::Error,"execution expired" if(timeout<0) 
            timeout = -1         if(timeout==0)        
            begin
                CHST_UpdateIncomingDataAndRescheduleRuby(handle)
	            event = CHST_PopEvent(handle)
	            return event
	        rescue NoEventError
	        end
	        sleep(0.001)
	    end
    }
end

addHelpEntry("CoolHost","CHST_TimeoutGetBypassPacket","handle,flowid,timeout","[array_of_bytes]","Gets the next packet incoming on this handle. The handle must have been open with CHST_BypassOpen. Returns an array of bytes containing the content of the packet. If timeout is reached (No bypass packet could be get), raises a Timeout::error.")
def CHST_TimeoutGetBypassPacket(handle,flowid,timeout=2.0)
    Timeout::timeout(timeout) {
        while(true)
            # Trick: go into one-shot mode if timeout==0
            raise Timeout::Error,"execution expired" if(timeout<0) 
            timeout = -1         if(timeout==0)        
            begin
	          	return CHST_GetBypassPacket(handle,flowid)
	      	rescue NoBypassPacket
	        end
	        sleep(0.001)
	    end
    }
end



class CoolHostUtils
    #DON'T CHANGE THIS VALUE, IT'S DEFINED IN COOLHOST_LIBRARY
    COOLHOST_READ_RESULT_SLOT_COUNT         = 17
    WRITE_BLOCK_CHUNK_SIZE                  = 0x1000
    WRITE_BLOCK_CHUNK_SIZE_USB              = 0xA000    
    COOLHOST_USB_READBLOCK_PACKET_MAX_SIZE  = 1000
    MASK0	                                = 0x000000FF
    MASK1	                                = 0x0000FF00
    MASK23	                                = 0xFFFF0000
    SMASK0	                                = 0x00FF
    SMASK1	                                = 0xFF00
    #END OF DON'T TOUCH THIS

    COOLHOST_OPERATIONS_TIMEOUT             = 3.0

private
    def self.freeSlots(rr,ww)
        return(COOLHOST_READ_RESULT_SLOT_COUNT - (ww-rr) - 1)  if(ww - rr >= 0)
        return (rr-ww-1)
    end
    
    def self.coolhostOperationTimeout(exceptionStr)

        begin
            Timeout::timeout(COOLHOST_OPERATIONS_TIMEOUT) {
                yield()
            }
        rescue Timeout::Error
            raise CoolHostTimeoutError,exceptionStr
        end

    end 
  
    def self.fullSlots(rr,ww)
        return(ww - rr) if((ww - rr)>=0)
        return(COOLHOST_READ_RESULT_SLOT_COUNT - (ww - rr))
    end
    
    def self.countBytesNotToReadAtEndOfMask(mask)
        return 0 if(!mask)
        
        count = 0
        mask.reverse_each() { |i|
            if(i == 0)
                count += 1
            else
                return count
            end
        }
        return count
    end
    
    def self.countBytesToReadFromOffest(mask,offset,alignment)
        case alignment
            when 0 :
                # The result will be 4,2,1, or 0 (3 is excluded, because we don't want to make a read 3)
                return 4    if(!mask) # Read the max possible
                totest = mask[offset..[offset+3,mask.size()-1].min].from8to32bits()[0]
                if( (totest&MASK0)== MASK0)
                    if( (totest&MASK1) == MASK1)
                        return 4 if( (totest& MASK23) == MASK23)    
                        return 2
                    end
                    return 1
                end   
                return 0
            when 1,3 :
                # It will be 1 or 0 (cannot do anything else with that alignment)
                return 1 if(!mask) # Read the max possible   
                totest = mask[offset]
                return 1 if( (totest & 0xFF) == 0xFF)
                return 0
            when 2 :
                # The result will be 2,1, or 0 (cannot do anything else with that alignment)
                return 2 if(!mask) # Read the max possible  
                totest = mask[offset..[offset+1,mask.size()-1].min].from8to16bits()[0]
                if( (totest & SMASK0) == SMASK0) 
                    return 2 if( (totest & SMASK1) == SMASK1)
                    return 1
                end
                return 0
        end
    end
    
    def self.countBytesToReadFromOffestUSB(mask,offset)
        return COOLHOST_USB_READBLOCK_PACKET_MAX_SIZE if(!mask)
        count = 0
        (offset..mask.size()-1).each{ |i|
            if(mask[i]!=0)
                count += 1
                # Don't allow requests with a size bigger than the USB packet size
                return COOLHOST_USB_READBLOCK_PACKET_MAX_SIZE if(count >= COOLHOST_USB_READBLOCK_PACKET_MAX_SIZE)
            else
                return count
            end
        }
        return count
    end    
    
    def self.finishedReading(datalen,writeOffset,bytesNotToRead)
        return(writeOffset + bytesNotToRead >= datalen)
    end

    def self.atomisticWrite(handle,address,value,size,internal=false)

        CHST_UpdateIncomingDataAndRescheduleRuby(handle) #Clean the TCP socket.
        CHST_ResetWriteResult(handle)
        case size
            when 1:
                if(internal)
                    CHST_WriteRequestInternal(handle,address,value)
                else
                    CHST_WriteRequest8(handle,address,value)
                end
            when 2:
                CHST_WriteRequest16(handle,address,value)
            when 4:
                CHST_WriteRequest32(handle,address,value)
            else
                raise "Internal stupid error."
        end
        
        coolhostOperationTimeout("Write operation timed out!") {
            while(true)
                break if(CHST_WriteResultArrived(handle))    
                CHST_UpdateIncomingDataAndRescheduleRuby(handle)
            end   
        }
        return size
    end
    
    def self.writeRealign(handle,address,data)
        align = (address%4)
        writtenBytes = 0
        case align
            when 0:
                case data.size
                    when 0:
                    when 1:
                        writtenBytes += atomisticWrite(handle,address,data[0],1)
                    when 2:
                        writtenBytes += atomisticWrite(handle,address,data[0..1].from8to16bits()[0],2)
                    when 3:
                        writtenBytes += atomisticWrite(handle,address,data[0..1].from8to16bits()[0],2)
                        writtenBytes += atomisticWrite(handle,address,data[2],1)
                end
            when 1:
                case data.size
                    when 0:
                    when 1:
                        writtenBytes += atomisticWrite(handle,address,data[0],1)
                    when 2:
                        writtenBytes += atomisticWrite(handle,address,data[0],1)
                        writtenBytes += atomisticWrite(handle,address+1,data[1],1)
                    else
                        writtenBytes += atomisticWrite(handle,address,data[0],1)
                        writtenBytes += atomisticWrite(handle,address+1,data[1..2].from8to16bits()[0],2)
                end
            when 2:
                case data.size
                    when 0:
                    when 1:                 
                        writtenBytes += atomisticWrite(handle,address,data[0],1)
                    else
                        writtenBytes += atomisticWrite(handle,address,data[0..1].from8to16bits()[0],2)
               end
            when 3:
                case data.size
                    when 0:
                    else
                        writtenBytes += atomisticWrite(handle,address,data[0],1)
                    end
        end  
        return align, writtenBytes     
    end
    
    def self.writeBlockGeneric(handle,address,data,chunksize)
        raise "INTERNAL USE ERROR" if(chunksize!=WRITE_BLOCK_CHUNK_SIZE && chunksize!=WRITE_BLOCK_CHUNK_SIZE_USB)
    
        CHST_UpdateIncomingDataAndRescheduleRuby(handle) #Clean the TCP socket.
        CHST_ResetWriteResult(handle)
        
        # Count the number of write8 we will have to do to realign on 32bits
        align, realigncount = self.writeRealign(handle,address,data)
        
        bigDataLen		= ((data.size - realigncount)/4)*4 #align on 32bits now
        bigDataAddress	= address + realigncount
        bigDataSent	= 0
 
        while( (bigDataLen - bigDataSent)>0 )
        
            #Split the data in packets with size 'chunksize'
            bigSizeToSendThisTurn = (chunksize>(bigDataLen - bigDataSent))?(bigDataLen - bigDataSent):(chunksize)
            startoffset = realigncount+bigDataSent
            endoffset   = startoffset + bigSizeToSendThisTurn - 1

            CHST_ResetWriteResult(handle)
            CHST_WriteBlockRequest(handle,bigDataAddress,data[startoffset..endoffset])
             
            coolhostOperationTimeout("Write operation timed out!") {
                while(true)
                    break if(CHST_WriteResultArrived(handle))    
                    CHST_UpdateIncomingDataAndRescheduleRuby(handle)
                end   
            }   
                
            bigDataAddress  += bigSizeToSendThisTurn
            bigDataSent     += bigSizeToSendThisTurn
        end

        dataLeftLen		= data.size - realigncount - bigDataLen
        dataLeftAddress	= address + realigncount + bigDataLen
      
        # Make the last writes and then it will be realigned
        self.writeRealign(handle,dataLeftAddress,data[realigncount+bigDataLen..realigncount+bigDataLen+dataLeftLen-1])
        return true
    end

    def self.readBlockMaskedUSB(handle,startaddress,datalen,mask=nil)
        if(mask && mask.size()!=datalen)
            raise "Mismatch between wanted data length and mask length!"
        end  

        if(startaddress+datalen>(0xFFFFFFFF+1))
            raise RangeError, "Trying to read outside of the memory!"
        end
 
        rr = 0
        ww = 0
        
        data            = Array.new(datalen,0)  
        memslots        = Array.new(COOLHOST_READ_RESULT_SLOT_COUNT)
        readsizeslots   = Array.new(COOLHOST_READ_RESULT_SLOT_COUNT)

        currTime    = Time.new
        lastReqTime = currTime

        addressToRead = 0
        readOffset = 0
        writeOffset = 0
        lastWriteOffset = 0
        bytesNotToRead = countBytesNotToReadAtEndOfMask(mask)
              
        CHST_UpdateIncomingDataAndRescheduleRuby(handle) # Make sure we eliminate old data
        CHST_ResetReadResults(handle)
         
        while(!finishedReading(datalen,writeOffset,bytesNotToRead))
            currTime = Time.new
            
            # Full the free slots, make the requests
            while( (freeSlots(rr,ww)!=0) && (readOffset<datalen))  
                 addressToRead = startaddress+readOffset  
                 bytesToRead            = [countBytesToReadFromOffestUSB(mask,readOffset),datalen-readOffset].min   
                 if(bytesToRead == 0)
                    #Advance
                    readOffset += 1
                    redo
                 end
                 memslots[ww]           = addressToRead	    # Remember the address readed
                 readsizeslots[ww]      = bytesToRead	    # Remember the size readed
                 ww+=1
                 ww%=COOLHOST_READ_RESULT_SLOT_COUNT
                 lastReqTime = currTime
                 CHST_ReadRequestUSB(handle,addressToRead,bytesToRead)
                 readOffset+=bytesToRead     
            end
            
             # Check the results in the client's map
            loop do
                # Wait for at least one slot to be in pipe
                break if(fullSlots(rr,ww) == 0)
                # Check if a result arrived
                arrived,address,slotdata = CHST_ReadResultArrived(handle,rr,readsizeslots[rr])
                break if(!arrived)
 
                # Check if the address of the received read is correct.
                if(memslots[rr] != address)
                    raise ReadAnswerNotValid, "Received read result not matching request. Maybe some data was lost."    
                end
                # Update lastWriteOffset
                lastWriteOffset = memslots[rr]-startaddress
                # Write the result
                data[lastWriteOffset..lastWriteOffset+readsizeslots[rr]-1] = slotdata
                # Update the end of all writes
                #puts "%d %d" % [writeOffset,lastWriteOffset+readsizeslots[rr]]
                if(writeOffset<lastWriteOffset+readsizeslots[rr])
                    writeOffset = lastWriteOffset+readsizeslots[rr]
                    yield( (writeOffset + bytesNotToRead)/ (1.0*datalen)) if(block_given?())
                end    
                rr+=1
                rr%=COOLHOST_READ_RESULT_SLOT_COUNT
            end

            CHST_UpdateIncomingDataAndRescheduleRuby(handle)
            
            if(currTime - lastReqTime>COOLHOST_OPERATIONS_TIMEOUT)
                raise CoolHostTimeoutError, "Time out during read block!"
            end
        end
        return data           
    end

    def self.readBlockMaskedHost(handle,startaddress,datalen,mask=nil)
    
        if(mask && mask.size()!=datalen)
            raise "Mismatch between wanted data length and mask length!"
        end
        
        if(startaddress+datalen>(0xFFFFFFFF+1))
            raise RangeError, "Trying to read outside of the memory!"
        end
        
        rr = 0
        ww = 0
        
        data            = Array.new(datalen,0)  
        memslots        = Array.new(COOLHOST_READ_RESULT_SLOT_COUNT)
        readsizeslots   = Array.new(COOLHOST_READ_RESULT_SLOT_COUNT)

        currTime    = Time.new
        lastReqTime = currTime

        addressToRead = 0
        readOffset = 0
        writeOffset = 0
        lastWriteOffset = 0
        bytesNotToRead = countBytesNotToReadAtEndOfMask(mask)

        CHST_UpdateIncomingDataAndRescheduleRuby(handle) # Make sure we eliminate old data
        CHST_ResetReadResults(handle)
         
        while(!finishedReading(datalen,writeOffset,bytesNotToRead))
            
            currTime = Time.new
            
            # Full the free slots, make the requests
            while( (freeSlots(rr,ww)!=0) && (readOffset<datalen))
                
                addressToRead = startaddress+readOffset
                # Switch provided the alignment
                case [countBytesToReadFromOffest(mask,readOffset,addressToRead%4),datalen-readOffset].min
                    when 0 :
                        readOffset += 1 # Advance and nothing more
                    when 1 :
                        memslots[ww] = addressToRead	# Remember the address readed
                        readsizeslots[ww]           = 1	# Remember the size readed
                        ww+=1
                        ww%=COOLHOST_READ_RESULT_SLOT_COUNT
                        lastReqTime = currTime
                        CHST_ReadRequest8(handle,addressToRead)
                        readOffset+=1
                    when 2,3 : # 3 should happen only due to the min macro, meaning only 3 bytes left are to read
                        memslots[ww]		= addressToRead	# Remember the address readed
                        readsizeslots[ww]           = 2		# Remember the size readed
                        ww+=1
                        ww%=COOLHOST_READ_RESULT_SLOT_COUNT
                        lastReqTime = currTime
                        CHST_ReadRequest16(handle,addressToRead)
                        readOffset+=2
                    when 4 :
                        memslots[ww]		= addressToRead	#Remember the address readed
                        readsizeslots[ww]           = 4		#Remember the size readed
                        ww+=1
                        ww%=COOLHOST_READ_RESULT_SLOT_COUNT
                        lastReqTime = currTime
                        CHST_ReadRequest32(handle,addressToRead)
                        readOffset+=4
                end
            end
       
            # Check the results in the client's map
            loop do
                # Wait for at least one slot to be in pipe
                break if(fullSlots(rr,ww) == 0)
                    
                # Check if a result arrived
                arrived,address,slotdata = CHST_ReadResultArrived(handle,rr,readsizeslots[rr])
                break if(!arrived)
                    
                lastWriteOffset = memslots[rr]-startaddress
    
                # Check if the address of the received read is correct.
                if(memslots[rr] != address)
                    raise ReadAnswerNotValid, "Received read result not matching request. Maybe some data was lost."    
                end
                
                # Write the result
                data[lastWriteOffset..lastWriteOffset+readsizeslots[rr]-1] = slotdata
                
                # Update the end of all writes
                if(writeOffset<lastWriteOffset+readsizeslots[rr])
                    writeOffset = lastWriteOffset+readsizeslots[rr]
                    yield( (writeOffset + bytesNotToRead) / (1.0*datalen)) if(block_given?())
                end
                
                rr+=1
                rr%=COOLHOST_READ_RESULT_SLOT_COUNT
            end

            CHST_UpdateIncomingDataAndRescheduleRuby(handle)
            
            if(currTime - lastReqTime>COOLHOST_OPERATIONS_TIMEOUT)
                raise CoolHostTimeoutError, "Time out during read block!"
            end
        end
        return data

    end #function

    def self.batchRead(handle,startaddress,datalen,isUsb=false)

        if(startaddress+datalen>(0xFFFFFFFF+1))
            raise RangeError, "Trying to read outside of the memory!"
        end

        i = 0
        while(datalen>>i != 0)
            i += 1
        end
        i -= 9 if(i>8)
        progReportThresh = 1<<i
        tmpProgCnt = 0

        rr = 0
        data = Array.new(datalen,0)  
        writeOffset = 0

        stepSize = 4
        remaining = 0
        if(isUsb)
            stepSize = COOLHOST_USB_READBLOCK_PACKET_MAX_SIZE
            remaining = datalen % COOLHOST_USB_READBLOCK_PACKET_MAX_SIZE
        end

        currTime    = Time.new
        lastRecvTime = currTime

        CHST_UpdateIncomingDataAndRescheduleRuby(handle) # Make sure we eliminate old data
        CHST_ResetReadResults(handle)
        CHST_BatchReadRequest(handle, startaddress, datalen)

        while(writeOffset < datalen)

            currTime = Time.new

            # Check the results in the client's map
            loop do
                if(remaining!=0 && writeOffset+remaining==datalen)
                    stepSize = remaining
                end
                # Check if a result arrived
                arrived,address,slotdata = CHST_ReadResultArrived(handle,rr,stepSize)
                break if(!arrived)

                # Check if the address of the received read is correct.
                if(startaddress+writeOffset != address)
                    puts "address=0x%08x, should-be 0x%08x" % [address, startaddress+writeOffset]
                    raise ReadAnswerNotValid, "Received read result not matching request. Maybe some data was lost."    
                end

                # Write the result
                data[writeOffset..writeOffset+(stepSize-1)] = slotdata

                # Update the end of all writes
                writeOffset += stepSize
                tmpProgCnt += stepSize
                if(writeOffset==datalen || tmpProgCnt>=progReportThresh)
                    tmpProgCnt = 0
                    yield( writeOffset / (1.0*datalen)) if(block_given?())
                end

                lastRecvTime = currTime

                rr+=1
                rr%=COOLHOST_READ_RESULT_SLOT_COUNT
            end

            CHST_UpdateIncomingDataAndRescheduleRuby(handle)

            if(currTime-lastRecvTime > COOLHOST_OPERATIONS_TIMEOUT)
                raise CoolHostTimeoutError, "Time out during batch read!"
            end
        end
        return data

    end #function

public

    def self.write32(handle,address,value)
        writeBlock(handle,address,[value].from32to8bits())
    end
    
    def self.write16(handle,address,value)
        writeBlock(handle,address,[value].from16to8bits())
    end
    
    def self.write8(handle,address,value)
        writeBlock(handle,address,[value])
    end
    
    def self.writeInternal(handle,address,value)
        atomisticWrite(handle,address,value,1,true)
    end

    def self.read32(handle,address)
        readBlockMaskedHost(handle,address,4).from8to32bits()[0]
    end
    
    def self.read16(handle,address)
        readBlockMaskedHost(handle,address,2).from8to16bits()[0]
    end

    def self.read8(handle,address)
        readBlockMaskedHost(handle,address,1)[0]
    end    
    
    def self.readInternal(handle,address)
        val = 0
        
        CHST_UpdateIncomingDataAndRescheduleRuby(handle) # Make sure we eliminate old data
        CHST_ResetReadResults(handle)
        CHST_ReadRequestInternal(handle,address)
        
        coolhostOperationTimeout("Timeout during read!") {
            while(true)
                arrived,address,slotdata = CHST_ReadResultArrived(handle,0,1)
                if(arrived)    
                    val = slotdata[0]
                    break
                end
                CHST_UpdateIncomingDataAndRescheduleRuby(handle)
            end   
        }  
        
        return val
    end    
    
    def self.writeBlock(handle,address,data,usbmode=false)
        self.writeBlockGeneric(handle,address,data,(usbmode)?(WRITE_BLOCK_CHUNK_SIZE_USB):(WRITE_BLOCK_CHUNK_SIZE) )
    end
    
    def self.readBlockMasked(handle,address,datalen,mask,usbmode=false)
        if(!usbmode)
            self.readBlockMaskedHost(handle,address,datalen,mask) {|f| yield(f) if(block_given?()) }
        else
            self.readBlockMaskedUSB(handle,address,datalen,mask)  {|f| yield(f) if(block_given?()) }       
        end  
    end
    
    def self.readBlock(handle,address,datalen,usbmode=false)
        self.readBlockMasked(handle,address,datalen,nil,usbmode) {|f| yield(f) if(block_given?()) }
    end
    
   
    

end #class

=begin
def rbnew(handle,address,datalen,mask=nil)
    CoolHostUtils.readBlockMasked(handle,address,datalen,mask)
end

def CHST_WriteBlockMT(handle,address,array)

    CHST_UpdateIncomingDataAndRescheduleRuby(handle)  # Make sure we eliminate old data
    chunksize = 0x1000 #TODO : optimize chunk size for USB
    offset = 0
    while(offset<array.size)
        datalen = (array.size - offset>chunksize)?(chunksize):(array.size - offset)
        CHST_ResetWriteResult()
        CHST_WriteBlockRequest(handle,address+offset,array[offset..offset+datalen-1])
        
        coolhostOperationTimeout("Timeout during write operation!") {
            while(true)
                break if(CHST_WriteResultArrived(handle))    
                CHST_UpdateIncomingDataAndRescheduleRuby(handle)
            end   
        }
        
        offset += datalen
        yield(offset) if(block_given?())
    end        

end

def CHST_ReadMT(handle,address)
    CoolHostUtils.read32(handle,address)
end
=end
