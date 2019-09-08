

module TraceSnifferPlugin

    @@traceSnifferActivated = false

    def TraceSnifferPlugin.activateTraceSnifferGui

        if (@@traceSnifferActivated == false)
            puts "Loading TraceTool GUI ..."
            
            #We need the path to be set before we require anything
            load "tracesniffer/tracesniffergui.rb"

            @@traceSnifferActivated = true
            puts "TraceTool GUI loaded."
        else
            puts "TraceTool GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateTraceSnifferGui
        TraceSnifferPlugin::activateTraceSnifferGui()
        setupTraceLevelGui()
        
        defaulttfg = "rbbase/common/plugins/tracesniffer/default.tfg"
        entry = cwGetProfileEntry("defaultTfg","")
        
        if(entry=="")
           cwSetProfileEntry("defaultTfg",defaulttfg)
           entry = defaulttfg 
        end
        
        loadTraceConfig(entry)
        
        sniffTraces()
    end
    
    begin
        cwAddMenuCommand("Plugins", "Activate TraceTool","activateTraceSnifferGui()",0) if(!$enterprisever)
    rescue Exception
        puts "*** TraceTool not present. TraceTool unavailable ***"
    end
    
rescue Exception
end

