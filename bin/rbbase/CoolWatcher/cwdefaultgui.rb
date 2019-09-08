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
#  Functions to create a default GUI for CoolWatcher.
#
###############################################################################

require "CoolWatcher/coolwatcher"
require "CoolWatcher/cwiconlib.rb"
require "CoolWatcher/cwwatch.rb"
require "help"
require "eventsniffer.rb"
require "setenv"



#addHelpEntry("CoolWatcher","cwAddKillThreadButton","","","Add a button meant
#to kill the main command thread of CoolWatcher to CoolWatcher toolbar.")

def cwAddKillThreadButton
    cwAddScriptButton('cwdefault', "$CMDTHREAD.kill; &", CWIconLib::NUCLEARICON,"Kill CoolWatcher ruby command thread (attempts to stop current running command)")
end



#addHelpEntry("CoolWatcher","cwAddClearScreenButton","","","Add a button meant
#to clear CoolWatcher's screen to CoolWatcher toolbar.")

def cwAddClearScreenButton
    cwAddScriptButton('cwdefault', "cwClear &", CWIconLib::SWEEPICON,"Clear ruby script output")
end

def cwAddKillAllThreadButton
    cwAddScriptButton('cwdefault', "Thread.list.each { |t| t.kill } &", CWIconLib::NUCLEARICON,"Kill all ruby threads")
end



