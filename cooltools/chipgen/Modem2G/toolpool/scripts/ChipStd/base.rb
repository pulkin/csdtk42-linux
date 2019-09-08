require "rbbase/common/base.rb"

def chipDieName()
    return cwGetProfileEntry("chipDie","")
end

def chipHasAp?()
    return (CH__chipstd_globals::ChipHasAp == 1)
end

def isAddressInRam(address)
    if( $PROJNAME.split[0].casecmp("Greenstone") == 0 ||
        $PROJNAME.split[0].casecmp("Gallite") == 0 ||
        $PROJNAME.split[0].casecmp("8808") == 0
      )
        # External RAM memory:
        # PSRAM (0x80000000)
        # Internal RAM memory:
        # BBSRAM (0x81980000), INTSRAM (0x81c00000), DUALPORTSRAM (0x81b00000)
        if( (address >= 0x80000000 && address < 0x82000000) ||
            (address >= 0xa0000000 && address < 0xa2000000)
          )
            return true
        end
    else # 8809 or later
        # External RAM memory:
        # PSRAM (0x82000000)
        # Internal RAM memory:
        # BBSRAM (0x81980000), INTSRAM (0x81c00000), DUALPORTSRAM (0x81b00000)
        if( (address >= 0x82000000 && address < 0x88000000) ||
            (address >= 0xa2000000 && address < 0xa8000000) ||
            (address >= 0x81980000 && address < 0x81e00000) ||
            (address >= 0xa1980000 && address < 0xa1e00000)
          )
            return true
        end

        if(chipHasAp?)
            # MAILBOX (0x818a0000)
            if( (address >= 0x818a0000 && address < 0x81900000) ||
                (address >= 0xa18a0000 && address < 0xa1900000)
              )
                return true
            end
        end
    end

    return false
end

def resetSpiFlash(connection)
    begin
        val = $SPI_FLASH.spi_cmd_addr.prepl(0)
    rescue
        # There is no SPI flash controller
        return
    end

    puts "\nResetting SPI flash..."
    
    # Stop XCPU
    chipXfbp(connection)
    
    # Single line mode
    val = $SPI_FLASH.spi_config.prepl(0)
    val = $SPI_FLASH.spi_config.sample_delay.wl(val,1)
    val = $SPI_FLASH.spi_config.clk_divider.wl(val,2)
    $SPI_FLASH.spi_config.write(connection,val)
    sleep(0.1)
    val = $SPI_FLASH.spi_cmd_addr.prepl(0)
    val = $SPI_FLASH.spi_cmd_addr.spi_tx_cmd.wl(val,0x66)
    val = $SPI_FLASH.spi_cmd_addr.spi_address.wl(val,0)
    $SPI_FLASH.spi_cmd_addr.write(connection,val)
    sleep(0.1)
    val = $SPI_FLASH.spi_cmd_addr.prepl(0)
    val = $SPI_FLASH.spi_cmd_addr.spi_tx_cmd.wl(val,0x99)
    val = $SPI_FLASH.spi_cmd_addr.spi_address.wl(val,0)
    $SPI_FLASH.spi_cmd_addr.write(connection,val)
    
    # Quad line mode
    val = $SPI_FLASH.spi_config.prepl(0)
    val = $SPI_FLASH.spi_config.sample_delay.wl(val,1)
    val = $SPI_FLASH.spi_config.clk_divider.wl(val,2)
    val = $SPI_FLASH.spi_config.cmd_quad.wl(val,1)
    $SPI_FLASH.spi_config.write(connection,val)
    sleep(0.1)
    val = $SPI_FLASH.spi_cmd_addr.prepl(0)
    val = $SPI_FLASH.spi_cmd_addr.spi_tx_cmd.wl(val,0x66)
    val = $SPI_FLASH.spi_cmd_addr.spi_address.wl(val,0)
    $SPI_FLASH.spi_cmd_addr.write(connection,val)
    sleep(0.1)
    val = $SPI_FLASH.spi_cmd_addr.prepl(0)
    val = $SPI_FLASH.spi_cmd_addr.spi_tx_cmd.wl(val,0x99)
    val = $SPI_FLASH.spi_cmd_addr.spi_address.wl(val,0)
    $SPI_FLASH.spi_cmd_addr.write(connection,val)
    
    puts "Done.\n"
