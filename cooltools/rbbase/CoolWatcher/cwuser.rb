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
apconn = cwGetProfileEntry("apConnection","")
username = cwGetProfileEntry("username","")

setenv(softdir,projname,apconn)

# Include the script lib for CoolWatcher.
require "CoolWatcher/coolwatcher.rb"
require "CoolWatcher/cwdefaultgui.rb"

puts "html><font color=darkgreen>[Project set to " + $PROJNAME + "]</font>"

# Now, the environment is ok, we add a few chip dependent commands.
require "breakpoint.rb"
require "csmail.rb"

# Setup the default GUI.
cwBuildStdGui()

# Display a welcome message.
if(username!="")
    puts "html><font color=darkgreen>[Bienvenue " + username + " !]</font>"
end

# Smart open of the COM port.
cwDefaultOpen(projname.downcase()=="jade")

begin
    profscript = "%s.rb" % cwGetProfileEntry("name","")
    puts "Loading profile script : %s" % profscript
    require profscript
rescue LoadError
end


