

def handleAssert()
	begin
		puts "html><font color=red>Assert received!! Reading detail...</font>"
        #First event is pointing to the formatted assert string
        event1 = $EVENTSNIFFERCONNECTION.connection.popEvent(1.0) #no timeout
        str1 = event1.RS(256) 
            puts "ASSERT DETAIL : \n%s" % str1		
	rescue Exception => e
	    puts "Failed to read assert detail."
        puts e.inspect
	end
end


addHelpEntry("chip", "chipLastResetTime", "", "", "Returns the last time at which the reset event has been detected.")

$lastResetTime = nil

def chipLastResetTime
    $lastResetTime
end



def chipResetEventCallBack
    puts "html><font color=darkgreen>Detected system reset (0x57).</font>"
    $lastResetTime = Time.now
end

