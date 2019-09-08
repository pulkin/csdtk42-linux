
module FlowMode
    # In s/w flow control, XON is 0x11 and XOFF is 0x13 by default. The escape character is 0x5c ('\').
    # If a meta character exists in the bit stream, it will be transformed to 2 characters: 0x5c (0xff - <meta to be sent>).
    # E.g., 0x11 will be transformed to 0x5c 0xee.
    FLOWMODE = CoolHost::FLOWMODE_XONXOFF
end

require "ChipStd/base.rb"
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