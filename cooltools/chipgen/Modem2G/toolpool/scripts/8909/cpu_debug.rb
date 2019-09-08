#!/usr/bin/env ruby
include CoolHost

require "ChipStd/base.rb"
require "chip_control.rb"


# TODO Do real diag functions usings xml defined regs
addHelpEntry("chip", "diag", "", "", "Returns the status of the system CPU.")
def diag()

    # Test if CPU running
    # if
    pc     = $XCPU_CORE.rf0_addr.R
    cause  = $XCPU_CORE.cp0_Cause.R
    status = $XCPU_CORE.cp0_Status.R
    epc    = $XCPU_CORE.cp0_EPC.R
    puts "\n XCPU:"
    puts "PC     = 0x%08X" % pc 
    puts "EPC    = 0x%08X" % epc
    puts "Status = 0x%08X" % status
    puts "Cause  = 0x%08X" % cause

    # if not
    # todo
end



addHelpEntry("chip", "diagb", "", "", "Returns the status of the BCPU.")
def diagb()

    # Test if CPU running
    # if
    pc     = $BCPU_CORE.rf0_addr.R
    cause  = $BCPU_CORE.cp0_Cause.R
    status = $BCPU_CORE.cp0_Status.R
    epc    = $BCPU_CORE.cp0_EPC.R
    puts "\n BCPU:"
    puts "PC     = 0x%08X" % pc 
    puts "EPC    = 0x%08X" % epc
    puts "Status = 0x%08X" % status
    puts "Cause  = 0x%08X" % cause

    # if not
    # todo
end
