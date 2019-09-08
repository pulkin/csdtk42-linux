require "base.rb"
require "fastpf/fastpf.rb"
require "fastpf/fastpficons.rb"

module CWFastpf

begin
    cwAddScriptButton("Fastpf", "fastpfChooseLod()", FastpfPluginIcons::LOD, "Choose LOD file...")
    cwAddScriptButton("Fastpf", "fastpfChooseFlashProgrammer()", FastpfPluginIcons::DRV, "Choose Flash Programmer file...")
    cwAddScriptButton("Fastpf", "fastpfGo()", FastpfPluginIcons::FASTPFGO, "Launch fastpf with current LOD and Flash Programmer files")
    #cwAddScriptButton("Fastpf", "fastpfVerifyGo()", FastpfPluginIcons::FASTPFVERPLUS, "Launch verify (for fastpf) with current LOD and Flash Programmer files")
    
    cwAddMenuCommand("Flash", "Choose LOD file...", "fastpfChooseLod()", 0)
    cwAddMenuCommand("Flash", "Choose Flash Programmer...", "fastpfChooseFlashProgrammer()", 0)
    cwAddMenuCommand("Flash","","",0)
    cwAddMenuCommand("Flash", "Launch fastpf with current LOD file and Flash Programmer", "fastpfGo()", 0)
    cwAddMenuCommand("Flash","","",0)
    cwAddMenuCommand("Flash", "Launch verify (for fastpf) with current LOD and Flash Programmer files","fastpfVerifyGo()",0)
    
rescue #No CoolWatcher.
end

end


def fastpfChooseLod()
    lastLodForDownload  = cwGetProfileEntry("lastLodForDownload", $SOFT_WORKDIR + "/hex")
    if(chipHasAp?())
        postfix = "Images or lods (*.img *.lod);;Images (*.img);;Lods (*.lod)"
    else
        postfix = "Lods (*.lod)"
    end
    file = cwGetOpenFileName("Choose LOD file for fastpfization", lastLodForDownload, postfix)
    if(file=="?")
        return
    end
    lastLodForDownload = file
    cwSetProfileEntry("lastLodForDownload", lastLodForDownload)
    puts "html>Lod file for fastpfization set to: <br><i>%s</i>" % lastLodForDownload
end


def fastpfChooseFlashProgrammer()
    lastFlashProgForFastpf  = cwGetProfileEntry("lastFlashProgForFastpf", $SOFT_WORKDIR + "/toolpool/plugins/fastpf/flash_programmers")
    file = cwGetOpenFileName("Choose flash programmer file for fastpfization", lastFlashProgForFastpf, "*.lod")
    if(file=="?")
        return
    end
    lastFlashProgForFastpf = file
    cwSetProfileEntry("lastFlashProgForFastpf", lastFlashProgForFastpf)
    puts "html>Flash programmer file for fastpfization set to: <br><i>%s</i>" % lastFlashProgForFastpf
end

def fastpfGenericGo(flashProgForFastpf, lodForDownload, mode)
    # If nothing passed, try to get the latest in history.
    if(lodForDownload == "")
        lastLodForDownload = cwGetProfileEntry("lastLodForDownload", "")
    else
        lastLodForDownload = lodForDownload
    end
    if(lastLodForDownload == "")
        puts "No file chosen for fastpfization!"
        return
    end
    if(flashProgForFastpf == "")
        lastFlashProgForFastpf = cwGetProfileEntry("lastFlashProgForFastpf", "")
    else
        lastFlashProgForFastpf = flashProgForFastpf
    end
    if(lastFlashProgForFastpf == "")
        puts "No flash programmer chosen for fastpfization!"
        return
    end
    
    # Add the command with the file names as parameters to the command line history,
    # only when the command line was fastpfGo(), i.e. relying on the profile entries.
    if((lodForDownload == "") && (flashProgForFastpf == ""))
        cwAddCommandHistoryEntry "fastpfGo \"%s\", \"%s\"" % [lastFlashProgForFastpf, lastLodForDownload]
    end
   

    begin
        fastpf(lastFlashProgForFastpf, lastLodForDownload, mode)
    rescue FastPf::FastpfVerifyFailed => e
        puts "html><table><tr><td bgcolor=#FFFFCC width=100%><font color=red>VERIFY FAILED DURING FASTPF. Reason : (#{e.class}): <br>#{e.message} <br>LAUNCHING IN-DEPTH VERIFY.</font></td></tr></table>"
        fastpfVerifyGo()
        return
    end
    
    #If we get here everything was ok
    begin #Test if we have the plugin, quit if we do not.
        include CoolDisassembly
    rescue Exception
        return
    end

    # Load disassembly (this will unload old disassembly if needed).
    disfile = lastLodForDownload
    matched = (disfile =~ /(_[^_]*\.lod)/)
    matched = (disfile =~ /(\.lod)/) if(!matched)
    disfile[matched..-1]=".dis"
    if(File.exists?(disfile))
        puts "html>Loading disassembly information from: <i>#{disfile}</i>."
        cwLoadDisassembly(disfile)
    else
        puts "[No disassembly found. Unloading previously loaded disassembly.]"
        cwLoadDisassembly(nil)
    end        
end

def fastpfGo(flashProgForFastpf="", lodForDownload="")
    fastpfGenericGo(flashProgForFastpf,lodForDownload,FastPf::FULLFASTPF)
end

def fastpfVerifyGo()
    lastLodForDownload = cwGetProfileEntry("lastLodForDownload", "")
    lastFlashProgForFastpf = cwGetProfileEntry("lastFlashProgForFastpf", "")
    if(lastLodForDownload == "")
        puts "No file chosen for fastpfization!"
        return
    end
    if(lastFlashProgForFastpf == "")
        puts "No flash programmer chosen for fastpfization!"
        return
    end
    fastpfVerify(lastFlashProgForFastpf, lastLodForDownload,FastPf::FULLFASTPF)
end
