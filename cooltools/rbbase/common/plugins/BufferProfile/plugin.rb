
module BufferProfilePlugin

    @@BufferProfileActivated = false

    def BufferProfilePlugin.activateBufferProfileGui

        if (@@BufferProfileActivated == false)
            puts "Loading Parse Coolprofile GUI ..."
            
            #We need the path to be set before we require anything
            load "BufferProfile/bufferprofileplugin.rb"

            @@BufferProfileActivated = true
            puts "parse Bufferprofile GUI loaded."
        else
            puts "Parse BufferProfile GUI already loaded."
        end
    end
end


#CoolWatcher specific
begin
    include CoolWatcher

    def activateBufferProfileGui
        BufferProfilePlugin.activateBufferProfileGui()
        puts "The BufferProfile is ok!"
    end
    
    begin
        cwAddMenuCommand("Plugins", "Activate BufferProfile","activateBufferProfileGui()",0) if(!$enterprisever)
    rescue Exception
        puts "*** BufferProfile not present. BufferProfile unavailable ***"
    end
    
rescue Exception
end

