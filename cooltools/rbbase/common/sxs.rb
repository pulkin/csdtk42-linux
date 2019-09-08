include CoolHost

module SXS
     
    SXS_SPY_RMC             = 0
    SXS_RMC_YIELD           = 0x80
    SXS_TRACE_RMC           = 0x80
    SXS_DUMP_RMC            = 0x81
    SXS_RPC_RSP_RMC         = 0x82
    SXS_TIME_STAMP_RMC      = 0x83
    SXS_READ_RMC            = 0x84
    SXS_WRITE_RMC           = 0x85
    SXS_TRACE_LEVEL_RMC     = 0x86
    SXS_RPC_RMC             = 0x87
    SXS_DWL_START_RMC       = 0x88
    SXS_DWL_RMC             = 0x89
    SXS_MBX_RMC             = 0x8A
    SXS_RAZ_RMC             = 0x8B
    SXS_INIT_RMC            = 0x8C
    SXS_SERIAL_RMC          = 0x8D
    SXS_WIN_RMC             = 0x8E
    SXS_MEM_RMC             = 0x8F
    SXS_AT_RMC              = 0x90
    SXS_DATA_RMC            = 0x91
    SXS_DATACTRL_RMC        = 0x92
    SXS_EXIT_RMC            = 0x93
    
    #Returns
    SXS_RPC_RETURN_VOID     = 0
    SXS_RPC_RETURN_POINTER  = (1 << 17)
    SXS_RPC_RETURN_VALUE    = (1 << 18)
    
    SXS_RPC_NO_PARAM        = 0xFF
    SXS_RPC_VALUE_PARAM     = 0
    
    # Generate unique Id for read request on SX.
    class SXSRBIdGen
        @@nextId = 0

        def self.generate()
            id = @@nextId
            @@nextId +=1
            @@nextId = 0 if (@@nextId>=256)
            return id
        end
    end

    def sxsGetNextPacket(connection,wantedFlowId,timeout=2)
        @@isTimeout = 0
        loop {
            begin
                begin
                    a = connection.waitPacketForFlowId(wantedFlowId,timeout)
                    @@isTimeout = 0
                    return a[1]        
                    rescue CHBPConnectionTimeout
                    if(@@isTimeout == 0)
                        puts "sxsGetNextPacket timeout"
                        @@isTimeout = 1
                        connection.sendPacket(SXS_EXIT_RMC,[])
                        sleep 0.01 
                        #Sleep when there ain't no packets  
                        redo    
                    else
                        raise "sxsGetNextPacket timeout"
                    end
                end
           end
            }
    end

    def sxsGetDataRMCAnswer(connection,verbose=false)
        a = sxsGetNextPacket(connection,SXS_DATA_RMC)
        # If we get here, no timeout occured
        msg = a.slice(12..-1).pack("c*")
        puts "[ " + msg + "]" if(verbose)
        return (msg == "OK")
    end    
    
    def sxsGetDumpPacket(connection, mode=:sxMode)
        p = sxsGetNextPacket(connection,SXS_DUMP_RMC)
        # We know this dump packet is for thus because the 
        # first byte of the format string is a space
        # Id is third byte of the format string,
        # ie 7th bytes of the packet (Four first are size)
        # (First index index being 0)
        # Always get the read when the protocol is for the uart monitor.
        if (p[4] == 0x20 || mode == :uartMode)
            id = p[6]
            return p.slice(12..-1), id
        else
            return [], nil
        end
    end
       
    def sxsWrite(connection,address,byte_array)
        size = byte_array.size()
        tosend = [(size & 0xFF),(size & 0xFF00)>>8,0x01,0x00] + [address].from32to8bits() + byte_array
        connection.sendPacket(SXS_WRITE_RMC,tosend)
    end
    
    def sxsReadRequest32(connection,address,size, readId=0)
        tosend = [(size & 0xFF),(size & 0xFF00)>>8,0x04,readId] + [address].from32to8bits()
        connection.sendPacket(SXS_READ_RMC,tosend)
    end
    
    # Special one used in the uart_ramrun and the uart monitor.
    def sxsReadblock(connection,address,size)
        
        raise "Size should be a multiple of 4." if(size%4 != 0)
        data = []
        psize = 0x400
        
        datadone    = 0
        tmplen      = 0
        
        while(datadone<size)
            tmplen = (size-datadone>psize)?psize:(size-datadone)
            sxsReadRequest32(connection, (address + datadone) | 0x80000000,tmplen)
            #SHOULD RECEIVE OK
            if(sxsGetDataRMCAnswer(connection))
                #THEN SHOULD RECEIVE DATA
                # We don't care about the Id for the Uart monitor.
                dataPacket, readId = sxsGetDumpPacket(connection, :uartMode)
                data += dataPacket
            end
            datadone += tmplen
        end
 
        return data
    end
    
    # To use this function, SXS_READ_RMC, SXS_DUMP_RMC should be bypassed.
    def sxsRB(connection,address,size)
        raise "Size should be a multiple of 4." if(size%4 != 0)
        data = []
        psize = 0x400
        
        datadone    = 0
        tmplen      = 0

        # Generate id
        readId = SXSRBIdGen::generate()
        
        while(datadone<size)
            tmplen = (size-datadone>psize)?psize:(size-datadone)
            sxsReadRequest32(connection, (address + datadone) | 0x80000000, tmplen, readId)
            packetReceived = false
            while (!packetReceived)
                dataPacket, packetId = sxsGetDumpPacket(connection)
                # Check this is our packet.
                if (packetId == readId)
                    data     += dataPacket
                    datadone += tmplen
                    packetReceived = true
                    yield(datadone*1.0/size)    if(block_given?())
                end
            end
        end
 
        return data    
    end
    
    def sxsExecute(connection,func_address,return_type,param_len_array,param_byte_array)
        #Make data array
        tosend = [func_address,return_type].from32to8bits() + [param_len_array[0],param_len_array[1],param_len_array[2],param_len_array[3]] + param_byte_array
        connection.sendPacket(SXS_RPC_RMC,tosend)
    end
    
    def sxsWaitExecuteResult(connection, timeout=2.0)
        a = connection.waitPacketForFlowId(SXS_RPC_RSP_RMC,timeout)
    
        # First 32 bits are 'id' (ie the required response type)
        rspType = a[1][0..3].from8to32bits[0]
        case rspType
        when SXS_RPC_RETURN_VALUE
        # Other is returned value.
            return a[1][4..-1].from8to32bits[0]
        else
            raise "TODO Implement the 'return pointer ' case, where the pointed stuff is dumped."
        end
        
    end


    def sxsExitmonitor(connection)
        connection.sendPacket(SXS_EXIT_RMC,[])
    end

