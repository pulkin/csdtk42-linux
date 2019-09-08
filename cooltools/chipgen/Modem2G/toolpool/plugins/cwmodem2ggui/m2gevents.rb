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
#  Functions to add some event handlers for Modem2G.
#
###############################################################################

require "help"
require "eventsniffer"
require "eventhandlers"

$xfbpAtHardResetEnabled = false

addHelpEntry("chip", "xfbpreset", "", "",
    "Force XCPU breakpoint at hard reset boot up time (one-time only).")
def xfbpreset()
    $xfbpAtHardResetEnabled = true
end

addHelpEntry("chip", "noxfbpreset", "", "",
    "Cancel XCPU breakpoint at hard reset boot up time.")
def noxfbpreset()
    $xfbpAtHardResetEnabled = false
end

def chipHardResetEventCallBack
    if ($xfbpAtHardResetEnabled)
        xfbp()
        puts "html><font color=red>xfbp() has been run for this hard reset.</font>"
        $xfbpAtHardResetEnabled = false
    end        
    puts "html><font color=darkgreen>Detected hardware reset (0xffffffff).</font>"
end

addHelpEntry("CoolWatcher", "modem2GHandleEvents", "","", "Add some event handlers for Modem2G.")
    
def modem2GHandleEvents
    
    # Overwrite some event detection.
    # Reset event.
    addEventSnifferCallBack(0x57, lambda { chipResetEventCallBack })
    
    # Hard reset event.
    addEventSnifferCallBack(0xFFFFFFFF, lambda { chipHardResetEventCallBack })
    
    # Power off event (IaMdead).
    addEventSnifferCallBack(0x1a11dead, lambda { puts "html><font color=darkgreen>Detected shutdown request (0x1a11dead), should be off now.</font>" })

    # Various host monitor events.
    addEventSnifferCallBack(0xff000001, lambda { puts "html><font color=darkgreen>Detected host monitor enter (0xff000001).</font>" })
    addEventSnifferCallBack(0xff000002, lambda { puts "html><font color=darkgreen>Detected host monitor unsupported command (0xff000002).</font>" })
    addEventSnifferCallBack(0xff000003, lambda { puts "html><font color=darkgreen>Detected host monitor exit (0xff000003).</font>" })
    addEventSnifferCallBack(0xff000004, lambda { puts "html><font color=darkgreen>Detected host monitor command treatment start (0xff000004).</font>" })
    addEventSnifferCallBack(0xff000005, lambda { puts "html><font color=darkgreen>Detected host monitor command treatment end (0xff000005).</font>" })

    # Mismatch between chip ROM version and linked ROM version.
    addEventSnifferCallBack(0x00000054, lambda { puts "html><font color=darkgreen>Detected version mismatch between chip ROM and linked ROM (0x54).</font>" })
    
    # Entering the boot monitor via the boot sector.
    addEventSnifferCallBack(0x00000055, lambda { puts "html><font color=darkgreen>Detected boot sector reconfigure EBC CS RAM (0x55).</font>" })

    # Boot sector Entering the boot monitor via the boot sector.
    addEventSnifferCallBack(0x00000056, lambda { puts "html><font color=darkgreen>Detected boot sector enter boot monitor (0x56).</font>" })
    
    # GDB loop event.
    addEventSnifferCallBack(0x9db00000, lambda { puts "html><font color=darkgreen>Detected XCPU in the GDB loop (0x9db00000), connect GDB debugger for more info.</font>" })
    addEventSnifferCallBack(0x9db10000, lambda { puts "html><font color=darkgreen>Detected BCPU in the GDB loop (0x9db10000), connect GDB debugger for more info.</font>" })
    addEventSnifferCallBack(0x9db20000, lambda { puts "html><font color=darkgreen>Detected WCPU in the GDB loop (0x9db20000), connect GDB debugger for more info.</font>" })
    
    # NO GDB event.
    addEventSnifferCallBack(0x9db00010, lambda { puts "html><font color=darkgreen>Detected CPU fatal error (0x9db00010), it went through GDB stub and will reboot.</font>" })
    
    # Assert handler.
    addEventSnifferCallBack(0x00a55e47, lambda { handleAssert() })
end


modem2GHandleEvents()


