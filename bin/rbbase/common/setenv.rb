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
#  This script provides a function to set environment global variables provided
#    the user and the project names for which coolruby must be initialized. It 
#      also adds pathes in the $LOAD_PATH variable where ruby will crawl for 
#        scripts.
#
###############################################################################


def setenv(softworkdir,project,chipIsModem,apconnection)
	#puts "burp"
    # Setup some environement variables.
    $PROJNAME                   = project
    $SOFT_WORKDIR               = softworkdir
    $TOOLPOOL                   = $SOFT_WORKDIR + "/toolpool"    
    $PLUGINS_WORKDIR            = $TOOLPOOL + "/plugins"
    $SCRIPT_WORKDIR             = $TOOLPOOL + "/scripts"
    $SCRIPT_GENDIR              = $SCRIPT_WORKDIR + "/ChipStd"
    $SCRIPT_PROJDIR             = $SCRIPT_WORKDIR + "/" + $PROJNAME.split[0]
    $COMMON_PLUGINS_WORKDIR     = "./rbbase/common/plugins"

    if(chipIsModem.strip.casecmp("yes") == 0)
        $CHIP_IS_MODEM          = true
    else
        $CHIP_IS_MODEM          = false
    end

    if(apconnection.strip.casecmp("yes") == 0)
        $APCONNECTION           = true
    else
        $APCONNECTION           = false
    end

    # Add the path the working dir where the script under development are stored. 
    $LOAD_PATH.unshift("./rbbase/")
    $LOAD_PATH.unshift($TOOLPOOL)
    $LOAD_PATH.unshift($PLUGINS_WORKDIR)
    $LOAD_PATH.unshift($COMMON_PLUGINS_WORKDIR)
    $LOAD_PATH.unshift($SCRIPT_WORKDIR)  
    $LOAD_PATH.unshift($SCRIPT_GENDIR) 
    $LOAD_PATH.unshift($SCRIPT_PROJDIR)    
	#$LOAD_PATH.unshift($SCRIPT_WORKDIR + "/common")
end

# If the onlythese array is defined, only the plugins specified in it will be loaded.
# Else, all plugins are loaded
def loadPlugins(onlythese=nil)
    Dir.foreach($COMMON_PLUGINS_WORKDIR) { |entry| 
        next if(onlythese && !onlythese.index(entry))
        d = $COMMON_PLUGINS_WORKDIR + "/" + entry
        if FileTest.directory?(d)
            if(FileTest.exists?( d + "/plugin.rb"))
                puts "Loading common plugin %s..." % entry
                load(d + "/plugin.rb")
            end
        end
    }
    Dir.foreach($PLUGINS_WORKDIR) { |entry| 
        next if(onlythese && !onlythese.index(entry))
        d = $PLUGINS_WORKDIR + "/" + entry
        if FileTest.directory?(d)
            if(FileTest.exists?( d + "/plugin.rb"))
                puts "Loading plugin %s..." % entry
                load(d + "/plugin.rb")
            end
        end
    }
end

