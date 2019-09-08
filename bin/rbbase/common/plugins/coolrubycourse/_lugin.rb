# Main plugin script (loaded by coolwatcher at start).

module CoolRubyCourseModule

    @@coolrubyCourseActivated = false

    # This function is used to activate the plugin in CoolWatcher.
    def CoolRubyCourseModule.activateCoolRubyCourseGui

        if (@@coolrubyCourseActivated == false)
            puts "Loading CoolRubyCourse GUI ..."
            
            load "coolrubycourse/coolrubycoursegui.rb"

            @@coolrubyCourseActivated = true
            puts "CoolRubyCourse GUI loaded."
        else
            puts "CoolRubyCourse GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateCoolRubyCourseGui
        CoolRubyCourseModule::activateCoolRubyCourseGui()
    end
    
    begin
        # Add a menu command in CoolWatcher to activate the plugin at start.
        cwAddMenuCommand("Plugins", "Activate CoolRubyCourse","activateCoolRubyCourseGui()",0)
    rescue Exception
        puts "*** CoolWatcher not present. CoolRubyCourse unavailable ***"
    end
    
rescue Exception
end

