
module MultiprofilerPlugin

    @@multiprofilerActivated = false

    def MultiprofilerPlugin.activateMultiprofilerGui

        if (@@multiprofilerActivated == false)
            puts "Loading Multi Profiler GUI ..."
            
            #We need the path to be set before we require anything
            load "multiprofiler/mpplugin.rb"

            @@multiprofilerActivated = true
            puts "Multi Profiler GUI loaded."
        else
            puts "Multi Profiler GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateMultiprofilerGui
        MultiprofilerPlugin::activateMultiprofilerGui()
    end
    
    begin
        cwAddMenuCommand("Plugins", "Activate Multi Profiler","activateMultiprofilerGui()",0) if(!$enterprisever)
    rescue Exception
        puts "*** Multi Profiler not present. Multi Profiler unavailable ***"
    end
    
rescue Exception
end

