require "base.rb"
require "multiprofiler/multiprofiler.rb"
include MultiProfilerModule
require "multiprofiler/mpicons.rb"

module CWMultiprofiler

begin
    cwAddMenuCommand("Multi Profiler", "Start Recording...", "mpStartProfilingButtonCallback()", 0)
    cwAddMenuCommand("Multi Profiler", "Stop Recording!", "mpStopProfilingButtonCallback()", 0)
    cwAddMenuCommand("Multi Profiler","","",0)
    cwAddMenuCommand("Multi Profiler", "Add configuration Watch", "profilerDisplayControl", 0)

    # Add configuration (For levels, etc) in the register watcher.
    profilerDisplayControl()

rescue #No CoolWatcher.
end

end






