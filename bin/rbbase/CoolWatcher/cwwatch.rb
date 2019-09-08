require 'yaml'

addHelpEntry("CoolWatcher","cwInsertWatchesFromStringArray","array_of_string_of_watches","","Adds a list of watches from array_of_string_of_watches")

def cwInsertWatchesFromStringArray(watchesTab)
    inwin=cwGetRootWatches;
    watchesTab.reverse_each { |w|
        if !(inwin.include?(w)) then
            if w=~/:RubyLabel:(.*):RubyExpression:(.*)/ then
                cwAddRubyWatch($2,$1);
            else
                cwAddWatch(eval(w));
            end
        end
    }
end

addHelpEntry("CoolWatcher","cwSaveWatches","","","Save the list of watches in the current Watches window to a file.")

def cwSaveWatches()
   
    watchesTab = cwGetRootWatches();
    if (watchesTab.empty?) then return end
    
    lastWatchFile  = cwGetProfileEntry("lastWatchFile","rbbase/CoolWatcher/registers.cww")
    file = cwGetSaveFileName("Choose CoolWatcherWatches file",lastWatchFile,"CoolWatcherWatches file (*.cww)")
    if (file=="?")
        return
    end
    lastWatchFile = file
    cwSetProfileEntry("lastWatchFile",lastWatchFile)
    File.open( file, 'w' ) do |output|
        output << YAML::dump(watchesTab);
    end
end

addHelpEntry("CoolWatcher","cwLoadWatches","","","Load the list of watches from a file to the current Watches window.")
def cwLoadWatches(specifiedfile=nil)

    file = ""   
    if(!specifiedfile)
        lastWatchFile  = cwGetProfileEntry("lastWatchFile","rbbase/CoolWatcher/registers.cww")
        file = cwGetOpenFileName("Choose CoolWatcherWatches file",lastWatchFile,"CoolWatcherWatches file (*.cww)")
        if (file=="?")
            return
        end
        lastWatchFile = file
        cwSetProfileEntry("lastWatchFile",lastWatchFile)
    else
        file = specifiedfile
    end
    
    watchesTab = []
    File.open( file, 'r' ) do |input|
        watchesTab = YAML::load( input )
    end
    cwInsertWatchesFromStringArray(watchesTab)
end


def cwRegisterWatchToolbar()
    cwAddRegWatchButton("cwLoadWatches &",CWIconLib::REGOPEN,"Load Watch Configuration...")
    cwAddRegWatchButton("cwSaveWatches &",CWIconLib::REGSAVE,"Save Watch Configuration...")
    cwAddRegWatchButton("cwClearWatches &",CWIconLib::SWEEPICON,"Clear Watches")
    cwAddRegWatchButton("",nil,"")
end

