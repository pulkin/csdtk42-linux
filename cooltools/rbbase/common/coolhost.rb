#!/usr/bin/env ruby
include CoolHost

require 'help.rb'

=begin
CoolHost: implemeted in native codes

handle =    CHST(comport, baudrate, flowmode)
handle =    CHST_USBOpen(usbport)
handle =    CHST_BypassOpen(comport, baudrate, flowmode, [flowid_array])
handle =    CHST_USBBypassOpen(usbport, [flowid_array])
            CHST_Close(handle)

bool =      CHST_IsHandleValid?(handle)
bool =      CHST_IsCoolhostConnected?(handle)
            CHST_CleanCoolhost(handle)
            CHST_FlushCoolhostFifos(handle)
            CHST_ChangeFlowMode(handle, flowmode)

[flowid,[bytes]] = CHST_GetBypassPacket(handle)
[flowid,[bytes]] = CHST_WaitBypassPacket(handle)
            CHST_SendBypassPacket(handle, flowid, [bytes])
            CHST_FlushBypassPackets(handle)

            CHST_EnableEvents(handle, enabled)
event =     CHST_PopEvent(handle)
event =     CHST_WaitEvent(handle, timeout_ms)
            CHST_FlushEvents(handle)

            CHST_LaunchTargetAutoBauding(handle)
            CHST_ForceCoolhostReconnection(handle)
            CHST_KillCoolhost(handle)
            CHST_ComBreak(handle)
            CHST_SetCoolhostUSBIdPolicy(handle, vid, pid)

[bytes] =   CHST_ReadBlock(handle, address, size)
[bytes] =   CHST_ReadBlockMasked(handle, address, size, [mask])
val_u8 =    CHST_ReadInternal(handle, address)
val_u32 =   CHST_Read(handle, address)
val_u32 =   CHST_Read32(handle, address)
val_u16 =   CHST_Read16(handle, address)
val_u8 =    CHST_Read8(handle, address)

            CHST_WriteBlock(handle, address, [bytes])
            CHST_WriteInternal(handle, address, val_u8)
            CHST_Write(handle, address, val_u32)
            CHST_Write32(handle, address, val_u32)
            CHST_Write16(handle, address, val_u16)
            CHST_Write8(handle, address, val_u8)

[packets] = LOD_Read(str_filename, chunksize=0x8000)
            CHST_DumpLod(handle, str_filename, address, nbwords, lod0bin1, isbatch)

=end

addHelpEntry("CoolHost","CHST_TimeOutPopEvent","handle,timeout_s","event","Pop an event from the event queue, or wait for one until 'timeout' (s) is reached. Raises NoEventError if no events could be popped.")
def CHST_TimeOutPopEvent(handle,timeout)
    if(timeout == 0)
        return CHST_PopEvent(handle)
    end
    return CHST_WaitEvent(handle, timeout*1000)
end
