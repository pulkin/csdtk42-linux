require "CoolGDB/cwcoolgdb.rb"
require "CoolGDB/cwcoolgdbicons.rb"

module CoolGDB
    
begin
#    cwAddScriptButton("CoolGDBToolBar", "gdb(\"cgdb\")", CoolGDBIcons::CGDB)
    cwAddScriptButton("CoolGDBToolBar", "gdb(\"mips-elf-gdbtui\")", CoolGDBIcons::GDBTUI,"Launch GDB TUI")
    cwAddScriptButton("CoolGDBToolBar", "coolGDBChooseConfiguration()", CoolGDBIcons::CGDBCONF,"Choose a configuration for CoolGDB")
    
    cwAddMenuCommand("GDB", "Choose a configuration for CoolGDB","coolGDBChooseConfiguration()",0)
    cwAddMenuCommand("GDB", "", "", 0)    
    cwAddMenuCommand("GDB", "Launch Debugger C-GDB", "gdb(\"cgdb\")", 0)
    cwAddMenuCommand("GDB", "Relaunch C-GDB",        "gdb(\"cgdb\", false)", 0)
    cwAddMenuCommand("GDB", "", "", 0)    
    cwAddMenuCommand("GDB", "Launch Debugger GDB-Tui", "gdb(\"mips-elf-gdbtui\")", 0)
    cwAddMenuCommand("GDB", "Relaunch GDB-Tui",        "gdb(\"mips-elf-gdbtui\", false)", 0)
    cwAddMenuCommand("GDB", "", "", 0)
    cwAddMenuCommand("GDB", "Launch CoolGDB Engine For Remote",
                     "CoolGDB.launchCoolGdbForRemote", 0)
    
rescue #No CoolWatcher
end

end

def coolGDBChooseConfiguration()
    lastCoolGDBConfiguration  = cwGetProfileEntry("lastCoolGDBConfiguration", "./rbbase/common/plugins/CoolGDB/profiles")
    file = cwGetOpenFileName("Choose CoolGDB Configuration", lastCoolGDBConfiguration, "*.cgdb")
    if(file=="?")
        return
    end
    lastCoolGDBConfiguration = file
    cwSetProfileEntry("lastCoolGDBConfiguration", lastCoolGDBConfiguration)
    puts "html>CoolGDB configuration set to: <br><i>%s</i>" % lastCoolGDBConfiguration
end