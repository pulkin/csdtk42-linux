# Main plugin script (loaded by coolwatcher at start).

module UsrDataToolModule

    @@usrdatatoolActivated = false

    # This function is used to activate the plugin in CoolWatcher.
    def UsrDataToolModule.activateUsrDataToolGui

        if (@@usrdatatoolActivated == false)
            puts "Loading User data convert tool GUI ..."
            
            load "usrdatatool/usrdatatoolgui.rb"

            @@usrdatatoolActivated = true
            puts "User data convert tool GUI loaded."
        else
            puts "User data convert tool GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateUsrDataToolGui
        UsrDataToolModule::activateUsrDataToolGui()
    end
    
    begin
        # Add a menu command in CoolWatcher to activate the plugin at start.
        cwAddMenuCommand("Plugins", "Activate User data convert tool","activateUsrDataToolGui()",0)
    rescue Exception
        puts "*** CoolWatcher not present. Trace Flash unavailable ***"
    end
    
rescue Exception
end