addHelpEntry("CoolWatcher", "cwBuildStdGui", "","", "Setup the standard 
        interface for CoolWatcher.")
    
def cwBuildStdGui
    # Standard GUI elements.
    cwShowGuiElement(DebugArea)
    cwShowGuiElement(ModuleLibrary)
    cwShowGuiElement(TypedefLibrary)
    cwShowGuiElement(DescriptionArea)
    cwShowGuiElement(WatchArea)
    cwShowGuiElement(CommandLine)
    #cwHideGuiElement(OutputArea)

    cwSetTitle "CoolWatcher Developer with profile " + cwGetProfileEntry("name", "Unknown Profile")

    errorpng = cwGetProfileEntry("errorpng","resources/error16.png")
    cwSetErrorPng(errorpng)

    # Standard menus.
    cwAddMenuCommand("File", "Open disassembly...", "openDisassembly()",0)
    cwAddMenuSeparator("File")
    cwAddMenuCommand("File", "Kill Command Thread", "$CMDTHREAD.kill; &", 0)
    cwAddMenuCommand("File", "Kill All Threads", "Thread.list.each { |t| t.kill }; &", 0)
    cwAddMenuCommand("File", "", "", 0)
    cwAddMenuCommand("File", "Quit", "cwQuit", 0)

    cwAddMenuCommand("View", "Refresh", "cwRefreshWatches", Key_F5)
    cwAddMenuCommand("View", "Clear Screen", "cwClear", 0)
    
    cwRegisterWatchToolbar()
        
    # Add "plugins" menu
    # Should be done in a dynamic way, taking into consideration plugins really
    # added to CoolWatcher directory.
    loadPlugins()
    
    # Setup default toolbar.
    cwAddClearScreenButton()
    cwAddKillThreadButton()
    
    begin
        cwRestoreWindowState("profiles/"+cwGetProfileEntry("name","unknown")+".cws")
    rescue Exception
    end
    
    begin
        cwRestoreHistory("profiles/"+cwGetProfileEntry("name","unknown")+".cwh")
    rescue Exception
    end
end

addHelpEntry("CoolWatcher", "cwBuildSvnCheckerGui", "","", "Setup the SVN Checker 
        interface for CoolWatcher.")
    
def cwBuildSvnCheckerGui
    # Standard GUI elements.
    cwShowGuiElement(DebugArea)
    cwShowGuiElement(ModuleLibrary)
    cwShowGuiElement(TypedefLibrary)
    cwHideGuiElement(DescriptionArea)
    cwHideGuiElement(WatchArea)
    cwShowGuiElement(CommandLine)
    #cwHideGuiElement(OutputArea)

    cwSetTitle "SVN Checker with profile " + cwGetProfileEntry("name", "Unknown Profile")

    # Standard menus.
    cwAddMenuSeparator("File")
    cwAddMenuCommand("File", "Kill Command Thread", "$CMDTHREAD.kill; &", 0)
    cwAddMenuCommand("File", "Kill All Threads", "Thread.list.each { |t| t.kill }; &", 0)
    cwAddMenuCommand("File", "", "", 0)
    cwAddMenuCommand("File", "Quit", "cwQuit", 0)

    cwAddMenuCommand("View", "Clear Screen", "cwClear", 0)
    
    cwRegisterWatchToolbar()
    
    # Add "plugins" menu
    # Should be done in a dynamic way, taking into consideration plugins really
    # added to CoolWatcher directory.
    loadPlugins()
    
    # Setup default toolbar.
    cwAddClearScreenButton()
    cwAddKillThreadButton()
    
    begin
        cwRestoreWindowState("profiles/"+cwGetProfileEntry("name","unknown")+".cws")
    rescue Exception
    end
end


def cwBuildMinimalGui
    # Standard GUI elements.
    cwHideGuiElement(DebugArea)
    cwHideGuiElement(ModuleLibrary)
    cwHideGuiElement(TypedefLibrary)
    cwHideGuiElement(DescriptionArea)
    cwHideGuiElement(WatchArea)
    cwShowGuiElement(CommandLine)
    #cwHideGuiElement(OutputArea)

    cwSetTitle "CoolWatcher Enterprise (" + cwGetProfileEntry("project", "Unknown Project") + ")"

    # Standard menus.
    cwAddMenuSeparator("File")
    cwAddMenuCommand("File", "Kill Command Thread", "$CMDTHREAD.kill; &", 0)
    cwAddMenuCommand("File", "Kill All Threads", "Thread.list.each { |t| t.kill }; &", 0)
    cwAddMenuCommand("File", "", "", 0)
    cwAddMenuCommand("File", "Quit", "cwQuit", 0)

    cwAddMenuCommand("View", "Clear Screen", "cwClear", 0)
  
    cwRegisterWatchToolbar()
    
    # Add "plugins" menu
    # Should be done in a dynamic way, taking into consideration plugins really
    # added to CoolWatcher directory.
    
    loadPlugins(["flamulator"])
    loadPlugins(["fastpf"])
    loadPlugins(["cwmodem2ggui"])
    loadPlugins(["CoolGDB"])
    loadPlugins(["tracesniffer"])
    loadPlugins(["disassembly"])
    
    # Setup default toolbar.
    cwAddClearScreenButton()
    cwAddKillThreadButton()
    begin
        cwRestoreWindowState("profiles/"+cwGetProfileEntry("name","unknown")+".cws")
    rescue Exception
    end
end


def askForComPort()
    # Ask which COM port to open.
    puts "html><font color=darkgreen>Com port to open? (enter number for host, or 'u' for usb).</font>"
    entry = gets    
    puts entry
      
    comport = checkComPortAndReaskIfBad(entry)
    return comport
end

def checkComPortAndReaskIfBad(comport)
    begin
        if(comport == "usb" || comport == "u")
            comport = "usb"
        else
            comport = comport.to_i   
        end
    rescue
        puts "Wrong COM port, try again."
        comport = askForComPort
    end    
       
    return comport
end

def openDisassembly()
    lastDisassemblyToOpen  = cwGetProfileEntry("lastDisassemblyToOpen","")
    file = cwGetOpenFileName("Choose DIS file...",lastDisassemblyToOpen,"*.dis")
    if(file=="?")
        return
    end
    lastDisassemblyToOpen = file
    cwSetProfileEntry("lastDisassemblyToOpen",lastDisassemblyToOpen)
    puts "html><table><tr><td bgcolor=#FFFFCC width=100%><font color=red>WARNING: Be aware that you are loading a disassembly without doing a flamulation or fastpf in CoolWatcher. Therefore, the code in your target and your disassembly file might not be synchronized.</font></td></tr></table>"
    puts "html>Opening disassembly file: <br><i>%s</i>" % lastDisassemblyToOpen
    cwLoadDisassembly(lastDisassemblyToOpen)
    puts "Done."
end

addHelpEntry("CoolWatcher", "cwDefaultOpen", "","", "Ask which COM port to open
        and save it as default one.")
def cwDefaultOpen(jade=false)
    # Get previous COM port.
    validLastComport = true
    entry = cwGetProfileEntry("lastcomport", "")

    if (entry=="")
        validLastComport = false
        comport = askForComPort
    elsif (entry=="none")
        return
    else
        comport = checkComPortAndReaskIfBad(entry)
    end
    
    success = false
    
    # Open the COM port.
    puts "Opening Chip on COM%s..." % comport.to_s
    
    if(comport == "usb")
        success = uopen()
    else
        if(jade)
            success = jopen(comport)
        else    
            success = copen(comport)
        end
    end
       
    if(success)        
        okputs "[COM OPEN OK]"
        # Save COM port.
        cwSetProfileEntry("lastcomport", comport.to_s)
    else
        errputs "[COM OPEN FAILED]<br>Try to open the port manually after your target is ready with copen, uopen or the like."
        
    end    
    
  	# Launch the event sniffer on the current handle.
	# This allows to see the events received from the chip.
   sniffEvents
    
end

begin
    require 'rubygems'
    require 'win32/sound'
    include Win32
rescue Exception
end

def cwCoin
    Sound.play('resources/coin.wav')
end
