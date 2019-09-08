require "base.rb"
require "tracesniffer/tracesniffer.rb"

cwRegisterPlugin(File.dirname(__FILE__)+"/traceplugin.dll")

module CWTraceSniffer

begin
    # Add a menu to control the Trace Sniffer.
    #cwAddMenuCommand("Trace Tool", "Start the Trace Tool", "sniffTraces()", 0)
    #cwAddMenuCommand("Trace Tool", "Stop the Trace Tool", "dontSniffTraces()", 0)
    #cwAddMenuCommand("Trace Tool", "", "", 0)
    
    # Some buttons?
    #cwAddToggleButton("TraceSnifferToolbar", "TraceSnifferEnabled", false, true, nil);
rescue #No CoolWatcher.
end

    def tsRunTraceButton()
        sniffTraces()
    end

    def tsStopTraceButton()
        sniffTracesEnable(0)
        dontSniffTraces()
    end

end

include CWTraceSniffer