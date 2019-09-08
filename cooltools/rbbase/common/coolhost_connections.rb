require 'coolhost.rb'
require 'thread.rb'

class CHConnection
    include Comparable
    attr_reader :comport, :baudrate, :flowmode
    # @handle  haspassed not readable. We don't want people to use handles outside of the connection classes.
    # Instead, connection objects should be used.

    @@showAllEvents = false
    READ_BLOCK_SIZE = 0x4000 # 16KB

protected
    @name
    @comport
    @handle
    @baudrate
    @flowmode

    # Private function to be overloaded by child connections.
    def popen()
        @handle = CHST_Open(@comport,@baudrate,@flowmode)
    end

public
   def CHConnection.showAllEvents(enable)
        @@showAllEvents = enable
   end

   def handle()
        return @handle
   end

   def initialize(p,b=BR_AUTOMATIC,x=CoolHostBase::FLOWMODE)
        @name = "Unamed"
        @comport = p
        @baudrate = b
        @flowmode = x
        @handle = -1
        register()
    end

    def register()
        $COOLHOSTCONNECTIONS[@handle] = self
    end

    def unregister()
        $COOLHOSTCONNECTIONS.delete(@handle)
    end

    def launchTargetAutoBauding()
        CHST_LaunchTargetAutoBauding(@handle)
    end

    def killCoolhost()
        CHST_KillCoolhost(@handle)
    end

    def comBreak()
        CHST_ComBreak(@handle)
    end

    def isCoolhostConnected()
        return CHST_IsCoolhostConnected?(@handle)
    end

    def cleanCoolhost()
        CHST_CleanCoolhost(@handle)
    end

    def flushCoolhostFifos()
        CHST_FlushCoolhostFifos(@handle)
    end

    def reconnectCoolhostToDevice()
        CHST_ForceCoolhostReconnection(@handle)
    end

    def changeCoolhostFlowMode(mode)
        @flowmode = mode
        CHST_ChangeFlowMode(@handle,mode)
    end

    def waitCoolhostConnected(wtime=4)
        begin
            timeout(wtime) {
                loop do
                    break if(CHST_IsCoolhostConnected?(@handle))
                    sleep(0.01)
                end
            }
        rescue Timeout::Error
            #CHST_KillCoolhost(@handle)
            raise ConnectionFailedError,"Coolhost could not connect properly to device!"
        end
    end

    def open(forceCoolhostReconnection=true)
        close() #will do unregister
        popen()
        register()
        reconnectCoolhostToDevice() if(forceCoolhostReconnection)
        sleep 0.1
        if(forceCoolhostReconnection)
            waitCoolhostConnected()
        else
            if(!isCoolhostConnected())
                raise ConnectionFailedError,"Coolhost is not connect properly to device!"
            end
        end
    end

    def reopen(forceCoolhostReconnection=true)
        open(forceCoolhostReconnection)
    end

    def open?()
        return CHST_IsHandleValid?(@handle)
    end

    def close
        begin
            # Wait for the commands sent on the TCP socket to be effectively
            # received by CoolHost before closing the connection.
            isCoolhostConnected()
        rescue Exception
        end

        unregister()
        begin
            CHST_Close(@handle)
        rescue Exception
        end
    end

    def setName(newname)
        @name = newname
    end

    def name
        return @name+" (COM%d)" % @comport
    end

    def implementsHostReset
        return false
    end

    def implementsUsb()
        return false
    end

    def <=>(other)
        return @handle <=> other.handle
    end

    def copyConnection()
        return CHConnection.new(@comport,@baudrate,@flowmode)
    end

    def read32(address)
        return CHST_Read32(@handle, address)
    end

    def read16(address)
        return CHST_Read16(@handle, address)
    end

    def read8(address)
        return CHST_Read8(@handle, address)
    end

    def readInternal(address)
        return CHST_ReadInternal(@handle, address)
    end

    def write32(address,value)
        return CHST_Write32(@handle, address, value)
    end

    def write16(address,value)
        return CHST_Write16(@handle, address, value)
    end

    def write8(address,value)
        return CHST_Write8(@handle, address, value)
    end

    def writeInternal(address,value)
        return CHST_WriteInternal(@handle, address, value)
    end

    def readBlock(address,sizeToRead)
        data = Array.new(sizeToRead, 0)
        pos = 0

        while (pos < sizeToRead)
            len = sizeToRead - pos
            len = READ_BLOCK_SIZE if (len > READ_BLOCK_SIZE)

            bytes = CHST_ReadBlock(@handle, address, len)
            address += len
            data[pos..pos+len-1] = bytes
            pos += len

            yield((pos) / (1.0*sizeToRead)) if(block_given?())
        end
        return data
    end

    def readBlockMasked(address,sizeToRead,mask)
        if (!mask)
            return readBlock(address, sizeToRead) {|f| yield(f) if(block_given?()) }
        end

        if (mask.size() != sizeToRead)
            raise "Mismatch between wanted data length and mask length!"
        end

        data = Array.new(sizeToRead, 0)
        pos = 0

        while (pos < sizeToRead)
            len = sizeToRead - pos
            len = READ_BLOCK_SIZE if (len > READ_BLOCK_SIZE)

            bytes = CHST_ReadBlockMasked(@handle, address, len, mask)
            address += len
            data[pos..pos+len-1] = bytes
            pos += len

            yield((pos) / (1.0*sizeToRead)) if(block_given?())
        end
        return data
    end

    def batchRead(address,sizeToRead)
        return readBlock(address, sizeToRead) {|f| yield(f) if(block_given?()) }
    end

    def dumpByLib(filename,address,nbWords,lod0bin1,isBatchRead)
        return CHST_DumpLod(@handle,filename,address,nbWords,lod0bin1,isBatchRead)
    end

    def writeBlock(address,data)
        return CHST_WriteBlock(@handle,address,data)
    end

    def readString(address,sizeToRead)
        #ReadBlock doest the protect
        return self.readBlock(address,sizeToRead).pack("c*").split("\000")[0]
    end

    def popEvent(timeout=0)
        evt = CHST_TimeOutPopEvent(@handle,timeout)
        if (@@showAllEvents)
            puts "- Received event: 0x%08x" % evt
        end
        return evt
    end

    def enableEvents(en)
        CHST_EnableEvents(@handle,en)
    end

    def flushEvents()
        CHST_FlushEvents(@handle)
    end

    def waitEvent(eventnumber,timeout)
        begin
            timeout(timeout) {
                loop do
                    # Trick: go into one-shot mode if timeout==0
                    raise Timeout::Error,"execution expired" if(timeout<0)
                    timeout = -1         if(timeout==0)
                    begin
                        #Wait for events
                        recvEvent = popEvent()
                        if( eventnumber == recvEvent )
                            return true
                        end
                        #Throw away unmatching events
                    rescue NoEventError
                    end
                    sleep(0.01)
                end
            }
        rescue Timeout::Error
            return false
        end
    end

    def waitMutlipleEvents(arrayofevents,timeout)
        begin
            timeout(timeout) {
                loop do
                    # Trick: go into one-shot mode if timeout==0
                    raise Timeout::Error,"execution expired" if(timeout<0)
                    timeout = -1         if(timeout==0)
                    begin
                        ev = popEvent()
                        #Wait for events
                        if( arrayofevents.include?(ev) )
                            return ev
                        end
                        #Throw away unmatching events
                    rescue NoEventError
                    end
                    sleep(0.001)
                end
            }
        rescue Timeout::Error
            return nil
        end
    end

    def empty?()
        return false
    end
