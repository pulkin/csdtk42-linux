#!/usr/bin/env ruby

require "ChipStd/target_executor.rb"


module TargetExecutor

    begin
        @@BootSectorGlobals = CH__boot_map_globals
    rescue
        puts "*** WARNING!! The XMD Boot Sector Global vars of the chip are not loaded and the target executor cannot work without them. Please reload the environment with the loading of the Boot Map XMDs. ***"
    end
    
public
    class TargetXtor

        def configEbcRam()
            swBootMode = $SYS_CTRL.Reset_Cause.SW_Boot_Mode.read(@connection)
            if(swBootMode & (1<<0) == 0x00)
                puts "\nNo boot sector struct exists. Skipping ...\n"
                return
            end
            bootSectorStructPtr = $boot_sector_struct_ptr.read(@connection)
            if( (bootSectorStructPtr < 0x81c00000 || bootSectorStructPtr >= 0x81c10000) &&
              (bootSectorStructPtr < 0xa1c00000 || bootSectorStructPtr >= 0xa1c10000) )
                puts "\nWarning: Boot sector struct ptr (0x%08x) is invalid. Skipping ...\n" % bootSectorStructPtr
                return
            end
            ebcTag = $boot_sector_struct_ptr.>.ebcConfigValidTag.read(@connection)
            if(ebcTag != @@BootSectorGlobals::BOOT_SECTOR_EBC_VALID_TAG)
                puts "\nWarning: Boot sector's EBC config tag (0x%08x) is invalid. Skipping ...\n" % ebcTag
                return
            end
            ramTiming = $boot_sector_struct_ptr.>.ebcConfigRamTimings.read(@connection)
            ramMode = $boot_sector_struct_ptr.>.ebcConfigRamMode.read(@connection)
            puts "\nConfiguring EBC RAM ..."
            $MEM_BRIDGE.CS_Time_Write.write(@connection, ramTiming)
            $MEM_BRIDGE.CS_Config[0].CS_Mode.write(@connection, ramMode)
            puts "Done.\n"
        end

        def enterHostMonitor()
            # we want to see events
            enableEvents
            flushFifo

            # For AP connection, the target cannot be reset, but the
            # host monitor commands can still be recognized and executed.
            if($APCONNECTION)
                return
            end

            #Reset the target
            crestart(@connection)
            # Sometimes we need to unfreeze the phone's CPU (for example, when the code
            # loaded into a phone crashes early after the boot).
            cunfreeze(@connection)

            # write enter monitor command
            $INT_REG_DBG_HOST.H2P_STATUS.write(@connection, BOOT_HST_MONITOR_START_CMD)

            #Ack the event, with a timeout
            if (@connection.waitEvent(BOOT_HST_MONITOR_START_EVT, 10))
                disableEvents
                flushFifo                

                puts "Entered Host Monitor mode."

                # Necessary if burst(sync) mode is incompatible with non-burst(async) mode for the RAM chip
                configEbcRam()

                return true
            else
                disableEvents
                flushFifo                

                # Proper event not received
                closeConnection
                raise(EnterHostError,"Enter Host Monitor failed!")
                return false
            end            
        end
    end
end


include TargetExecutor

