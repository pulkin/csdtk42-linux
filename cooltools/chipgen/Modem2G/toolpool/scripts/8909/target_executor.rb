#!/usr/bin/env ruby

require "ChipStd/target_executor.rb"


module TargetExecutor

    begin
        @@UsbGlobals = CH__boot_usb_monitor_globals
    rescue
        puts "*** WARNING!! The XMD USB Global vars of the chip are not loaded and the target executor cannot work without them. Please reload the environment with the loading of the USB XMDs. ***"
    end

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
            $MEM_BRIDGE.CS_Config[1].CS_Mode.write(@connection, ramMode)
            puts "Done.\n"
        end

        def usbEnumDone()
            if(@connection.implementsUsb)
                puts "wait usb enum done"
                sleep 3
            end
        end

        def enterHostMonitor()
            # we want to see events
            enableEvents
            flushFifo
    
            if(@connection.implementsUsb)
                monstate = $boot_usb_monitor.>(@connection).Context.>(@connection).MonitorMode.read(@connection)
                if(monstate == @@UsbGlobals::BOOT_HOST_USB_MODE_BOOT)
                    #Already in the monitor!
                    return true
                end 
            end

            # Without MMI power, ROM will dead loop at USB initialization
            $PMUC.mmi_pwr_enable.pu_mmi_power.w(1)

            #Write the Boot_Mode in Sys_CTRL Registers
            #(Read Modify Write in the Reset_Cause register)
            #Get the value to write in the register without tainting the reset cause
            #All the bitfields will be set to 0 except the Boot_Mode
            ############## puts "#TODO : GET THE BOOT MODE CONSTANT FROM THE XMD"
            previousBootMode = $SYS_CTRL.reset_cause.boot_mode.read(@connection)
            usbenable = (@connection.implementsUsb)?0:1
         
            if(@connection.implementsUsb)
              # Enter in boot sector
              $INT_REG_DBG_HOST.H2P_STATUS.write(@connection, BOOT_HST_MONITOR_ENTER);
              @connection.waitCoolhostConnected()
              sleep(0.2)
            else
=begin
              val = $SYS_CTRL.Reset_Cause.Boot_Mode.wl(0, previousBootMode | (1<<1) | (usbenable<<3))
              #puts "Writing 0x%08X in $SYS_CTRL.Reset_Cause" % val
              $SYS_CTRL.Reset_Cause.write(@connection, val,true) #write forced
=end
              # SW_Boot_Mode is newly introduced in Gallite, which should NOT be cleared unintentionally.
              # So here we update only the register bitfield: Reset_Cause.Boot_Mode,
              # rather than the whole register: Reset_Cause.
              val = previousBootMode | (1<<1) #| (usbenable<<3)
              #puts "Writing 0x%08X in $SYS_CTRL.Reset_Cause.Boot_Mode" % val
              $SYS_CTRL.reset_cause.boot_mode.write(@connection, val,true) #write forced

              #Reset the target
              crestart(@connection)
            
              # Sometimes we need to unfreeze the phone's CPU (for example, when the code
              # loaded into a phone crashes early after the boot).
              cunfreeze(@connection)
            end
                
            # write enter monitor command
            $INT_REG_DBG_HOST.H2P_STATUS.write(@connection, BOOT_HST_MONITOR_START_CMD)
    
            #Ack the event, with a timeout
            if (@connection.waitEvent(BOOT_HST_MONITOR_START_EVT, 10))
                # Put back the previous boot mode, without that, the chip would
                # always enter in the boot monitor after a restart.
                $SYS_CTRL.reset_cause.boot_mode.write(@connection, previousBootMode)
                disableEvents
                flushFifo                

                puts "Entered Host Monitor mode."
               
                # Necessary if burst(sync) mode is incompatible with non-burst(async) mode for the RAM chip
                # TODO: configEbcRam()
                
                return true
            else
                # Put back the previous boot mode, without that, the chip would
                # always enter in the boot monitor after a restart.
                $SYS_CTRL.reset_cause.boot_mode.write(@connection, previousBootMode)
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

