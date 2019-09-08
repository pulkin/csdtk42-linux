require "sxs"

include SXS

class ProfilerControl
    DEFAULT_BUFFER_SIZE = 1024*512

    PROFILE_IN_RAM      = :inRam
    PROFILE_ON_TRACE    = :onTrace

    # Properties of the allocated buffer
    @@bufferAddress     = 0
    @@bufferSize        = DEFAULT_BUFFER_SIZE


    # setter
    def self.bufferSize=(size)
        @@bufferSize = size
    end
    def self.bufferAddress=(size)
        @@bufferAddress = size
    end

    # getter
    def self.bufferSize()
        return @@bufferSize
    end
    def self.bufferAddress()
        return @@bufferAddress
    end

    

end

def profilerControlReset()
    ProfilerControl::bufferAddress  = 0
    ProfilerControl::bufferSize     = 0
end

# Return true if code is not crashed, in which case 
# SX remote feature (sxs_execute, sxs_RB, ...) can be used
def codeIsNotCrashed(myCon)
    if (($xcpu_error_info.cpu_sp_context.read(myCon) == 0)     &&
        (($XCPU.cp0_Cause.read(myCon) & 0x7FFFFFFF) == 0)      &&
        ($INT_REG_DBG_HOST.CTRL_SET.Force_BP_XCPU.read(myCon) == 0)     &&
        ($SYS_CTRL.XCpu_Dbg_BKP.Stalled.read(myCon) == 0)
       )
        return true
    else
        return false
    end
end
    

# Allocate the Remote Buffer use to profile in RAM.
def profilerMallocRamBuffer(size=ProfilerControl::DEFAULT_BUFFER_SIZE, connection=$CURRENTCONNECTION)

    CRVERBOSE("Malloc Profiling Ram Buffer", 2)

    myCon           = CHEmptyConnection.new()
    sxsConnection   = CHEmptyConnection.new()

    if (ProfilerControl::bufferAddress != 0)
        # Buffer already allocated.
        raise MultiProfilerExceptionBufferAlreadyAllocated, "Ram buffer already allocated at:%0x8" % [ProfilerControl::bufferAddress]
    end

    # Duplicate connection.
    myCon = connection.copyConnection()
    myCon.open(false)

  	sxsConnection = CHBPConnection.new(myCon, [SXS_RPC_RMC,  SXS_RPC_RSP_RMC])
  	sxsConnection.open(false)
    
    if (codeIsNotCrashed(myCon))
      	sxsExecute(sxsConnection,
    				$map_table.>(myCon).hal_access.>(myCon).profileControl.mallocRamBuffer.read(myCon),
    				SXS_RPC_RETURN_VALUE,
    				[SXS_RPC_VALUE_PARAM,SXS_RPC_NO_PARAM,SXS_RPC_NO_PARAM,SXS_RPC_NO_PARAM],
    				[size].from32to8bits)
    else
        raise MultiProfilerExceptionSystemCrashed, "Embedded System Crashed, cannot profile it."
    end
    
    # Hypothesis: Malloc succeeded
    ProfilerControl::bufferAddress = sxsWaitExecuteResult(sxsConnection, 8.0)
    ProfilerControl::bufferSize = size if (ProfilerControl::bufferAddress() != 0)


ensure
    sxsConnection.close()
    myCon.close()
end


# Free the Remote Buffer use to profile in RAM.
def profilerFreeRamBuffer(connection=$CURRENTCONNECTION)

    CRVERBOSE("Malloc Profiling Ram Buffer", 2)

    myCon           = CHEmptyConnection.new()
    sxsConnection   = CHEmptyConnection.new()

    if (ProfilerControl::bufferAddress == 0)
        # Buffer already freed.
        raise MultiProfilerExceptionBufferAlreadyFreed, "Ram buffer already freed"
    end

    # Duplicate connection.
    myCon = connection.copyConnection()
    myCon.open(false)

  	sxsConnection = CHBPConnection.new(myCon, [SXS_RPC_RMC,  SXS_RPC_RSP_RMC])
  	sxsConnection.open(false)
    
    if (codeIsNotCrashed(myCon))
      	sxsExecute(sxsConnection,
    				$map_table.>(myCon).hal_access.>(myCon).profileControl.freeRamBuffer.read(myCon),
    				SXS_RPC_RETURN_VOID,
    				[SXS_RPC_NO_PARAM,SXS_RPC_NO_PARAM,SXS_RPC_NO_PARAM,SXS_RPC_NO_PARAM],
    				[])
    else
        raise MultiProfilerExceptionSystemCrashed, "Embedded System Crashed, cannot free the buffer. (Not that it matters anymore ...)"
    end


