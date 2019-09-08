#Helper function to invoke coolgdb server.


require 'find'
require 'netutils'
require 'rbconfig'
require 'rexml/document'


class CoolGDBConfigLoader
    include REXML
    
    def cgdbstrtoul(param)
        return param.hex if(/0x/ =~ param)
        
        return param.to_i
    end
    
    def loadCoolGDBParametersFromXMLFile(file)
        doc = Document.new(File.new(file))
        h = {}
        doc.root.elements.each{ |e| 
            case e.name
                when "tcpserverport"
                    h[e.name] = cgdbstrtoul(e.text)
                when "eventnumber"
                    h[e.name] = cgdbstrtoul(e.text)
                when "spcontextaddress"
                    h[e.name] = cgdbstrtoul(e.text)
                when "debugirqregaddress"
                    h[e.name] = cgdbstrtoul(e.text)
                when "debugirqtext"
                    h[e.name] = cgdbstrtoul(e.text)
                when "debugirqvalue"
                    h[e.name] = cgdbstrtoul(e.text)
                when "chipname"
                    h[e.name] = e.text
                when "jadecoolhost"
                    h[e.name] = (e.text=="true")?true:false
                when "flowmode"
                    h[e.name] = cgdbstrtoul(e.text)
                else
                    h[e.name] = e.text
            end
        } 
        return h
    end
end


module CoolGDB
    
begin
    require 'win32ole'
    @@Win32 = true
    @@Mac = false
rescue Exception
    @@Win32 = false
    include Config
    @@Mac = (CONFIG["target_vendor"] == "apple")
    if (@@Mac)
        puts "CoolGDB: Mac OS X Version."
    else
        puts "CoolGDB: Linux Version."
    end
end

    def CoolGDB.isWin32()
        return @@Win32
    end

    def CoolGDB.launchCoolGdbForRemote
        puts "Launching CoolGDB..."
        if(coolgdb())
            puts "Type, in the directory of your test:"
            puts "html><font color=darkgreen>make gdb COOLGDB_IP=" +
                getLocalIp().to_s + "</font>"
        end
    end

    def CoolGDB.coolgdb()
        
 		configuration = ""
        # Get configuration from profile 
        lastCoolGDBConfiguration  = cwGetProfileEntry("lastCoolGDBConfiguration", "./rbbase/common/plugins/CoolGDB/profiles")
		if(lastCoolGDBConfiguration == "./rbbase/common/plugins/CoolGDB/profiles")
		    lastCoolGDBConfiguration = nil
	    end
        
        if(!lastCoolGDBConfiguration)
            chipname = cwGetProfileEntry("project","").downcase
            puts "*** No configuration chosen for CoolGDB!"
            puts "*** Please chose one with the dedicated button."
            return false
        end
        
        puts "html>Launching CoolGDB with configuration file :<br><br> <i>%s</i>." % lastCoolGDBConfiguration
        configuration = lastCoolGDBConfiguration
        
        if($CURRENTCONNECTION.implementsUsb)
            connectionArg = "USB"
        else
            connectionArg = "COM"+$CURRENTCONNECTION.comport().to_s
        end
        
        if(@@Win32)
            
            # Create an instance of the Windows Shell object...
            shell = WIN32OLE.new('Shell.Application')
            
            # The shell object's ShellExecute method performs a specified operation on a specified file. The syntax is...
            shell.ShellExecute("coolgdb.exe",connectionArg + " " + configuration,".",'open',2)
        
            return true
            
        elsif (@@Mac)
            Thread.new {
                arguments = connectionArg + " " + configuration
                puts "Launching" + " coolgdb " + arguments
                `../../../coolgdb.app/Contents/MacOS/coolgdb #{arguments}`
            }
            return true
            
        else
            Thread.new {
                arguments = connectionArg + " " + configuration
                puts "Launching" + " coolgdb " + arguments
                `./coolgdb #{arguments}`
            }
            return true
        end
    end
    
    def CoolGDB.generateCoolGDBInit(path)
        tcpport = nil
        begin
            lastCoolGDBConfiguration = cwGetProfileEntry("lastCoolGDBConfiguration","")
            cloader = CoolGDBConfigLoader.new
            params = cloader.loadCoolGDBParametersFromXMLFile(lastCoolGDBConfiguration)
            tcpport = params["tcpserverport"]
        rescue Exception => e
            puts "Since no valid configuration could be open, using standard TCP port for CoolGDB server."
            tcpport = 0x66DB
        end
        
        File.open(path+"/coolgdbinit","wb") { |f|
            f << "target remote localhost:%d" % tcpport
        }
        puts "html>Generating :<br><br><i>#{path}/coolgdbinit</i> and telling GDB to find CoolGDB on TCP port: %d." % tcpport
    end
    
    def CoolGDB.launchDebugger(debugger,pathToCgdb,elfToDebug)    
        if(@@Win32)
            #Generate coolgdbinit
            CoolGDB.generateCoolGDBInit(pathToCgdb)
            
            elfToDebug = "\""+elfToDebug+"\""
            #puts elfToDebug
            if(debugger == "cgdb")
                args = "-d %s/mips-elf-gdb.exe -x %s/coolgdbinit %s" % [pathToCgdb,pathToCgdb,elfToDebug]
            else
                args = "-x %s/coolgdbinit %s" % [pathToCgdb,elfToDebug]
            end
            
            # Create an instance of the Windows Shell object...
            shell = WIN32OLE.new('Shell.Application')
            
            # The shell object's ShellExecute method performs a specified operation on a specified file. The syntax is...
            shell.ShellExecute(pathToCgdb+"/"+debugger+".bat",args,pathToCgdb,'open',1)

        elsif (@@Mac)
            
            Thread.new {
                #Generate coolgdbinit
                CoolGDB.generateCoolGDBInit("/tmp")
                
                # Wait for CoolGDB to be launched
                sleep 2
                
                # Launch CGDB in command line
                `echo /Developer/CoolTools/bin/bin/mips-elf-gdbtui -x /tmp/coolgdbinit #{elfToDebug} > /tmp/cwgdblaunch.sh`
                #`echo "target remote localhost:26331" > /tmp/coolgdbinit`
                `chmod +x /tmp/cwgdblaunch.sh`
                `open -a Terminal /tmp/cwgdblaunch.sh`
                puts "Connecting GDB."
            }
            return true
            
        else
            #Generate coolgdbinit
            CoolGDB.generateCoolGDBInit(".")
                
            if(debugger == "cgdb")
                cmd = "cgdb -d /n/tools/gdb/bin/mips-elf-gdb -x ./coolgdbinit %s" % [elfToDebug]
            else
                cmd = "/n/tools/gdb/bin/mips-elf-gdbtui -x ./coolgdbinit %s" % [elfToDebug]
            end
        
            fork { exec("xterm -e " + cmd) }
            
        end
    end
    
    def CoolGDB.cgdb(pathToCgdb,elfToDebug)
        launchDebugger("cgdb",pathToCgdb,elfToDebug)
    end
    
    def CoolGDB.gdbtui(pathToCgdb,elfToDebug)
        launchDebugger("mips-elf-gdbtui",pathToCgdb,elfToDebug)
    end