end

def chipRestart(connection, bool_xcpufreeze, bool_powerStayOn, bool_reEnableUart, bool_intRegOnly)
    
    evconnection = connection.copyConnection()
    evconnection.open(false)
    evconnection.enableEvents(true)
    
    clkHostOn = false;
    if(!bool_intRegOnly)
        if($DEBUG_HOST.mode.Clk_Host_On.read(evconnection)==1)
            clkHostOn = true;
        end
        
        if(!evconnection.implementsUsb())
            # Reset SPI flash if applicable
            resetSpiFlash(evconnection)
        else
            # SPI flash will be reset by phone itself just before restarting
        end
    end
    
    # Redefining chipRestart
    val = $INT_REG_DBG_HOST.CTRL_SET.prepl(0)
    val = $INT_REG_DBG_HOST.CTRL_SET.Debug_Reset.wl(val,1)
    val = $INT_REG_DBG_HOST.CTRL_SET.XCPU_Force_Reset.wl(val,(bool_xcpufreeze)?1:0) 
    val = $INT_REG_DBG_HOST.CTRL_SET.Force_Wakeup.wl(val,(bool_powerStayOn)?1:0)
    
    $INT_REG_DBG_HOST.CTRL_SET.write(evconnection,val)
    
    if(!evconnection.implementsUsb() && !$APCONNECTION)
        if(clkHostOn)
            evconnection.waitEvent(0xFFFFFFFF,3)
        else
            # 0xFFFFFFFF will be sent on a baud rate derived from 32K.
            # Unless 26M is setup on the target, the baud rate of the debug uart
            # is not correct, and the communication between host and target
            # is broken.
            
            # Wait 0.5 second -- 26M should be setup by then
            sleep(0.5)
        end
        
        # Do a read modify write
        # (In case of com break, this get back the IFC access
        # and the trace.)
        val = $INT_REG_DBG_HOST.CFG.read(evconnection)
        val = $INT_REG_DBG_HOST.CFG.Disable_IFC_H.wl(val,0)
        val = $INT_REG_DBG_HOST.CFG.Disable_Uart_H.wl(val,(bool_reEnableUart)?0:1)
        val = $INT_REG_DBG_HOST.CFG.Force_Prio_H.wl(val,1)
        $INT_REG_DBG_HOST.CFG.write(evconnection,val)
    else
        # Wait the USB/AP connection to be disconnected.
        sleep(0.5)
        
        # If the reset make us lose the connection to the device (just like for the USB).
        # Wait for Coolhost to be reconnected to device.
        evconnection.waitCoolhostConnected()
        # It seems that operations cannot be done on the USB right after the reset so sleep just a bit.
        sleep(0.1)
    end
ensure
    evconnection.close()
end

def chipUnfreeze(connection)
    $INT_REG_DBG_HOST.CTRL_CLR.XCPU_Force_Reset.clear!(connection)
end

def chipXfbp(connection)
    $INT_REG_DBG_HOST.CTRL_SET.Force_BP_XCPU.set!(connection)
      $SYS_CTRL.XCpu_Dbg_BKP.write(connection,0x130)
end

def chipXrbp(connection)
    $INT_REG_DBG_HOST.CTRL_CLR.Force_BP_XCPU.clear!(connection)
    $SYS_CTRL.XCpu_Dbg_BKP.write(connection,0)
end

def chipXcbp(connection)
    val = $INT_REG_DBG_HOST.CTRL_SET.Force_BP_XCPU.read(connection)
    return (val==1)?true:false
end

def chipBfbp(connection)
    $INT_REG_DBG_HOST.CTRL_SET.Force_BP_BCPU.set!(connection)
end

def chipBrbp(connection)
    $INT_REG_DBG_HOST.CTRL_CLR.Force_BP_BCPU.clear!(connection)
    $SYS_CTRL.BCpu_Dbg_BKP.write(connection,0)
end

def chipBcbp(connection)
   val = $INT_REG_DBG_HOST.CTRL_SET.Force_BP_BCPU.read(connection)
   return (val==1)?true:false
end

def chipXDbgIrq(connection)
    $SYS_IRQ.NonMaskable.Debug_IRQ.write(connection, 1)
end

