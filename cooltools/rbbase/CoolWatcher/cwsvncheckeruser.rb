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
#  Default user coolwatcher initialisation script.
#
###############################################################################



def svnCheckerRoutine(script)

    puts "Hi, I am the SVN Checker."    
    
    lastSchedule = Time.now()
    #Round the time to last hour.
    lastSchedule -= lastSchedule.min()*60 + lastSchedule.sec()
    nextSchedule = lastSchedule 
    
    while(true)
    
        cwClear
       
        puts "We are currently %s " % Time.now().to_s()
        puts "The exterior temperature is 24 degrees C."
        puts "The SVN Checker is about to start a new fly. Please fasten seatbelts."
        puts "Loading script : %s" % script
        
        load script
        
        nextSchedule += 3600

        if(Time.now()>nextSchedule)
            
            while(Time.now()>nextSchedule)
                nextSchedule += 3600
            end
            
            puts "We already passed next schedule. Re scheduling to %s" % nextSchedule
            
        else
            puts "Next schedule is at %s." % nextSchedule
        end
    
        #Wait for the schedule to be reached        
        while(Time.now() < nextSchedule)
            sleep 1
        end

    end
end


# Set the environment provided the user and project given in the profile
require "setenv.rb"

softdir = cwGetProfileEntry("softDir","")
if(softdir=="")
    puts "html><font color=orange>Warning! No softDir key was present in your profile.</font>"
end
projname = cwGetProfileEntry("project","")
if(projname=="")
    puts "html><font color=orange>Warning! No project key was present in your profile.</font>"
end
username = cwGetProfileEntry("username","")

setenv(softdir,projname)

# Include the script lib for CoolWatcher.
require "CoolWatcher/coolwatcher.rb"
require "CoolWatcher/cwdefaultgui.rb"

puts "html><font color=darkgreen>[Project set to " + $PROJNAME + "]</font>"

# Setup the default GUI.
cwBuildSvnCheckerGui()


require "eventsniffer"
require "eventhandlers"

addEventSnifferCallBack(0x57, lambda { chipResetEventCallBack })
# Hard reset event.
addEventSnifferCallBack(0xFFFFFFFF, lambda { puts "html><font color=darkgreen>Detected hardware reset (0xffffffff).</font>" })
# Assert handler.
addEventSnifferCallBack(0x00a55e47,lambda {handleAssert() } )

puts "Welcome to the SVN checker!"

puts "Activating CoolTester."
activateCoolTester()

cwAddScriptButton('svnchecker', "svnCheckerRoutine(\"CoolTester/scenarii/svnChecker_modem2g.rb\")", CWIconLib::NUCLEARICON,"Launch SVNChecker on Modem2G")
cwAddScriptButton('svnchecker', "svnCheckerRoutine(\"CoolTester/scenarii/svnChecker_jade.rb\")", CWIconLib::NUCLEARICON,"Launch SVNChecker on Jade")

# Smart open of the COM port.
cwDefaultOpen()

# Now, the environment is ok, we add a few chip dependent commands.
require "breakpoint.rb"

