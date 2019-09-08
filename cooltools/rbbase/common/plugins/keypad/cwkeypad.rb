cwRegisterPlugin(File.dirname(__FILE__)+"/keypad.dll")

include KeypadModule

def keypadSendKeyToRemote(val,state)    
    # For debugging purpose
    case state
        when Keypad::KPDOWN
            puts "+ %c [KEY HANDLING NOT IMPLEMENTED FOR THIS CHIP]" % val
        when Keypad::KPHOLD
            puts "= %c [KEY HANDLING NOT IMPLEMENTED FOR THIS CHIP]" % val
        when Keypad::KPUP
            puts "- %c [KEY HANDLING NOT IMPLEMENTED FOR THIS CHIP]" % val
    end
end

begin
    require "keys.rb"
rescue LoadError
    puts "**** Warning, this chip does not have a keys.rb script, the keypad will not work."
end

class Keypad
    
    KPDOWN = 0
    KPHOLD = 1
    KPUP   = 2
        
    @@holdThreads = {}
    @@mutex = Mutex.new
    
    def self.hold(val)
        keypadSendKeyToRemote(val,Keypad::KPHOLD)
    end
    
    def self.down(val)
        @@mutex.synchronize{ 
            keypadSendKeyToRemote(val,Keypad::KPDOWN)
            @@holdThreads[val].kill if(@@holdThreads[val])
            @@holdThreads[val] = Thread.new(val) { |v|
                while(true)
                    sleep(0.5)
                    self.hold(v)
                end
            }
        }
    end
    
    def self.up(val)
        @@mutex.synchronize { 
            if(@@holdThreads[val])
                @@holdThreads[val].kill 
                @@holdThreads[val] = nil
            end
            
            keypadSendKeyToRemote(val,Keypad::KPUP)
        }
    end
    
    def self.inspect
        @@holdThreads.inspect
    end
end