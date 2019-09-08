#!/usr/bin/env ruby

begin
    include GPIB
rescue Exception
    puts "*** GPIB MODULE NOT AVAILABLE"
end

require 'help.rb'

# Those functions are exported from CoolRuby DLL.
#addHelpEntry("GPIB","GPIBThreadIbsta","","","")
#addHelpEntry("GPIB","GPIBThreadIberr","","","")
#addHelpEntry("GPIB","GPIBThreadIbcnt","","","")
#addHelpEntry("GPIB","GPIBThreadIbcntl","","","")
#addHelpEntry("GPIB","GPIBdev","","","")
#addHelpEntry("GPIB","GPIBconfig","","","")
#addHelpEntry("GPIB","GPIBtmo","","","")
#addHelpEntry("GPIB","GPIBwrt","","","")
#addHelpEntry("GPIB","GPIBrd","","","")
#addHelpEntry("GPIB","GPIBonl","","","")