ensure
    profilerControlReset()
    sxsConnection.close()
    myCon.close()
end

# Enable/Disable the Profiling on Ram
def profilerEnableProfiling(enable, mode=ProfilerControl::PROFILE_IN_RAM, connection=$CURRENTCONNECTION)
    myCon           = CHEmptyConnection.new()

    # Duplicate connection.
    myCon = connection.copyConnection()
    myCon.open(false)

    # Build accessor
    profilerCtrlCfg = $map_table.>(myCon).hal_access.>(myCon).profileControl.config

    val = profilerCtrlCfg.read(myCon)
    val = profilerCtrlCfg.prepl(val)


    case mode
    when ProfilerControl::PROFILE_IN_RAM
        val = profilerCtrlCfg.Global_Enable_Ram.wl(val, (enable)?(1):(0))

    when ProfilerControl::PROFILE_ON_TRACE
        val = profilerCtrlCfg.Global_Enable_Trace.wl(val, (enable)?(1):(0))

    end

    # Clear status if start
    if (enable)
        $map_table.>(myCon).hal_access.>(myCon).profileControl.status.write(myCon, 0)
    end

    # Write register
    profilerCtrlCfg.write(myCon, val)

ensure
    myCon.close()

end

# Dumper function.
def profilerDumpRamBuffer(forceHwRead=false, connection=$CURRENTCONNECTION)

    CRVERBOSE("Dump Profiling Ram Buffer", 2)

    myCon           = CHEmptyConnection.new()
    sxsConnection   = CHEmptyConnection.new()

    # Duplicate connection.
    myCon = connection.copyConnection()
    myCon.open(false)

  	sxsConnection = CHBPConnection.new(connection, [SXS_READ_RMC, SXS_DUMP_RMC])
  	sxsConnection.open(false)

    # If we had wrapped, we need the whole file.
    if ($map_table.>(myCon).hal_access.>(myCon).profileControl.status.wrapped.read(myCon) == 1)
        size = ProfilerControl::bufferSize
    else
        writePointer = $map_table.>(myCon).hal_access.>(myCon).profileControl.writePointer.read(myCon)
        size = writePointer - ProfilerControl::bufferAddress
    end

    puts "html> Start dumping data ... (May take a while)"
    # FIXME sxsRB is broken, as any dump can pollute it.
    # Use direct version as long as the fix is not implemented.
    # return sxsRB(sxsConnection, ProfilerControl::bufferAddress, size).from8to32bits
    puts "Size #{size}, addr:%08x" % [ProfilerControl::bufferAddress]
    
    # Check is code is not crashed, and the CPU is not stalled.
    if (codeIsNotCrashed(myCon) && !forceHwRead)
        capturedData = (sxsRB(sxsConnection, ProfilerControl::bufferAddress, size)).from8to32bits
        puts "html> Dump done."
        return capturedData
    else
        puts "html> SX Dump failed. Fallback on hw."
        capturedData = (myCon.readBlock(ProfilerControl::bufferAddress, size){|p| mpSetProgress(p*100)} ).from8to32bits
        puts "html> Dump done."
        return capturedData
    end

ensure
    sxsConnection.close()
    myCon.close()
end


# Display the control structure in the register watcher
def profilerDisplayControl()
    controlPanel = "$map_table.>.hal_access.>.profileControl"

    # Check if already displayed
    inwin=cwGetRootWatches
    if (!(inwin.include?(controlPanel)))
        # Add to watch
        cwAddWatch(eval(controlPanel.gsub(/\.>/,".derefAt(4)")))
    end

    # Refresh
    cwRefreshWatches
end


# Gets the Profiler Control Status
def profilerGetStatus(connection=$CURRENTCONNECTION)
    hash = {}
    myCon           = CHEmptyConnection.new()

    # Duplicate connection.
    myCon = connection.copyConnection()
    myCon.open(false)

    # Build status.
    if ($map_table.>(myCon).hal_access.>(myCon).profileControl.status.wrapped.read(myCon) == 1)
        hash[:wrapped] = true;
        writePointer = $map_table.>(myCon).hal_access.>(myCon).profileControl.writePointer.read(myCon)
        # Index refers to a 32 bits array
        hash[:wrapIndex] = (writePointer - ProfilerControl::bufferAddress)/4
    else
        # No wrap
        hash[:wrapIndex] = 0
    end
    
    if ($map_table.>(myCon).hal_access.>(myCon).profileControl.status.overflowed.read(myCon) == 1)
        hash[:overflowed] = true
    end
    

    return hash
ensure
    myCon.close()
end



