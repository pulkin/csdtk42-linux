require "base.rb"
require "flamulator/flamulator.rb"
require "flamulator/flamicons.rb"


module CWFlamulator

begin
    cwAddScriptButton("FlamulatorToolbar", "flamulatorChooseLod()", FlamulatorPluginIcons::LOD, "Choose LOD file...")
    cwAddScriptButton("FlamulatorToolbar", "flamulatorChooseCfp()", FlamulatorPluginIcons::CFP, "Choose CFP file...")
    cwAddScriptButton("FlamulatorToolbar", "flamulatorGo()", FlamulatorPluginIcons::FLAMU, "Download to Romulator current LOD and CFP files" )
    cwAddScriptButton("FlamulatorToolbar", "flashDumpUsbToFile()", FlamulatorPluginIcons::TOLOD, "Dump all flash data to  files in lod format through USB.\nMake sure the PC and phone  connetcted through USB" )

    cwAddMenuCommand("Romulator", "Choose LOD file...", "flamulatorChooseLod()", 0)
    cwAddMenuCommand("Romulator", "Choose CFP file...", "flamulatorChooseCfp()", 0)
    cwAddMenuCommand("Romulator","","",0)
    cwAddMenuCommand("Romulator", "Download to Romulator current LOD and CFP files", "flamulatorGo()", 0)
    
rescue #No CoolWatcher.
end

end



    # Calibration parameters save to file
    def flashDumpUsbToFile()
        file = cwGetSaveFileName("Chose file to save","","Flash lod file(*.lod)")
        if(file=="?") #User canceled
            return
        end
        flashDumpUsbToLodFile(file)
    end

def flashDumpUsbToLodFile(file)
    puts "Not implemented!"
end

def flamulatorChooseLod()
    lastLodForDownload  = cwGetProfileEntry("lastLodForDownload","")
    file = cwGetOpenFileName("Choose LOD file for FLAMulation",lastLodForDownload,"*.lod")
    if(file=="?")
        return
    end
    lastLodForDownload = file
    cwSetProfileEntry("lastLodForDownload",lastLodForDownload)
    puts "html>Lod file for flamulation set to: <br><i>%s</i>" % lastLodForDownload
end


def flamulatorChooseCfp()
    lastCfpForFlamulator = cwGetProfileEntry("lastCfpForFlamulator","")
    file = cwGetOpenFileName("Choose CFP file for FLAMulation",lastCfpForFlamulator,"*.cfp")
    if(file=="?")
        cwSetProfileEntry("lastCfpForFlamulator","")
        puts "html>Cfp file for flamulation set to: <i>none</i>" % lastCfpForFlamulator
        return
    end
    lastCfpForFlamulator = file
    cwSetProfileEntry("lastCfpForFlamulator",lastCfpForFlamulator)
    puts "html>Cfp file for flamulation set to: <br><i>%s</i>" % lastCfpForFlamulator
end

def flamulatorGo()
    lastLodForDownload = cwGetProfileEntry("lastLodForDownload","")
    lastCfpForFlamulator = cwGetProfileEntry("lastCfpForFlamulator","")
    if(lastLodForDownload == "")
        puts "No file chosen for FLAMulation!"
        return
    end

	# Disable auto-polling for faster loading
	cwDisableAutoPoll()
	sleep(0.3) # Time for previous command to take effect

	# Flamulate
    files = []
    files << lastLodForDownload
    files << lastCfpForFlamulator if(lastCfpForFlamulator!="")
    flamulate(files, true, false)
    
    begin #Test if we have the plugin, quit if we do not.
        include CoolDisassembly
    rescue Exception
        return
    end
 
    # Load disassembly   
    disfile = lastLodForDownload
    matched = (disfile =~ /(_[^_]*\.lod)/)
    disfile[matched..-1]=".dis"
    if(File.exists?(disfile))
        puts "html> Loading disassembly information from : <i>#{disfile}</i>."
        cwLoadDisassembly(disfile)
    else
        puts "[No disassembly found. Unloading previously loaded disassembly.]"
        cwLoadDisassembly(nil)
    end
end
