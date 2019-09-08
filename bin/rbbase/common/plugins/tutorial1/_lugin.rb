# Main plugin script (loaded by coolwatcher at start).

module Tutorial1Module

    @@tutorial1Activated = false

    # This function is used to activate the plugin in CoolWatcher.
    def Tutorial1Module.activateTutorial1Gui

        if (@@tutorial1Activated == false)
            puts "Loading Tutorial1 GUI ..."
            
            load "tutorial1/tutorial1gui.rb"

            @@tutorial1Activated = true
            puts "Tutorial1 GUI loaded."
        else
            puts "Tutorial1 GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateTutorial1Gui
        Tutorial1Module::activateTutorial1Gui()
    end
    
    begin
        # Add a menu command in CoolWatcher to activate the plugin at start.
        cwAddMenuCommand("Plugins", "Activate Tutorial1","activateTutorial1Gui()",0)
    rescue Exception
        puts "*** CoolWatcher not present. Tutorial1 unavailable ***"
    end
    
rescue Exception
end