end


addHelpEntry("chip",
            "sxsRun",
            "functionAddress, param0=0, param1=0, param2=0, param3=0, connection=$CURRENTCONNECTION",
            "",
            "Execute a function on the target (RPC) via SXS RMC, with 4 maximum number of parameters and no return value.")
def sxsRun(functionAddress, param0=0, param1=0, param2=0, param3=0, connection=$CURRENTCONNECTION)
    sxsConnection = CHBPConnection.new(connection, [SXS_RPC_RMC, SXS_RPC_RSP_RMC])
    sxsConnection.open(false)
    begin
        SXS.sxsExecute(sxsConnection,
                        functionAddress,
                        SXS_RPC_RETURN_VOID,
                        [SXS_RPC_VALUE_PARAM,SXS_RPC_VALUE_PARAM,SXS_RPC_VALUE_PARAM,SXS_RPC_VALUE_PARAM],
                        [param0, param1, param2, param3].from32to8bits)
    ensure
        sxsConnection.close()
    end
end


addHelpEntry("chip",
            "sxsRunRetVal",
            "functionAddress, param0=0, param1=0, param2=0, param3=0, connection=$CURRENTCONNECTION",
            "",
            "Execute a function on the target (RPC) via SXS RMC, with 4 maximum number of parameters and return value.")
def sxsRunRetVal(functionAddress, param0=0, param1=0, param2=0, param3=0, connection=$CURRENTCONNECTION)
    sxsConnection = CHBPConnection.new(connection, [SXS_RPC_RMC, SXS_RPC_RSP_RMC])
    sxsConnection.open(false)
    begin
        SXS.sxsExecute(sxsConnection,
                        functionAddress,
                        SXS_RPC_RETURN_VALUE,
                        [SXS_RPC_VALUE_PARAM,SXS_RPC_VALUE_PARAM,SXS_RPC_VALUE_PARAM,SXS_RPC_VALUE_PARAM],
                        [param0, param1, param2, param3].from32to8bits)
        value = SXS.sxsWaitExecuteResult(sxsConnection)
        puts "Return value: %d (0x%08x)" % [ value, value ]
    ensure
        sxsConnection.close()
    end
end

addHelpEntry("chip",
            "sxsTaskList",
            "connection=$CURRENTCONNECTION",
            "",
            "List all SX task name.")
def sxsTaskList(connect=$CURRENTCONNECTION)
   for n in 0...32
      begin
         puts "Task %d: %s" % [n, RS($map_table.>.sx_access.>.sxrTaskPtr.>.Ctx[n].Desc.>.Name.R, 32)]
      rescue
      end
   end
end