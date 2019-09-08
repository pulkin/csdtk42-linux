module TracerPlugin
    @@tracerActivated = false
    def TracerPlugin.activateTracerGui
        if (@@tracerActivated == false)
            puts "Loading Tracer GUI ..."

            #We need the path to be set before we require anything
            load "tracer/tracergui.rb"

            @@tracerActivated = true
            puts "Tracer GUI loaded."
        else
            puts "Tracer GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateTracerGui
        TracerPlugin::activateTracerGui()
    end

    begin
        cwAddMenuCommand("Plugins", "Activate Tracer","activateTracerGui()",0) if(!$enterprisever)
    rescue Exception
        puts "*** Tracer not present. Tracer unavailable ***"
    end

rescue Exception
end

