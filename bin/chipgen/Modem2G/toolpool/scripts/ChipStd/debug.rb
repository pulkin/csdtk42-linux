###############################################################################
#                                                                            ##
#            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            ##
#                            All Rights Reserved                             ##
#                                                                            ##
#      This source code is the property of Coolsand Technologies and is      ##
#      confidential.  Any  modification, distribution,  reproduction or      ##
#      exploitation  of  any content of this file is totally forbidden,      ##
#      except  with the  written permission  of  Coolsand Technologies.      ##
#                                                                            ##
###############################################################################
#
#  $HeadURL$
#  $Author$
#  $Date$
#  $Revision$
#
###############################################################################
#
#  Functions dealing with the debug features of the Modem2G chips.
#
###############################################################################

require "netutils"


addHelpEntry("chip", "getDbgPortConfig", "", "",
    "Returns the configuration of the debug port.")
        
def getDbgPortConfig()
    puts "Debug Port configuration mode:"
    $DEBUG_PORT.Debug_Port_Mode.mode_Config.r;
    puts "Debug Port PXTS tag level enables:"
    puts "PXTS level for RESERVED_0  ( 0) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_0.R.to_s
    puts "PXTS level for BOOT        ( 1) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_1.R.to_s
    puts "PXTS level for HAL         ( 2) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_2.R.to_s
    puts "PXTS level for SX          ( 3) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_3.R.to_s
    puts "PXTS level for PAL         ( 4) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_4.R.to_s
    puts "PXTS level for EDRV        ( 5) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_5.R.to_s
    puts "PXTS level for SVC         ( 6) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_6.R.to_s
    puts "PXTS level for STACK       ( 7) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_7.R.to_s
    puts "PXTS level for CSW         ( 8) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_8.R.to_s
    puts "PXTS level for RESERVED_9  ( 9) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_9.R.to_s
    puts "PXTS level for RESERVED_10 (10) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_10.R.to_s
    puts "PXTS level for RESERVED_11 (11) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_11.R.to_s
    puts "PXTS level for BB_IRQ      (12) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_12.R.to_s
    puts "PXTS level for SPAL        (13) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_13.R.to_s
    puts "PXTS level for SPP         (14) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_14.R.to_s
    puts "PXTS level for SPC         (15) = " + $DEBUG_PORT.Pxts_Exl_Cfg.Enable_Pxts_Tag_15.R.to_s
end



addHelpEntry("chip", "calib", "", "",
    "Tells whether the phone has been calibrated.")

def calib()
    if ($map_table.>.calib_access.>.opInfo.>.method.enum.R == 0xCA11E700) then
        puts "This phone has not been calibrated!"
    else
        puts "This phone has a calibration. Calibrated using the method:"
        $map_table.>.calib_access.>.opInfo.>.method.enum.r
    end
end



addHelpEntry("chip", "date", "", "",
    "Display the time and date of the phone in a human readable format. Tell when the calendar has never been programmed.")

def date() 
    puts "Current time of the phone: %d:%02d:%02d" %[$CALENDAR.Calendar_CurVal_L.Hour.R, $CALENDAR.Calendar_CurVal_L.Min.R, $CALENDAR.Calendar_CurVal_L.Sec.R]
    puts "Calendar not programmed." if ($CALENDAR.Status.Calendar_Not_Prog.R == 1)
end



addHelpEntry("chip", "setAnAlarm", "inNSec", "",
    "Reset the calendar to zero, clear the current alarm status and program an alarm at inNSec seconds.")

def setAnAlarm(inNSec)
    $CALENDAR.Cmd.Alarm_Enable_Clr.clear
    $CALENDAR.AlarmVal_L.w 0
    $CALENDAR.Calendar_LoadVal_L.w 0
    $CALENDAR.AlarmVal_H.w 0
    $CALENDAR.Calendar_LoadVal_H.w 0
    $CALENDAR.AlarmVal_L.Sec.w inNSec
    $CALENDAR.Cmd.Alarm_Clr.clear
    $CALENDAR.Cmd.Calendar_Load.set
    $CALENDAR.Cmd.Alarm_Load.set
    $CALENDAR.Cmd.Alarm_Enable_Set.set
end



addHelpEntry("chip", "getAlarm", "", "",
    "Check if an alarm has been reached.")

def getAlarm()
    if ($CALENDAR.Status.Alarm_Irq_Cause.R == 0) then
        puts "No alarm reached."
        return false
    else
        puts "Alarm reached."
        return true
    end
end



addHelpEntry("chip", "lowpower", "", "",
    "Check whether the phone is in low power or not.")

def lowpower()
    puts "Getting the low power state of the phone..."
    result = "The phone is not in low power."
    
    # Check if the phone goes to 32kHz.
    # The minimum duration of a deep sleep period is 500ms, so we check every 250ms.
    # We keep on checking this for 3 seconds, because sometimes we stay out of the 
    # deep sleep for a long time, to output traces.
    12.times do
        if ($SYS_CTRL.Sel_Clock.Slow_sel_RF.R == 1) then
            result = "The phone is in low power."
            break
        end
        sleep(0.25)
    end

    puts result
