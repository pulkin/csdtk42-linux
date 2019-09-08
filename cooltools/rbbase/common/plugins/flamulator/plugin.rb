
module FlamulatorPlugin

    @@FlamulatorActivated = false

    def FlamulatorPlugin.activateFlamulatorGui

        if (@@FlamulatorActivated == false)
            puts "Loading Flamulator GUI ..."
            
            #We need the path to be set before we require anything
            load "Flamulator/flamulatorplugin.rb"

            @@FlamulatorActivated = true
            puts "parse Flamulator GUI loaded."
        else
            puts "Parse Flamulator GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateFlamulatorGui
        FlamulatorPlugin.activateFlamulatorGui()
        puts "The Flamulator is ok!"
    end
    
    begin
        cwAddMenuCommand("Plugins", "Activate Flamulator","activateFlamulatorGui()",0) if(!$enterprisever)
    rescue Exception
        puts "*** Flamulator not present. Flamulator unavailable ***"
    end
    
rescue Exception
end