end

class CHHostConnection < CHConnection
    def initialize(p,b=BR_AUTOMATIC,x=CoolHostBase::FLOWMODE)
        super(p,b,x)
    end

    def implementsHostReset
        return true
    end

    def copyConnection()
        return CHHostConnection.new(@comport,@baudrate,@flowmode)
    end
end

class CHUartConnection < CHConnection
    def initialize(p,b=BR_AUTOMATIC)
        super(p,b, CoolHost::FLOWMODE_HARDWARE)
    end

    def name()
        return super()+" (UART)"
    end
end

class CHEmptyConnection < CHConnection

protected
    def popen()
        raise "Trying to open an empty connection!"
    end

public
    def initialize()
        @name = "Unamed"
        @comport = -1
        @handle = -1
        @baudrate = -1
        @flowmode = CoolHostBase::FLOWMODE
    end

    def name()
        @name + " (EMPTY CONNECTION)"
    end

    def open?()
        return false
    end

    def reopen()
    end

    def close()
    end

    def copyConnection()
        return CHEmptyConnection.new()
    end

    def empty?()
        return true
    end
end

## CHUSBConnection : USB Connection
class CHUSBConnection < CHConnection
    OPEN_STABLE_TIME = 3

protected
    def popen()
        @handle = CHST_USBOpen(@comport)
    end

public
    def initialize()
        @name = "Unamed"
        @comport = 0
        @baudrate = BR_AUTOMATIC
        @flowmode = CoolHostBase::FLOWMODE
        @handle = -1
        register()
    end

    def name()
        return @name + " (USB)"
    end

    def copyConnection()
        return CHUSBConnection.new()
    end

    def implementsUsb()
        return true
    end

    def implementsHostReset
        return true
    end
end

## CHJConnection : Jade Connection
class CHJConnection < CHConnection

    def popen()
        @handle = CHST_JadeOpen(@comport,@baudrate)
    end

public
    def initialize(p,b=BR_AUTOMATIC)
        @name = "Unamed"
        @comport = p
        @baudrate = b
        @flowmode = CoolHostBase::FLOWMODE
        @handle = -1
        register()
    end

    def name()
        return super()+" (JADE)"
    end

    def copyConnection()
        return CHJConnection.new(@comport,@baudrate)
    end