end



addHelpEntry("chip", "checkForGreenstoneAhbMasterBug", "", "",
    "Tells whether the chip crashed because of an AHB Master bug.")

class GreenstoneAhbMasterNoBugException < Exception
end

def checkForGreenstoneAhbMasterBug()
    puts "Reading internal registers..."

    begin
        # After the bug appears, the host port may be in a locked state.
        # So, we ask CW to stop auto-polling and send a break to the Host Port.
        begin
            cwDisableAutoPoll
        rescue Exception
            # No need to stop the polling, we are not in CoolWatcher.
        end
        cbreak
		sleep 4

        # Check that the AHB sys master register value is stable.
        ahbSysMasterReg = $INT_REG_DBG_HOST.AHB_SYS_MASTER.R

        20.times do
            if ahbSysMasterReg != $INT_REG_DBG_HOST.AHB_SYS_MASTER.R
                raise GreenstoneAhbMasterNoBugException, "sys master register value not constant"
            end
        end

        # Check that at least two masters are requesting the AHB bus and none
        # of them is granted.
		if ($INT_REG_DBG_HOST.AHB_SYS_MASTER.SYS_IFC_HMBURSREQ.rl(ahbSysMasterReg) +
            $INT_REG_DBG_HOST.AHB_SYS_MASTER.SYS_DMA_HMBURSREQ.rl(ahbSysMasterReg) +
            $INT_REG_DBG_HOST.AHB_SYS_MASTER.SYS_AHB2AHB_HMBURSREQ.rl(ahbSysMasterReg) +
            $INT_REG_DBG_HOST.AHB_SYS_MASTER.XCPU_HMBRSREQ.rl(ahbSysMasterReg) < 2)
            raise GreenstoneAhbMasterNoBugException, "less than two master requests"
        end

        if ($INT_REG_DBG_HOST.AHB_SYS_MASTER.SYS_IFC_HMGRANT.rl(ahbSysMasterReg) +
            $INT_REG_DBG_HOST.AHB_SYS_MASTER.SYS_DMA_HMGRANT.rl(ahbSysMasterReg) +
            $INT_REG_DBG_HOST.AHB_SYS_MASTER.SYS_AHB2AHB_HMGRANT.rl(ahbSysMasterReg) +
            $INT_REG_DBG_HOST.AHB_SYS_MASTER.XCPU_HMGRANT.rl(ahbSysMasterReg) != 0)
            raise GreenstoneAhbMasterNoBugException, "the bus is granted"
        end

        # If all the conditions above are met, the AHB bug occured.
        logString = "AHB Bug"
        puts "html><font color=red>Greenstone AHB master bug detected!</font>"

    rescue GreenstoneAhbMasterNoBugException
        logString = "No AHB Bug"
        puts "html><font color=darkgreen>No AHB master bug detected (" + $! + ").</font>"
    end

    # Get some information on the platform and logging them.
    logString += " @ " + getLocalIp.to_s
    logString += "  - At: " + Time.now.to_s
	if (nil == chipLastResetTime)
	    logString += " - No reset detected"
	else
        logString += " - Last reset: " + chipLastResetTime.to_s
        logString += " - Uptime: " + (Time.now - chipLastResetTime).to_s
	end
    logString += " - AHB Sys Master Register: 0x%x" %ahbSysMasterReg.to_s
    logAhbBug logString
    puts logString
end


addHelpEntry("chip", "pagespy", "start_add, end_add_excluded, on_writes = 1, on_reads = 0", "",
    "Set a page spy (0) from the address start_add (included) to the address end_add_excluded (excluded). Use the command 'pagespycheck' to check if this page has been hit.")
def pagespy(start_add, end_add_excluded, on_writes = 1, on_reads = 0)
    $PAGE_SPY.disable.disable_page_0.clear
    $PAGE_SPY.page[0].start.start_address.w start_add
    $PAGE_SPY.page[0].start.detect_read.w on_reads
    $PAGE_SPY.page[0].start.detect_write.w on_writes
    $PAGE_SPY.page[0].end.w end_add_excluded
    $PAGE_SPY.enable.enable_page_0.set
end


addHelpEntry("chip", "pagespycheck", "", "",
    "Check if the page spy (0) set by the command 'pagespy' has been hit.")
def pagespycheck()
    master_name = ["NO_ACCESS", "IFC", "DMA", "SYS_XCPU", "AHB2AHB"]
    hit_read = ($PAGE_SPY.page[0].addr.hit_read.R == 1)
    hit_write = ($PAGE_SPY.page[0].addr.hit_write.R == 1)
 
    if ($PAGE_SPY.status.status_page_0.R == 1) then
        print "Master " + master_name[$PAGE_SPY.page[0].master.master.R] +
        " hit the page 0 at address " +
        "0x%x." % $PAGE_SPY.page[0].addr.hit_addr.R
        
        if (hit_write && hit_read)
            print "Page hit while reading and writing."
        elsif (hit_read)
            print "Page hit while reading."
        elsif (hit_write)
            print "Page hit while writing."
        end
    end