def chipBDbgIrq(connection)
    $BB_IRQ.NonMaskable.Debug_IRQ.write(connection, 1)
end

def chipComEnable(connection)
    puts "Enabling COM ..."
    val = $INT_REG_DBG_HOST.CFG.read(connection)
    val = $INT_REG_DBG_HOST.CFG.prepl(val)
    val = $INT_REG_DBG_HOST.CFG.Disable_Uart_H.wl(val, 0)
    val = $INT_REG_DBG_HOST.CFG.Disable_IFC_H.wl(val, 0)
    $INT_REG_DBG_HOST.CFG.write(connection, val)
end

def chipBusReset(connection, bool_powerStayOn)
    # Debug reset
    # All the modules are reset
    # All the registers including this one will be reset
    puts "Resetting chip and halting XCPU ..."
    val = $INT_REG_DBG_HOST.CTRL_SET.prepl(0)
    val = $INT_REG_DBG_HOST.CTRL_SET.Debug_Reset.wl(val, 1)
    val = $INT_REG_DBG_HOST.CTRL_SET.XCPU_Force_Reset.wl(val, 1)
    val = $INT_REG_DBG_HOST.CTRL_SET.Force_Wakeup.wl(val, (bool_powerStayOn)?1:0)
    $INT_REG_DBG_HOST.CTRL_SET.write(connection, val)
    
    # As XCPU has been kept in reset state after debug reset,
    # we do not need to force breakpoint on XCPU.
=begin
    # Wait at least 0.2 second after debug reset, otherwise
    # the debug host will not accept new command.
    sleep(0.2)
    # Stop XCPU
    puts "Stopping XCPU ..."
    val = $INT_REG_DBG_HOST.CTRL_SET.prepl(0)
    val = $INT_REG_DBG_HOST.CTRL_SET.Force_BP_XCPU.wl(val, 1)
    $INT_REG_DBG_HOST.CTRL_SET.write(connection, val)
=end
    
    sleep(1)
    
    # Enable COM
    chipComEnable(connection)
    
    sleep(1)
    
    puts "Configuring system clock ..."
    # Enable RF_26M
    val = $TCU.LPS_PU_Ctrl.read(connection)
    val = $TCU.LPS_PU_Ctrl.LPS_PU_CLK_RF_On.wl(val, 1)
    $TCU.LPS_PU_Ctrl.write(connection, val)
    # Select 26M
    val = $SYS_CTRL.Cfg_Clk_Sys.read(connection)
    val = $SYS_CTRL.Cfg_Clk_Sys.Freq.wl(val, 1)
    $SYS_CTRL.Cfg_Clk_Sys.write(connection, val)
    # Clock select from RF, not OSC
    val = $SYS_CTRL.Sel_Clock.read(connection)
    val = $SYS_CTRL.Sel_Clock.RF_selected_L.wl(val, 0)
    val = $SYS_CTRL.Sel_Clock.Slow_sel_RF.wl(val, 0)
    $SYS_CTRL.Sel_Clock.write(connection, val)
    
    puts "Configuring external SRAM ..."
    # Config external SRAM memory
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.read(connection)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.CSEn.wl(val, 1)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.Polarity.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.ADMuxMode.wl(val, 1)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WBEMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.BEDlyMode.wl(val, 1)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WaitMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WriteWaitMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.PageSize.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.PageMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WritePageMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.BurstMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WriteBurstMode.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.ADVAsync.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.ADVWEPulse.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.ADMuxDly.wl(val, 3)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WriteSingle.wl(val, 0)
    val = $MEM_BRIDGE.CS_Config[1].CS_Mode.WriteHold.wl(val, 0)
    $MEM_BRIDGE.CS_Config[1].CS_Mode.write(connection, val)
    
    puts "html><font color=red><b>Warning:</b></font>"
    puts "If, unfortunately, the default memory settings cannot work, " \
         "please read the correct ones from a running target and set them to this target."
    puts "CS_Time_Write address : 0x%08x" % $MEM_BRIDGE.CS_Time_Write.address
    puts "CS--1 CS_Mode address : 0x%08x" % $MEM_BRIDGE.CS_Config[1].CS_Mode.address
    puts "CS--1 CS_Time address : 0x%08x" % $MEM_BRIDGE.CS_Config[1].CS_Time.address
end
