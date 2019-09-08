
module FlowMode
    # In s/w flow control, XON is 0x11 and XOFF is 0x13 by default. The escape character is 0x5c ('\').
    # If a meta character exists in the bit stream, it will be transformed to 2 characters: 0x5c (0xff - <meta to be sent>).
    # E.g., 0x11 will be transformed to 0x5c 0xee.
    FLOWMODE = CoolHost::FLOWMODE_XONXOFF
end

require "ChipStd/base.rb"
require "ChipStd/chip_info.rb"
require "ChipStd/spi_flash.rb"
require "bist.rb"

def enableFlowControlPins()
    val = $CONFIG_REGS.GPIO_Mode.R
    val = $CONFIG_REGS.GPIO_Mode.Mode_PIN_HST_CTS.wl(val,0)
    val = $CONFIG_REGS.GPIO_Mode.Mode_PIN_HST_RTS.wl(val,0)
    $CONFIG_REGS.GPIO_Mode.w val
    val = $DEBUG_UART.ctrl.R
    val = $DEBUG_UART.ctrl.SWRX_flow_ctrl.wl(val,0)
    val = $DEBUG_UART.ctrl.SWTX_flow_ctrl.wl(val,0)
    val = $DEBUG_UART.ctrl.BackSlash_En.wl(val,0)
    $DEBUG_UART.ctrl.w val
    $CURRENTCONNECTION.changeCoolhostFlowMode(CoolHost::FLOWMODE_HARDWARE)
end

def disableFlowControlPins()
    val = $DEBUG_UART.ctrl.R
    val = $DEBUG_UART.ctrl.SWRX_flow_ctrl.wl(val,1)
    val = $DEBUG_UART.ctrl.SWTX_flow_ctrl.wl(val,1)
    val = $DEBUG_UART.ctrl.BackSlash_En.wl(val,1)
    $DEBUG_UART.ctrl.w val
    $CURRENTCONNECTION.changeCoolhostFlowMode(CoolHost::FLOWMODE_XONXOFF)
end

def chipXfbp(connection)
    if (chipDieName() == "8955")
        $SYS_CTRL.XCpu_Dbg_BKP.write(connection,0x130)
    else
        $INT_REG_DBG_HOST.CTRL_SET.w(8)
    end
end

def chipXrbp(connection)
    if (chipDieName() == "8955")
        $SYS_CTRL.XCpu_Dbg_BKP.write(connection,0x0)
    else
        $INT_REG_DBG_HOST.CTRL_CLR.w(8)
    end
end

def chipBfbp(connection)
    if (chipDieName() == "8955")
        $SYS_CTRL.BCpu_Dbg_BKP.write(connection,0x130)
    else
        $INT_REG_DBG_HOST.CTRL_SET.w(0x10)
    end
end

def chipBrbp(connection)
    if (chipDieName() == "8955")
        $SYS_CTRL.BCpu_Dbg_BKP.write(connection,0x0)
    else
        $INT_REG_DBG_HOST.CTRL_CLR.w(0x10)
    end
end

def ispiRead(cs, addr)
    data = (1<<31) | (cs<<29) | (1<<25) | ((addr&0x1ff) << 16)
    w(0x01a13008, data)
    val = R(0x01a13008) & 0xffff
    puts "0x%04X" % val
    val
end

def ispiWrite(cs, addr, data)
    data = (0<<31) | (cs<<29) | (0<<25) | ((addr&0x1ff) << 16) | (data&0xffff)
    w(0x01a13008, data)
end

def pmuRead(addr)
    ispiRead(0, addr)
end

def pmuWrite(addr, data)
    ispiWrite(0, addr, data)
end

def abbRead(addr)
    ispiRead(1, addr)
end

def abbWrite(addr, data)
    ispiWrite(1, addr, data)
end