end


addHelpEntry("chip", "sxsIdleHookEnable", "enable = 1", "",
    "Enable or disable SXS idle hook functions to debug XCV, PMD, ABB and FM.")
def sxsIdleHookEnable(enable = 1)
    begin
        ptr = $map_table.>.sx_access.>.sxsDebugIdleHookEnablePtr.RU
    rescue
        puts "Failed to get SXS idle hook enable flag. Is the map file outdated?"
        return
    end

    if(isAddressInRam(ptr))
        w(ptr, enable)
        puts "SXS idle hook flag has been set to %d." % enable
    else
        puts "Incorrect pointer for SXS idle hook enable flag. Is the s/w outdated?"
    end
end


addHelpEntry("chip", "lpsForceNoSleep", "enable = 1", "",
    "Force the chip (HAL-LPS module) not to sleep.")
def lpsForceNoSleep(enable = 1)
    begin
        ptr = $map_table.>.hal_access.>.lpsForceNoSleepPtr.RU
    rescue
        puts "Failed to get LPS force no sleep flag. Is the map file outdated?"
        return
    end

    if(isAddressInRam(ptr))
        w(ptr, enable)
        puts "LPS force no sleep flag has been set to %d." % enable
    else
        puts "Incorrect pointer for LPS force no sleep flag."
    end
end


addHelpEntry("chip",
            "gdbSwitchContext",
            "id, isJob=0",
            "",
            "Switch the task/job context in GDB (for backtrace, etc)")
def gdbSwitchContext(id, isJob=0)
    ctxTypeName = (isJob==0)?"task":"job"

    begin
        ctxPtr = $map_table.>.hal_access.>.gdbSwitchCtxPtr
        ptrAddr = ctxPtr.RU
    rescue CoolHostTimeoutError
        puts "Timeout when reading gdbSwitchCtxPtr. Is the host connection broken?"
        return
    rescue NoMethodError
        puts "Failed to read gdbSwitchCtxPtr. Is the map file outdated?"
        return
    end

    if(!isAddressInRam(ptrAddr))
        puts "The value of gdbSwitchCtxPtr is incorrect. S/w version of the target might be too old."
        return
    end

    const = CH__hal_map_globals

    ctxPtr.>.id.w(id)
    ctxPtr.>.isJob.w(isJob)
    ctxPtr.>.cmd.w(const::HAL_GDB_SWITCH_CTX_CMD_START)

    time_out = 2
    result = const::HAL_GDB_SWITCH_CTX_CMD_START
	# Execute until time_out is reached (an exception is raised in that case)
	begin
		Timeout::timeout(time_out) {
			# Wait for the command to be executed
			while(result == const::HAL_GDB_SWITCH_CTX_CMD_START) do
				result = ctxPtr.>.cmd.RU
			end
		}
    rescue Timeout::Error
        puts "Error: GDB loop is not running."
        return
	end

    if(result == const::HAL_GDB_SWITCH_CTX_CMD_OK)
        puts "The GDB context is switched to %s %d." % [ ctxTypeName, id ]
    elsif(result == const::HAL_GDB_SWITCH_CTX_CMD_RESTORED)
        puts "The GDB context is restored."
    elsif(result == const::HAL_GDB_SWITCH_CTX_CMD_ERR)
        puts "Error: Not a valid %s ID: %d." % [ ctxTypeName, id ]
        return
    else
        puts "Error: Unknown response: 0x%X" % [result]
        return
    end

    puts "***NOTE***"
    puts "Please follow the steps below:"
    puts "html>(1) run <font color=red><b>disconnect</b></font> command in GDB console;"
    puts "html>(2) run <font color=red><b>target remote localhost:26331</b></font> command in GDB console."
    puts "Then you can dump the backtrace of the new %s." % [ ctxTypeName ]
end


addHelpEntry("chip", "gdbEnable", "enable = 1", "",
    "Enable GDB.")
def gdbEnable(enable = 1)
    version = $map_table.>.hal_version.>.number.RU
    if (version < 11)
        puts "Target software does NOT support GDB configuration. Please upgrade first."
        return
    end

    begin
        ptr = $map_table.>.hal_access.>.gdbEnabledPtr.RU
    rescue
        puts "Failed to get GDB enabled flag. Is the map file outdated?"
        return
    end

    if(ptr == 0)
        puts "GDB is mandatory in target software."
    elsif(isAddressInRam(ptr))
        w(ptr, enable)
        puts "GDB enabled flag has been set to %d." % enable
    else
        puts "Incorrect pointer for GDB enabled flag."
    end
end