end

class CHBPConnectionTimeout < StandardError
end

## CHBPConnection : Bypass Connection
class CHBPConnection < CHConnection


protected
    @flowid_array

    def popen()
        if(@isusb)
            @handle = CHST_USBBypassOpen(@comport,@flowid_array)
        else
            @handle = CHST_BypassOpen(@comport,@baudrate,@flowmode,@flowid_array)
        end
    end

public
    def initialize(aconnection,flowid_array)
        @name = "Unamed"
        @flowid_array = flowid_array
        @isusb = aconnection.implementsUsb()
        @handle = -1
        if(@isusb)
            @comport = 0
            @baudrate = BR_AUTOMATIC
            @flowmode = CoolHostBase::FLOWMODE
        else
            @comport = aconnection.comport
            @baudrate = aconnection.baudrate
            @flowmode = aconnection.flowmode
        end
        register()
    end

    def name()
        if(@isusb)
            return @name + " (USB) (BYPASS)"
        else
            return super()+" (BYPASS)"
        end
    end

    def copyConnection()
        return CHBPConnection.new(self,@flowid_array)
    end

    # This method will trash all packets until it gets the flow id it wants
    def waitPacketForFlowId(flowid,timeout=2.0)
         Timeout::timeout(timeout) {
            # Trick: go into one-shot mode if timeout==0
            raise Timeout::Error,"execution expired" if(timeout<0)
            timeout = -1         if(timeout==0)
            while(true)
                begin
                    a = getPacket(timeout)
                    retry if(flowid!=a[0])
                    return a
                end
            end
        }
    rescue Timeout::Error
        raise CHBPConnectionTimeout,"execution expired"
    end

    def getPacket(timeout=2.0)
        Timeout::timeout(timeout) {
            while(true)
                # Trick: go into one-shot mode if timeout==0
                raise Timeout::Error,"execution expired" if(timeout<0)
                timeout = -1         if(timeout==0)
                begin
                    return CHST_GetBypassPacket(@handle)
                rescue NoBypassPacket
                end
                sleep(0.01)
            end
        }
    rescue Timeout::Error
        raise CHBPConnectionTimeout,"execution expired"
    end

    def sendPacket(flowid,bytearray)
        CHST_SendBypassPacket(@handle,flowid,bytearray)
    end

    def flushPackets()
        CHST_FlushBypassPackets(@handle)
    end

end

class CHRawConnectionGetDataTimeout < StandardError
end

class CHRawConnection < CHConnection
protected
    # Private function to be overloaded by child connections.
    def popen()
        @handle = CHST_RawOpen(@comport,@baudrate,@flowmode)
    end

public
    def initialize(p,b=BR_AUTOMATIC)
        super(p,b,CoolHostBase::FLOWMODE)
    end

    def CHRawConnection.session(p,b=BR_AUTOMATIC)
        c = CHRawConnection.new(p,b)
        c.open(false)

        yield(c)

    ensure
        c.close
    end

    def name()
        return super()+" (RAW)"
    end

    def getData(timeout=2.0)
        begin
            Timeout::timeout(timeout) {
                while(true)
                    # Trick: go into one-shot mode if timeout==0
                    raise Timeout::Error,"execution expired" if(timeout<0)
                    timeout = -1         if(timeout==0)
                    begin
                        return CHST_GetRawData(@handle)
                    rescue NoRawData
                    end
                    sleep(0.01)
                end
            }
        rescue Timeout::Error
            raise CHRawConnectionGetDataTimeout, "No data received!"
        end
    end

    def sendData(bytearray)
        CHST_SendRawData(@handle,bytearray)
    end

    def flushData()
        CHST_FlushRawData(@handle)
    end
end

if(!$COOLHOSTCONNECTIONS)
    $COOLHOSTCONNECTIONS = {}
end

if(!$CURRENTCONNECTION)
    $CURRENTCONNECTION = CHEmptyConnection.new
end

if(!$TOOLCONNECTIONS)
    $TOOLCONNECTIONS = {}
end

class ToolConnection
    attr_reader :connection
    def initialize(name)
        @name = name
        @connection = CHEmptyConnection.new
        @connection.setName(@name)
    end

    # Standard recreate. Copy the current connection, reopen it
    def recreate()
        @connection.close()
        @connection = $CURRENTCONNECTION.copyConnection()
        @connection.setName(@name)
        begin
            @connection.open(false)
            okputs "Connection #{@connection.name} opened."
            return true
        rescue Exception => e
            errputs "Connection #{@connection.name} could not be opened.<br> Reason: #{e.message} (#{e.class})."
            return false
        end
    end
end

def registerNewToolConnection(toolconnection)
    $TOOLCONNECTIONS[toolconnection] = toolconnection
    return toolconnection
end
