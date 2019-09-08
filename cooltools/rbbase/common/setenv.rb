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


def setenv(softworkdir,project,apconnection)
    # Setup some environement variables.
    $PROJNAME                   = project
    $SOFT_WORKDIR               = softworkdir
    checkLoadPath($SOFT_WORKDIR)
    $TOOLPOOL                   = $SOFT_WORKDIR + "/toolpool"    
    checkLoadPath($TOOLPOOL)
    $PLUGINS_WORKDIR            = $TOOLPOOL + "/plugins"
    checkLoadPath($PLUGINS_WORKDIR)
    $SCRIPT_WORKDIR             = $TOOLPOOL + "/scripts"
    checkLoadPath($SCRIPT_WORKDIR)
    $SCRIPT_GENDIR              = $SCRIPT_WORKDIR + "/ChipStd"
    checkLoadPath($SCRIPT_GENDIR)
    $SCRIPT_PROJDIR             = $SCRIPT_WORKDIR + "/" + $PROJNAME.split[0]
    checkLoadPath($SCRIPT_PROJDIR)
    $COMMON_PLUGINS_WORKDIR     = "./rbbase/common/plugins"
    checkLoadPath($COMMON_PLUGINS_WORKDIR)

    if(apconnection.strip.casecmp("yes") == 0)
        $APCONNECTION           = true
    else
        $APCONNECTION           = false
    end

    # Add the path the working dir where the script under development are stored. 
    if(RUBY_PLATFORM.include?("linux"))
        #rubySo = `ldconfig -p | grep "libruby.so.1.8 (libc6)"`
        # Allow to use x86_64 ruby lib
        rubySo = `ldconfig -p | grep "libruby.so.1.8"`
        if(rubySo.empty?())
            $LOAD_PATH.unshift("./lib/ruby/1.8")
            $LOAD_PATH.unshift("./lib/ruby/1.8/i386-linux")
        end
    end

    $LOAD_PATH.unshift($TOOLPOOL)
    $LOAD_PATH.unshift($PLUGINS_WORKDIR)
    $LOAD_PATH.unshift($COMMON_PLUGINS_WORKDIR)
    $LOAD_PATH.unshift($SCRIPT_WORKDIR)  
    $LOAD_PATH.unshift($SCRIPT_GENDIR) 
    $LOAD_PATH.unshift($SCRIPT_PROJDIR)    
end

def checkLoadPath(path)
    if(!FileTest.directory?(path))
        raise "Invalid load path: #{path}"
    end
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

