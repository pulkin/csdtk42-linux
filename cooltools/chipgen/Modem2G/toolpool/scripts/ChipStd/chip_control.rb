addHelpEntry("chip", "turnoff", "", "",
    "Turn off the phone (set the Wakeup line to low).")
def getchipid()
	id= 0;
	#read 8809e2 flash reg
	flashadd8  = $CURRENTCONNECTION.read32(0xa1a25000+0x14)
	puts "8809e2 flash config is 0x%x" % flashadd8
	#read 8955 flash reg
	flashaddz  = $CURRENTCONNECTION.read32(0xa7fff000+0x34)
	puts "8955 flash config is 0x%x" % flashaddz
	if(flashadd8 == 0x211)
	$flashadd = 0xa1a25000;
	end
    if(flashaddz == 0x4000)
	$flashadd = 0xa7fff000;
	end
    puts "flashadd id 0x%x" % $flashadd
end

def turnoff()
    # Disable auto-polling when using coolwatcher.
    # It seems that reading phone registers
    # when it is switched off is a bad idea...
    begin 
        cwDisableAutoPoll()
    rescue
    end
    $INT_REG_DBG_HOST.CTRL_SET.Force_Wakeup.set
    $SYS_CTRL.REG_DBG.w 0xa50001
    $SYS_CTRL.WakeUp.force_Wakeup.w 0
    $INT_REG_DBG_HOST.CTRL_CLR.Force_Wakeup.clear
end
addHelpEntry("chip", "testram", "", "",
    "test ram.")

def testram()
    puts "w 0x82000000,0x12345678"
	puts "w 0x82000004,0xa5a5a5a5"
	w(0x82000000,0x12345678);
	w(0x82000004,0xa5a5a5a5);
	puts "r 0x82000000"
	r(0x82000000);
	puts "r 0x82000004"
	r(0x82000004);
end
