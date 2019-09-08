
module FlowMode
    if($APCONNECTION)
        FLOWMODE = CoolHost::FLOWMODE_HARDWARE
    else
        # In s/w flow control, XON is 0x11 and XOFF is 0x13 by default. The escape character is 0x5c ('\').
        # If a meta character exists in the bit stream, it will be transformed to 2 characters: 0x5c (0xff - <meta to be sent>).
        # E.g., 0x11 will be transformed to 0x5c 0xee.
        FLOWMODE = CoolHost::FLOWMODE_XONXOFF
    end
end

require "ChipStd/base.rb"

def startModem(bootLoaderAddr,bootSectorAddr=0x81c16000,calibMode=false)
    if(calibMode)
        param = 0xca1bca1b
    else
        param = 0
    end
    
    begin
        tgxtor = TargetXtor.new()
        if(bootSectorAddr != 0 && bootSectorAddr != nil)
    	    tgxtor.targetExecConfig(0xff, bootSectorAddr, 0, param, 0)
    	    $AP_COMREGS.ItReg_Clr.w(0x3)
    	    $AP_COMREGS.ItReg_Set.w(0x2)
    	    sleep 1
    	    status = $AP_COMREGS.ItReg_Clr.R
    	    if((status & 0x2) == 0)
    	        puts "Failed to run boot sector codes!"
    	        return
    	    end
    	end
  	    tgxtor.targetExecConfig(0xff, bootLoaderAddr, 0, param, 0)
	    $AP_COMREGS.ItReg_Clr.w(0x2)
   	    $AP_COMREGS.ItReg_Set.w(0x2)
	    sleep 1
	    status = $AP_COMREGS.ItReg_Clr.R
	    if((status & 0x4) == 0)
	        puts "Warning: Modem does NOT request to initialize communication buffer!"
	        return
	    end
	    $AP_COMREGS.ItReg_Clr.w(0x4)
	ensure
	    tgxtor.closeConnection()
	end
end

def startModemCalib(bootLoaderAddr,bootSectorAddr=0x81c16000)
    startModem(bootLoaderAddr,bootSectorAddr,true)
end