def xcvReadCtrl()
    ctrl = 0
    ctrl = $RF_SPI.Ctrl.Enable.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.CS_Polarity.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.DigRF_Read.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.Clocked_Back2Back.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Input_Mode.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.Clock_Polarity.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Clock_Delay.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.DO_Delay.wl(ctrl, 2)
    ctrl = $RF_SPI.Ctrl.DI_Delay.wl(ctrl, 3)
    ctrl = $RF_SPI.Ctrl.CS_Delay.wl(ctrl, 2)
    ctrl = $RF_SPI.Ctrl.CS_End_Hold.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Frame_Size.wl(ctrl, 9)
    ctrl = $RF_SPI.Ctrl.CS_End_Pulse.wl(ctrl, 3)
    ctrl = $RF_SPI.Ctrl.Input_Frame_Size.wl(ctrl, 17)
    ctrl = $RF_SPI.Ctrl.TurnAround_Time.wl(ctrl, 2)
    return ctrl
end

def xcvWriteCtrl()
    ctrl = 0
    ctrl = $RF_SPI.Ctrl.Enable.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.CS_Polarity.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.DigRF_Read.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.Clocked_Back2Back.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Input_Mode.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Clock_Polarity.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Clock_Delay.wl(ctrl, 1)
    ctrl = $RF_SPI.Ctrl.DO_Delay.wl(ctrl, 2)
    ctrl = $RF_SPI.Ctrl.DI_Delay.wl(ctrl, 3)
    ctrl = $RF_SPI.Ctrl.CS_Delay.wl(ctrl, 2)
    ctrl = $RF_SPI.Ctrl.CS_End_Hold.wl(ctrl, 0)
    ctrl = $RF_SPI.Ctrl.Frame_Size.wl(ctrl, 27)
    ctrl = $RF_SPI.Ctrl.CS_End_Pulse.wl(ctrl, 3)
    ctrl = $RF_SPI.Ctrl.Input_Frame_Size.wl(ctrl, 17)
    ctrl = $RF_SPI.Ctrl.TurnAround_Time.wl(ctrl, 2)
    return ctrl
end

def xcvRead(addr)
    cmdword = (1 << 31) | ((addr & 0x1ff) << 22)

    $RF_SPI.Ctrl.w(xcvReadCtrl())
    $RF_SPI.Divider.w(3)

    command = 0
    command = $RF_SPI.Command.Flush_Cmd_FIFO.wl(command, 1)
    command = $RF_SPI.Command.Flush_Rx_FIFO.wl(command, 1)
    $RF_SPI.Command.w(command)

    $RF_SPI.Cmd_Data.w((cmdword >> 24) & 0xff)
    $RF_SPI.Cmd_Data.w((cmdword >> 16) & 0xff)

    cmdsize = 0
    cmdsize = $RF_SPI.Cmd_Size.Cmd_Size.wl(cmdsize, 2)
    $RF_SPI.Cmd_Size.w(cmdsize)

    command = 0
    command = $RF_SPI.Command.Send_Cmd.wl(command, 1)
    $RF_SPI.Command.w(command)

    data2 = $RF_SPI.Rx_Data.RF & 0xff
    data1 = $RF_SPI.Rx_Data.RF & 0xff
    data0 = $RF_SPI.Rx_Data.RF & 0xff

    data = ((data2 << 16) | (data1 << 8) | data0) >> 6
    puts "0x%05x" % data
    return data
end

def xcvWrite(addr, data)
    cmdword = ((addr & 0x1ff) << 22) | ((data & 0x3ffff) << 4)

    $RF_SPI.Ctrl.w(xcvWriteCtrl())
    $RF_SPI.Divider.w(3)

    command = 0
    command = $RF_SPI.Command.Flush_Cmd_FIFO.wl(command, 1)
    command = $RF_SPI.Command.Flush_Rx_FIFO.wl(command, 1)
    $RF_SPI.Command.w(command)

    $RF_SPI.Cmd_Data.w((cmdword >> 24) & 0xff)
    $RF_SPI.Cmd_Data.w((cmdword >> 16) & 0xff)
    $RF_SPI.Cmd_Data.w((cmdword >> 8) & 0xff)
    $RF_SPI.Cmd_Data.w((cmdword >> 0) & 0xff)

    cmdsize = 0
    cmdsize = $RF_SPI.Cmd_Size.Cmd_Size.wl(cmdsize, 4)
    $RF_SPI.Cmd_Size.w(cmdsize)

    command = 0
    command = $RF_SPI.Command.Send_Cmd.wl(command, 1)
    $RF_SPI.Command.w(command)
end