end

def gdb(debugger="cgdb", launchcoolgdb=true)
    
    if(CoolGDB.isWin32())
    
        pathToGDB = cwGetProfileEntry("pathToGDB","")
        if(pathToGDB=="")
            puts "Oh, oh. The profile entry \"pathToGDB\" is not defined. GDB should be installed with CSDTK," \
                 "in PATHTOCSDTK/cygwin/coolgdb/bin.\n\n That's what we are going to check now:\n"
            
            cwSetProfileEntry("pathToGDB","")
            
            puts "The current dir is:"
            puts "   "+ Dir.pwd + "\n"
            
            pathToGDB = Dir.pwd + "/../cygwin/coolgdb/bin"
            puts "Searching for:"
            puts "   " + pathToGDB + "\n"
            
            if(Dir[pathToGDB]!=[])
                puts "Found! Setting your profile entry so that next time we won't have to make the check again."
                cwSetProfileEntry("pathToGDB",pathToGDB)
            else
                puts "Not found! Maybe you are using CoolWatcher in an other environment than the CSDTK. The pathToGDB" \
                     " profile entry of your profile is going to be set to \"\". Relaunch CoolWatcher and edit it to " \
                     "point to the good path if you really want to use the coolgdb system."
                return
            end
        end
        
        #Check if we can find mips-elf-gdb
        mipsElfGdbExe = pathToGDB + "/mips-elf-gdb.exe"
        if(!File.file?(mipsElfGdbExe))
            puts "The mips-elf-gdb.exe debugger could not be found in your pathToGDB!! Your profile entry \"pathToGDB\" " \
                 "is going to be resetted so that next time you launch the gdb command, it will try to redect it."
            cwSetProfileEntry("pathToGDB","")
            return
        end
        pathToGDB = File.expand_path(pathToGDB)
    else
        pathToGDB = ""
    end
    
    lodToDebug = cwGetProfileEntry("lastLodForDownload","")
    if(lodToDebug=="")
        puts "You have not burn any lod file with CoolWatcher. In order to be able to debug, the lod file should" \
             " be FLAMulated with the CoolWatcher GUI (use the FLAMulation buttons)."
        return
    end
    
    dirToCrawl = File.dirname(lodToDebug)
    elves = []
    Find.find(dirToCrawl) do |path|
        elves << path  if(path[-4..-1]==".elf")
    end
    
    if(elves.size == 0)
        puts "Sorry, no elf files found at your last FLAMulated lod file side, which is: "
        puts "   " + lodToDebug
        return 
    elsif(elves.size > 1)
        puts "*** WARNING !!!! %d elf files have been detected at your last FLAMulated lod file side. Debugging first one!" % elves.size
    end
    
    elfToDebug = elves[0]
    
    puts "Launching debugging system on:\n"
    puts "html><i>"+elfToDebug+"</i><br>"
    
    #Here we have a path for GDB. Launch the server.
    if(launchcoolgdb == true)
        if(!CoolGDB::coolgdb())
            return
        end
    end
    puts "html><br>"
    #Launch CGDB with the good parameters
    CoolGDB::launchDebugger(debugger,pathToGDB,elfToDebug)
    
end