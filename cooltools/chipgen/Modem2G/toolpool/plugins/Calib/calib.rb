include CoolHost

require 'timeout'
class CalibTimeout < StandardError
end

require 'test/unit/assertions'
include Test::Unit::Assertions

require 'lod'

module CoolCalib
    
    #####################################
    #           Private section         #
    #####################################
    private
    #Get the calib constants
    @@C = CH__calib_globals

    #Module constants
    SCRIPT_VERSION = "Modem2G 2.0"
    CALIB_VERSION = @@C::CALIB_MARK_VERSION | @@C::CALIB_MAJ_VERSION << 8 | @@C::CALIB_MIN_VERSION
    CALIB_SEND_CMD_DEFAULT_TIMEOUT = 1.5

    #TODO: Add the GSM850 constant and rename those to reflect frequency,
    # and incidentaly discriminate between GSM850 and GSM900
    # TODO: Use those structures from the XMD of the RFD drivers.
    DEFAULT_CALIB_PCL2DBM_ARFCN_G = [
    3300,   # Pcl  0 to 5 (33 dBm)
    3100,   #Pcl  6 (31 dBm)
    2900,   #Pcl  7 (29 dBm)
    2700,   #Pcl  8 (27 dBm)
    2500,   #Pcl  9 (25 dBm)
    2300,   #Pcl 10 (23 dBm)
    2100,   #Pcl 11 (21 dBm)
    1900,   #Pcl 12 (19 dBm)
    1700,   #Pcl 13 (17 dBm)
    1500,   #Pcl 14 (15 dBm)
    1300,   #Pcl 15 (13 dBm)
    1100,   #Pcl 16 (11 dBm)
    900,    #Pcl 17 ( 9 dBm)
    700,    #Pcl 18 ( 7 dBm)
    500]    #Pcl 19 to 31 (5 dBm)

    DEFAULT_CALIB_PCL2DBM_ARFCN_D = [
    3000,   #Pcl  0 (30 dBm)
    2800,   #Pcl  1 (28 dBm)
    2600,   #Pcl  2 (26 dBm)
    2400,   #Pcl  3 (24 dBm)
    2200,   #Pcl  4 (22 dBm)
    2000,   #Pcl  5 (20 dBm)
    1800,   #Pcl  6 (18 dBm)
    1600,   #Pcl  7 (16 dBm)
    1400,   #Pcl  8 (14 dBm)
    1200,   #Pcl  9 (12 dBm)
    1000,   #Pcl 10 (10 dBm)
    800,    #Pcl 11 ( 8 dBm)
    600,    #Pcl 12 ( 6 dBm)
    400,    #Pcl 13 ( 4 dBm)
    200,    #Pcl 14 ( 2 dBm)
       0,   #Pcl 15 to 28 (0 dBm)
    3200]    #Pcl 29 to 31 (32 dBm)

    DEFAULT_CALIB_PCL2DBM_ARFCN_P = [
    3000,   #Pcl  0 (30 dBm)
    2800,   #Pcl  1 (28 dBm)
    2600,   #Pcl  2 (26 dBm)
    2400,   #Pcl  3 (24 dBm)
    2200,   #Pcl  4 (22 dBm)
    2000,   #Pcl  5 (20 dBm)
    1800,   #Pcl  6 (18 dBm)
    1600,   #Pcl  7 (16 dBm)
    1400,   #Pcl  8 (14 dBm)
    1200,   #Pcl  9 (12 dBm)
    1000,   #Pcl 10 (10 dBm)
    800,    #Pcl 11 ( 8 dBm)
    600,    #Pcl 12 ( 6 dBm)
    400,    #Pcl 13 ( 4 dBm)
    200,    #Pcl 14 ( 2 dBm)
       0,   #Pcl 15 ( 0 dBm)
    3300,   #Pcl 30 (33 dBm)
    3200]   #Pcl 31 (32 dBm)
        
    #Calib module global variables and pointers
    @@MAP_TABLE = nil
    @@CALIBRATION_EBD = nil
    @@FLASH = nil
    @@OP_INFO = nil
    @@XCV = nil
    @@PA = nil
    @@SW = nil
    @@BB = nil
    @@IIR = nil
    @@HST_OP_INFO = nil
    @@HST_XCV = nil
    @@HST_PA = nil
    @@HST_SW = nil
    @@HST_BB = nil
    @@HST_IIR = nil
    @@CTX = nil

    @@VOC = nil
    @@HST_VOC = nil
    @@MUSIC = nil
    @@HST_MUSIC = nil

    @@FLASH_VER = 0
    @@FLASH_OP_DATE = 0
    @@FLASH_OP_METHOD = 0
    @@CODE_VERSION = 0
    @@PARAM_STATUS = 0

    @@FLASH_NEWAUDIO_VER = 0

    @@TargetPclPowG = DEFAULT_CALIB_PCL2DBM_ARFCN_G
    @@TargetPclPowD = DEFAULT_CALIB_PCL2DBM_ARFCN_D
    @@TargetPclPowP = DEFAULT_CALIB_PCL2DBM_ARFCN_P
        
    #C "Macros"
    def CALIB_MARK_MASK(x)            
        return (x & 0xFFFFF0000)
    end
    def CALIB_MAJ_MASK(x)
        return ((x >> 8) & 0xFF)
    end
    def CALIB_MIN_MASK(x)             
        return (x & 0xFF)
    end
    
    #Private utils
    def calib_send_cmd(command, time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT)    
        raise "No CALIBRATION_EBD ! Maybe the calibration initialization has not been done." if(!@@CALIBRATION_EBD)
        #Write command to the Target
        @@CALIBRATION_EBD.command.w(command);

        #Execute until time_out is reached (an exception is raised in that case)
        begin
            Timeout::timeout(time_out) {
                #Wait for the command to be executed
                while( command != @@C::CALIB_CMD_DONE ) do
                    command = @@CALIBRATION_EBD.command.RU
                    raise "Calib Command Update Error" if( command == @@C::CALIB_CMD_UPDATE_ERROR )
                    raise "Calib Command Flash Error" if( command == @@C::CALIB_CMD_FLASH_ERROR )       
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowledge never received!"
        end
    end
    
    def calib_update()
        calib_send_cmd(@@C::CALIB_CMD_UPDATE)
    end
    
    def calib_send_mode(mode,time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT)   
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
    
        #Stop CES activity before executing the command to avoid
        #a FINT re-entrance assert.
        @@CTX.state.w(mode)
    
        #Informs CES that the state recently changed.
        @@CTX.firstFint.w(1)
    
        #Wait for the command to be executed or time_out is reached (an exception is raised in that case)
        begin
            Timeout::timeout(time_out) {
            begin
                firstfint = @@CTX.firstFint.R
            end while(firstfint != 0)
        }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowledge never received!"       
        end
    end
    
    
    #Set the calibration method and time and do the update.

    def calib_set_method(automaticTool)
        #Check the op_info pointer in the Target.
        raise "No HST opInfo !" if(! @@HST_OP_INFO )
        
        #Set the status buffer to the calib method and the current time.
        method = (automaticTool) ? (@@C::CALIB_METH_AUTOMATIC) : (@@C::CALIB_METH_MANUAL)

        @@HST_OP_INFO.date.w( Time.now.to_i ); #TODO : sure of that ???????????
        @@HST_OP_INFO.method.w(method)
    
        #Update the calib buffers in the Target
        calib_send_cmd(@@C::CALIB_CMD_UPDATE)
    end

    
    def calib_mode_rx(band,arfcn,expPow,tag)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
    
        #Give a good default value to the tag
        tag = @@C::CALIB_STUB_MONIT_STATE if(tag==0)
        
        #Send the parameters to the Target.
        @@CTX.monBand.w(band)
        @@CTX.monArfcn.w(arfcn)
        @@CTX.monExpPow.w(expPow)
        
        #Send the mode command to CES
        calib_send_mode(tag)
    end

    def calib_mode_tx(band,arfcn,pcl,dacIdVal,tsc,tag)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
    
        #Give a good default value to the tag
        tag = @@C::CALIB_STUB_MONIT_STATE if(tag==0)
    
        #Send the parameters to the Target.
        @@CTX.txBand.w(band)
        @@CTX.txArfcn.w(arfcn)
        @@CTX.txTsc.w(tsc)
        
        if( tag == @@C::CALIB_STUB_PA_STATE ) then
            @@CTX.txDac.w(dacIdVal)
        else
            @@CTX.txPcl.w(pcl)
        end
        
        #Send the mode command to CES
        calib_send_mode(tag)
    end


    
    #####################################
    #           Public section          #
    #####################################       
    public

    addHelpEntry("Calibration",
                 "calib_init",
                 "",
                 "bool, bool",
                 "Initializes the calibration class variables, \
                  setting them to values read from the calibration \
                  structure in the Target. That means \
                  that a calib_init call is needed at least \
                  once before using the calibration methods.\n \
                  Returns calib_not_mod,phy_not_ready. calib_not_mod is \
                  TRUE if the calibration is read-only (not modifiable). \
                  phy_not_ready is TRUE if the code running on the target \
                  is not ready for the calibration.
                  Usable with both the Stub and the Daemon.
                  ")
    def calib_init()

        calib_not_mod = false
        phy_not_ready = false

#FIXME Check this function variable underscoring    
        #Read config stub pointer
        @@MAP_TABLE = $map_table.>
        raise "Config stub pointer is null !!" if(!@@MAP_TABLE)

        #Read (TODO Calib ? Map ?) version
        ver = @@MAP_TABLE.calib_version
        #TODO : verify the config version
        #   

        #Read the calibration pointer
        @@CALIBRATION_EBD = @@MAP_TABLE.calib_access.>

        #Check if we have a valid calibration_ebd object
        raise "No calibration mechanism available (embedded calibration pointer is null) !!" if(!@@CALIBRATION_EBD)

        #Read code version and param status
        @@CODE_VERSION = @@CALIBRATION_EBD.codeVersion.RU #& 0xFFFFFFFF
        @@PARAM_STATUS = @@CALIBRATION_EBD.paramStatus.RU #& 0xFFFFFFFF

        mark = CALIB_MARK_MASK(@@CODE_VERSION)
        maj_ver = CALIB_MAJ_MASK(@@CODE_VERSION)
        min_ver = CALIB_MIN_MASK(@@CODE_VERSION)

        raise "Calib version of PHY has not the good mark: %x." % mark if(mark!=@@C::CALIB_MARK_VERSION)
        raise "Calib version of PHY do not match HST one: %x." %maj_ver if(maj_ver!=@@C::CALIB_MAJ_VERSION)

        #FLASH things
        @@FLASH                 = @@CALIBRATION_EBD.flash.>
        @@FLASH_VER             = @@FLASH.versionTag.RU
        @@FLASH_AUDIO_VER       = @@FLASH.audioVersionTag.RU
        @@FLASH_OP_METHOD       = @@FLASH.opInfo.method.RU
        @@FLASH_OP_DATE         = @@FLASH.opInfo.date.RU
        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206))
            @@FLASH_NEWAUDIO_VER    = @@FLASH.audioExpand1VersionTag.RU
        end

        #Pointers 
        @@OP_INFO               = @@CALIBRATION_EBD.opInfo.>
        @@XCV                   = @@CALIBRATION_EBD.xcv.>
        @@PA                    = @@CALIBRATION_EBD.pa.>
        @@SW                    = @@CALIBRATION_EBD.sw.>
        @@BB                    = @@CALIBRATION_EBD.bb.>

        @@HST_OP_INFO           = @@CALIBRATION_EBD.hstOpInfo.>
        @@HST_XCV               = @@CALIBRATION_EBD.hstXcv.>
        @@HST_PA                = @@CALIBRATION_EBD.hstPa.>
        @@HST_SW                = @@CALIBRATION_EBD.hstSw.>
        @@HST_BB                = @@CALIBRATION_EBD.hstBb.>
        
        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206))
            @@IIR                   = @@CALIBRATION_EBD.iirParam.>
            @@VOC                   = @@CALIBRATION_EBD.audio_voc.>
            @@HST_IIR               = @@CALIBRATION_EBD.hstIIRParam.>
            @@MUSIC                 = @@CALIBRATION_EBD.audio_music.>
            #@@HST_VOC               = @@CALIBRATION_EBD.hstaudio_voc.>
            #@@HST_MUSIC             = @@CALIBRATION_EBD.hstaudio_music.>
        end

        begin
            @@CTX               = @@CALIBRATION_EBD.stubCtx.>
            calib_not_mod       = false
        rescue NullPointer
            puts "WARNING !!! NO STUB CONTEXT PRESENT"
            calib_not_mod       = true
        end

        if (!calib_not_mod) then
            phy_not_ready       = (@@CALIBRATION_EBD.command.RU != @@C::CALIB_CMD_DONE &&
                                   @@CALIBRATION_EBD.command.RU != @@C::CALIB_CMD_NOT_ACCESSIBLE)
        end

        return [calib_not_mod,phy_not_ready]
    end
    



    addHelpEntry("Calibration",
                 "calib_get_versions",
                 "calib_not_mode, phy_not_ready, initCalib(true)",
                 "version_str",
                 "This function returns various version numbers from the calibration \
                 structure and flash sector. \n \
                 Its parameters are the results from #calib_init. \n \
                 The function analysis the versions numbers of the calibration \
                 structure and the flash calibration sector. It checks their
                 compatibility with the calibration scripts. If available, 
                 it also returns the way the calibration sector was \
                 produced (automatic, manual, simulation, etc ...). \n \
                 The return type is a character string.
                initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                 NB: if the initCalib parameter is set to true, the other
                 two parameters (calib_not_mod,phy_not_ready) are ignored
                 as the result from calib_init() is internally used instead.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_get_versions(calib_not_mod, phy_not_ready, initCalib=true)
        calib_not_mod, phy_not_ready = calib_init() if(initCalib)
        
        mark = CALIB_MARK_MASK(@@CODE_VERSION)
        maj_ver = CALIB_MAJ_MASK(@@CODE_VERSION)
        min_ver = CALIB_MIN_MASK(@@CODE_VERSION)
        
        version_text = "Project Script Version: %s.\n" % SCRIPT_VERSION
        version_text += "Calib Tool Version: %02d.%02d.\n" % [@@C::CALIB_MAJ_VERSION,@@C::CALIB_MIN_VERSION]
        
        if( mark != @@C::CALIB_MARK_VERSION || maj_ver != @@C::CALIB_MAJ_VERSION ) then
            version_text += "This script can NOT read or write the calib parameters for a running code with this version!\n"
            version_text += "(Expection: 0x%08X - Read: 0x%08X)" % [ CALIB_VERSION, @@CODE_VERSION ]
        elsif(calib_not_mod)
            version_text+="The calib parameters can be read.\n"
        else
            version_text+="The calib parameters can be read and written.\n"
            version_text+="Running code on the Target not ready for calib.\n"  if(phy_not_ready) 
        end
        version_text += "Calib Tool Version: %02d.%02d.\n" % [maj_ver,min_ver]

        if( CALIB_MARK_MASK(@@FLASH_VER) == @@C::CALIB_MARK_VERSION ||
            CALIB_MARK_MASK(@@FLASH_AUDIO_VER) == @@C::CALIB_MARK_VERSION ) then

            paramStatus = @@PARAM_STATUS
            if (paramStatus == @@C::CALIB_PARAM_DEFAULT)
                version_text += "RF: Default, AUDIO: Default"
            elsif (paramStatus == @@C::CALIB_PARAM_CALIBRATED)
                version_text += "RF: Calibrated, AUDIO: Calibrated"
            elsif (paramStatus == @@C::CALIB_PARAM_DEFAULT_RF_MIS)
                version_text += "RF: NameMismatched, AUDIO: Default"
            elsif (paramStatus == @@C::CALIB_PARAM_RF_CALIBRATED)
                version_text += "RF: Calibrated, AUDIO: Default"
            elsif (paramStatus == @@C::CALIB_PARAM_AUDIO_CALIBRATED)
                version_text += "RF: Default, AUDIO: Calibrated"
            elsif (paramStatus == @@C::CALIB_PARAM_AUDIO_CALIBRATED_RF_MIS)
                version_text += "RF: NameMismatched, AUDIO: Calibrated"
            else
                version_text += "paramStatus = 0x%08X" % paramStatus
            end
            version_text += "\n"

            if (CALIB_MARK_MASK(@@FLASH_VER) == @@C::CALIB_MARK_VERSION)
                version_text += "RF Version: %02d.%02d.\n" %
                    [CALIB_MAJ_MASK(@@FLASH_VER),CALIB_MIN_MASK(@@FLASH_VER)]
            end
            if (CALIB_MARK_MASK(@@FLASH_AUDIO_VER) == @@C::CALIB_MARK_VERSION)
                version_text += "AUDIO Version: %02d.%02d.\n" %
                    [CALIB_MAJ_MASK(@@FLASH_AUDIO_VER),CALIB_MIN_MASK(@@FLASH_AUDIO_VER)]
            end

            if(CALIB_MAJ_MASK(@@FLASH_VER) == maj_ver) then
                version_text += "Calib sector was build using "

                case(@@FLASH_OP_METHOD) 
                    when @@C::CALIB_METH_DEFAULT
                        version_text += "Default method (Weird)"
                    when @@C::CALIB_METH_COMPILATION
                        version_text += "Compilation method"
                    when @@C::CALIB_METH_MANUAL
                        version_text += "Manual method"
                    when @@C::CALIB_METH_AUTOMATIC
                        version_text += "Automatic method"
                    when @@C::CALIB_METH_SIMULATION
                        version_text += "Simulation method"
                    else
                        version_text += "Unknown method"             
                end

                version_text += " at %s.\n" % Time.at(@@FLASH_OP_DATE).localtime
            end
        else
            version_text += "NO Calib Sector Found!\n"
            version_text += "(Expecting: 0x%08X - Read: 0x%08X / 0x%08X)\n" % [CALIB_VERSION, @@FLASH_VER, @@FLASH_AUDIO_VER]
        end

        return version_text
    end



    addHelpEntry("Calibration",
                 "calib_get_rf_chip_names",
                 "initCalib(true)",
                 "[xcv_name,pa_name,sw_name]",
                 "This function returns the three version numbers respectively \
                 for the XCV, PA and SW chips
                  Usable with both the Stub and the Daemon.
                 ")

    def calib_get_rf_chip_names(initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        raise "No PA context !" if(!@@PA)
        raise "No SW context !" if(!@@SW)
        xcv_name = @@XCV.palcust.name.R
        pa_name = @@PA.palcust.name.R
        sw_name = @@SW.palcust.name.R
        return [xcv_name,pa_name,sw_name]
    end
    
    addHelpEntry("Calibration",
                 "calib_get_rf_chip_names_str",
                 "initCalib(true)",
                 "[xcv_name_str,pa_name_str,sw_name_str]",
                 "This function (when implemented) will return the plain text \
                 version of the radio-chips (XCV, PA, and SW) embedded in the \
                 target. \n \
                 TODO: Implement !!!!!
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_get_rf_chip_names_str(initCalib=true)
        calib_init() if(initCalib)
        #TODO : add the string feature to the XML
        #The names should be taken from there
        xcv_name,pa_name,sw_name = calib_get_rf_chip_names()
        xcv_name_str = "XCV number : %08x (TODO : have real name)"  % xcv_name
        pa_name_str = "PA number : %08x (TODO : have real name)"        % pa_name
        sw_name_str = "SW number : %08x (TODO : have real name)"        % sw_name
        return [xcv_name_str,pa_name_str,sw_name_str]
    end
    
    #############################################
    #               CALIB COMMANDS              #
    #############################################
    
    #===========================================#
    #               Set Functions               #
    #===========================================#
    
    #Set the timing 'id' of an RF chip.
    addHelpEntry("Calibration",
                 "calib_set_xcv_time",
                 "id, time, initCalib(true)",
                 "",
                 "Set the 'id'th timing parameter to the 'time' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_xcv_time(id,time, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_XCV context !" if(!@@HST_XCV)
        @@HST_XCV.times[id].w(time)
        calib_update()
    end
    



    addHelpEntry("Calibration",
                 "calib_set_pa_time",
                 "id, time, initCalib(true)",
                 "",
                 "Set the 'id'th timing parameter  to the 'time' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_pa_time(id,time, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        @@HST_PA.times[id].w(time)
        calib_update()
    end
    



    addHelpEntry("Calibration",
                 "calib_set_sw_time",
                 "id, time, initCalib(true)",
                 "",
                 "Set the 'id'th timing parameter  to the 'time' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_sw_time(id,time, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_SW context !" if(!@@HST_SW)
        @@HST_SW.times[id].w(time)
        calib_update()
    end
    
    
    
    
    addHelpEntry("Calibration",
                 "calib_set_pal_time",
                 "id, time, initCalib(true)",
                 "",
                 "Set the 'id'th timing parameter  to the 'time' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_pal_time(id,time, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_BB context !" if(!@@HST_BB)
        @@HST_BB.times[id].w(time)
        calib_update()
    end




    #Set the parameter 'id' of an RF chip.
    addHelpEntry("Calibration",
                 "calib_set_xcv_param",
                 "id, param, initCalib(true)",
                 "",
                 "Set the 'id'th parameter  to the 'param' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_xcv_param(id,param, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_XCV context !" if(!@@HST_XCV)
        @@HST_XCV.param[id].w(param)
        calib_update()
    end
    



    addHelpEntry("Calibration",
                 "calib_set_pa_param",
                 "id, param, initCalib(true)",
                 "",
                 "Set the 'id'th parameter to the 'param' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_pa_param(id,param, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        @@HST_PA.param[id].w(param)
        calib_update()
    end




    addHelpEntry("Calibration",
                 "calib_set_sw_param",
                 "id, param, initCalib(true)",
                 "",
                 "Set the 'id'th parameter to the 'param' value. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_sw_param(id,param, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_SW context !" if(!@@HST_SW)
        @@HST_SW.param[id].w(param)
        calib_update()
    end




    #Set the value 'id' used to do the PA profile interpolation.
    addHelpEntry("Calibration",
                 "calib_set_pa_profile_interp",
                 "band, id, val, initCalib(true)",
                 "",
                 "In the HST_PA calibration structure, for the given 'band', \
                 sets the 'id'th value of the interpolation profile to 'val'.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_pa_profile_interp(band,id,val, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        if(band==@@C::CALIB_STUB_BAND_DCS1800 || band==@@C::CALIB_STUB_BAND_PCS1900) then
            @@HST_PA.palcust.profileInterpDp[id].w(val)
        else
            #@@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            @@HST_PA.palcust.profileInterpG[id].w(val)
        end
    end




    #Set Rx-Tx frequency offset for any band.
    addHelpEntry("Calibration",
                 "calib_set_rxtx_freq_offset",
                 "band, fof, initCalib(true)",
                 "",
                 "Set the Rx-Tx frequency offset of 'band' to 'fof'.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_rxtx_freq_offset(band,fof, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_XCV context !" if(!@@HST_XCV)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        @@HST_XCV.palcust.rxTxFreqOffset[band].w(fof)
        calib_update();
    end
    



    #Set Rx-Tx time offset for any band.
    addHelpEntry("Calibration",
                 "calib_set_rxtx_time_offset",
                 "tof, initCalib(true)",
                 "",
                 "The the Rx-Tx time offset. This is common for every band.
                  Usable with both the Stub and the Daemon.
                 ") 
    def calib_set_rxtx_time_offset(tof, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_XCV context !" if(!@@HST_XCV)
        @@HST_XCV.palcust.rxTxTimeOffset.w(tof)
        calib_update();
    end 



    #Set Tx target power per PCL for any band.
    addHelpEntry("Calibration",
                 "calib_set_target_power_per_pcl",
                 "band, pclId, power, initCalib(true)",
                 "",
                 "Set the Tx Target power for the PCL 'pclId' to 'power' for \
                 the 'band' band.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                  Usable with both the Stub and the Daemon.
                ")
    def calib_set_target_power_per_pcl(band,pclId,power, initCalib=true)
        calib_init() if(initCalib)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        case band
            when @@C::CALIB_STUB_BAND_PCS1900
                @@TargetPclPowP[pclId] = power
            when @@C::CALIB_STUB_BAND_DCS1800
                @@TargetPclPowD[pclId] = power
            else
                #@@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
                @@TargetPclPowG[pclId] = power
        end
    end



    #Set Tx power per PCL per ARFCN for any band.
    addHelpEntry("Calibration",
                 "calib_set_power_per_pcl_per_arfcn",
                 "band, pclId, arfcn, measuredPower, initCalib(true)",
                 "",
                 "Set the measured power for the 'pclId' PCL, 'arfcn' ARFCN, \
                 and 'band' band. The power offset is actually written by the\
                 script into the HST_PA structure.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                 Only usable with the calib stub.
                ")
    def calib_set_power_per_pcl_per_arfcn(band,pclId,arfcn,measuredPower, initCalib=true)
        calib_init() if(initCalib)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        assert(arfcn >=0 && arfcn<=1, "ARFCN must be 0 or 1")
        raise "No HST_PA context !" if(!@@HST_PA)
        case band
            when @@C::CALIB_STUB_BAND_PCS1900
                #Read the previous power.
                prevPower = @@HST_PA.palcust.pcl2dbmArfcnP[pclId][arfcn].R
                #Compute the new power, by calculating the offset.
                power = prevPower - (measuredPower - @@TargetPclPowP[pclId])
                #Write it
                @@HST_PA.palcust.pcl2dbmArfcnP[pclId][arfcn].w(power)
            when @@C::CALIB_STUB_BAND_DCS1800
                #Read the previous power.
                prevPower = @@HST_PA.palcust.pcl2dbmArfcnD[pclId][arfcn].R
                #Compute the new power, by calculating the offset.
                power = prevPower - (measuredPower - @@TargetPclPowD[pclId])
                #Write it
                @@HST_PA.palcust.pcl2dbmArfcnD[pclId][arfcn].w(power)
            else
                #@@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
                #Read the previous power.
                prevPower = @@HST_PA.palcust.pcl2dbmArfcnG[pclId][arfcn].R
                #Compute the new power, by calculating the offset.
                power = prevPower - (measuredPower - @@TargetPclPowG[pclId])
                #Write it
                @@HST_PA.palcust.pcl2dbmArfcnG[pclId][arfcn].w(power)
        end
    end
    



    #Set PA DAC profile for GSM or DCS-PCS and do the update.
    addHelpEntry("Calibration",
                 "calib_set_profile",
                 "band, measuredPowers, initCalib(true)",
                 "",
                 "Set PA DAC profile for GSM (850 or 900) or DCS-PCS and do the update. \
                 'band' is the band concerned by the profiles. \
                 'measuredPowers' is an array of the 16 points the CES has asked \
                 to measure to extrapolate a ramp.
                 TODO Correct this ...
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_profile(band,measuredPowers, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        
        min = measuredPowers[0]
        max = measuredPowers[15]
        
        #Avoid the division by zero in the next calculations.
        assert(max-min!=0,"Max and min measured powers are equal !")
        
        #Check the monotony for the PA profile.
        1.upto(15) { |i| assert( (measuredPowers[i-1] - measuredPowers[i]) > 0,"Profile is not monotonous !") }
        
        #Stop CES activity before executing the command to avoid a FINT re-entrance assert.
        calib_mode_stop()
        
        if(band == @@C::CALIB_STUB_BAND_DCS1800 || band == @@C::CALIB_STUB_BAND_PCS1900) then
            @@HST_PA.palcust.profileDbmMinDp.w(min)
            @@HST_PA.palcust.profileDbmMaxDp.w(max)
            
            #Resize the power from dB to a range from 0 to 1023 and send values to the Target for interpolation
            j = 0
            @@C::CALIB_PADAC_PROF_INTERP_QTY.times { |i| 
                @@HST_PA.palcust.profileDp[j].w( 1023 * (measuredPowers[i] - min)/(max - min) )
                j += 1
            }
            
            #Send the command to interpolate the profile on the Target.
            calib_send_cmd(@@C::CALIB_CMD_PA_PROFILE_DCSPCS)
        else
            # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            @@HST_PA.palcust.profileDbmMinG.w(min)
            @@HST_PA.palcust.profileDbmMaxG.w(max)
            
            #Resize the power from dB to a range from 0 to 1023 and send values to the Target for interpolation
            j = 0
            @@C::CALIB_PADAC_PROF_INTERP_QTY.times { |i| 
                @@HST_PA.palcust.profileG[j].w( 1023 * (measuredPowers[i] - min)/(max - min) )
                j += 1
            }       
            
            #Send the command to interpolate the profile in the Target.
            calib_send_cmd(@@C::CALIB_CMD_PA_PROFILE_GSM)
        end
    end




    #Set PA DAC ramp used for low or high power levels.
    addHelpEntry("Calibration",
                 "calib_set_ramp_low",
                 "id, val, initCalib(true)",
                 "",
                 "Set PA DAC ramp used for low power level. This ramp is made 
                 of 32 values, whose 'id'th is set to 'val'.
                 <br> The first 15 values describe the up ramp.
                 <br> The last 15 values describe the down ramp.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_ramp_low(id,val, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        @@HST_PA.palcust.rampLow[id].w(val)
        calib_update();
    end


    addHelpEntry("Calibration",
                 "calib_set_ramp_high",
                 "id, val, initCalib(true)",
                 "",
                 "Set PA DAC ramp used for high power level. This ramp is made \
                 of 32 values, whose 'id'th is set to 'val'
                 <br> The first 15 values describe the up ramp.
                 <br> The last 15 values describe the down ramp.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_ramp_high(id,val, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        @@HST_PA.palcust.rampHigh[id].w(val)
        calib_update();
    end



    #Set PA DAC ramp switch from low to high power for GSM or DCS-PCS.
    addHelpEntry("Calibration",
                 "calib_set_ramp_switch",
                 "band, val, initCalib(true)",
                 "",
                 "Set PA DAC ramp switch from low to high power for the 'band' \
                 band to 'val'.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_ramp_switch(band,val, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1])
        if(band == @@C::CALIB_STUB_BAND_DCS1800 || band == @@C::CALIB_STUB_BAND_PCS1900) then
            @@HST_PA.palcust.rampSwapDp.w(val)
        else
            # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            @@HST_PA.palcust.rampSwapG.w(val)
        end
        calib_update();
    end
    


    #Set PA DAC low voltage limitations
    addHelpEntry("Calibration",
                 "calib_set_low_volt",
                 "id, val, initCalib(true)",
                 "",
                 "PA DAC low voltage limitations
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_low_volt(id,val, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        @@HST_PA.palcust.lowVoltLimit[id].w(val)
        calib_update();
    end
    
    #Set PA DAC low DAC limitations
    addHelpEntry("Calibration",
                 "calib_set_low_dac",
                 "id, val, initCalib(true)",
                 "",
                 "PA DAC low DAC limitations
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_low_dac(id,val, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_PA context !" if(!@@HST_PA)
        @@HST_PA.palcust.lowDacLimit[id].w(val)
        calib_update();
    end 
    
    # Set VoC Echo Cancelling parameters for any audio interface.
    addHelpEntry("Calibration",
                 "calib_set_voc_ec",
                 "itf, io, vocEc, initCalib(true)",
                 "",
                 "Set VoC Echo Cancelling parameters for the given IO set of 
                 the given Audio Interface.
                 The voc parameter must be a structure of 
                 type CH__CALIB_AUDIO_VOC_EC.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_voc_ec(itf, io, vocEc, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_BB context !" if(!@@HST_BB)
        @@HST_BB.audio[itf].vocEc[io].ecMu.w(vocEc.ecMu.getl)
        @@HST_BB.audio[itf].vocEc[io].ecRel.w(vocEc.ecRel.getl)
        @@HST_BB.audio[itf].vocEc[io].ecMin.w(vocEc.ecMin.getl)
        @@HST_BB.audio[itf].vocEc[io].esOn.w(vocEc.esOn.getl)
        @@HST_BB.audio[itf].vocEc[io].esDtd.w(vocEc.esDtd.getl)
        @@HST_BB.audio[itf].vocEc[io].esVad.w(vocEc.esVad.getl)
        @@HST_BB.audio[itf].vocEc[io].enableField.w(vocEc.enableField.getl)
        calib_update();
    end

    
    # Set VoC Filters parameters for any audio interface.
    addHelpEntry("Calibration",
                 "calib_set_voc_filters",
                 "itf, io, vocFilters, initCalib(true)",
                 "",
                 "Set VoC Filter parameters for the given IO set of 
                 the given Audio Interface.
                 The voc parameter must be a structure of 
                 type CH__CALIB_AUDIO_VOC_FILTERS.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_voc_filters(itf, io, vocFilters, initCalib=true)
        calib_init() if(initCalib)  
        raise "No HST_BB context !" if(!@@HST_BB)
        @@C::CALIB_VOC_MDF_QTY.times { |i| @@HST_BB.audio[itf].vocFilters[io].mdfFilter[i].w(vocFilters.mdfFilter[i].getl) }
        @@C::CALIB_VOC_SDF_QTY.times { |i| @@HST_BB.audio[itf].vocFilters[io].sdfFilter[i].w(vocFilters.sdfFilter[i].getl) }    
        calib_update();
    end

    # Set gains for any audio interface.
    addHelpEntry("Calibration",
                 "calib_set_gains",
                 "itf, io, gains, initCalib(true)",
                 "",
                 " Set gains for any IO set of any Audio Interface.
                 (An IO set is for example 'loudspeaker' or 'earpiece', 
                 and an Audio Interface can be the analog one or a 
                 I2S chip , etc)
                 gain
                 is of type CH_CALIB_AUDIO_GAIN. 
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_gains(itf, io, gains, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_BB context !" if(!@@HST_BB)
        @@HST_BB.audioGains[itf].inGain.w(gains.inGain.getl)
            
        @@C::CALIB_AUDIO_GAIN_QTY.times { |i| @@HST_BB.audio[itf].audioGains[io].outGain[i].w(gains.outGain[i].getl) }
        @@C::CALIB_AUDIO_GAIN_QTY.times { |i| @@HST_BB.audio[itf].audioGains[io].sideTone[i].w(gains.sideTone[i].getl) }    
        @@C::CALIB_AUDIO_GAIN_QTY.times { |i| @@HST_BB.audio[itf].audioGains[io].amplGain[i].w(gains.amplGain[i].getl) }
        calib_update();
    end

    #Set GPADC sensor gains.
    addHelpEntry("Calibration",
                 "calib_set_gpadc_sensor",
                 "gain_a,gain_b, initCalib(true)",
                 "",
                 " Set the GPADC sensor gains for the A and B 
                 points of the GPADC calibration.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_gpadc_sensor(gain_a,gain_b, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_BB context !" if(!@@HST_BB)
        @@HST_BB.gpadc.sensorGainA.w(gain_a)
        @@HST_BB.gpadc.sensorGainB.w(gain_b)
        calib_update();
    end
    
    #Set I/Q time offset.
    addHelpEntry("Calibration",
                 "calib_set_rx_iq_time_offset",
                 "offset, initCalib(true)",
                 "",
                 "Set the Rx I/Q time offset.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_rx_iq_time_offset(offset, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_XCV context !" if(!@@HST_XCV)
        @@HST_XCV.palcust.rxIqTimeOffset.w(offset)
        calib_update();
    end
    
    addHelpEntry("Calibration",
                 "calib_set_tx_iq_time_offset",
                 "offset, initCalib(true)",
                 "",
                 "Set the Tx I/Q time offset.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_set_tx_iq_time_offset(offset, initCalib=true)
        calib_init() if(initCalib)
        raise "No HST_XCV context !" if(!@@HST_XCV)
        @@HST_XCV.palcust.txIqTimeOffset.w(offset)
        calib_update();
    end

    #===========================================#
    #               Get Functions               #
    #===========================================#
    
    
    #Get the timing 'id' of an RF chip.
    addHelpEntry("Calibration",
                 "calib_get_xcv_time",
                 "id, initCalib(true)",
                 "int16",
                 " Get the XCV id-th time parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_xcv_time(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        @@XCV.times[id].R
    end

    addHelpEntry("Calibration",
                 "calib_get_pa_time",
                 "id, initCalib(true)",
                 "int16",
                 " Get the PA id-th time parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_pa_time(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        @@PA.times[id].R
    end

    addHelpEntry("Calibration",
                 "calib_get_sw_time",
                 "id, initCalib(true)",
                 "int16",
                 " Get the SW id-th time parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_sw_time(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No SW context !" if(!@@SW)
        @@SW.times[id].R
    end

    addHelpEntry("Calibration",
                 "calib_get_pal_time",
                 "id, initCalib(true)",
                 "int16",
                 " Get the PAL id-th time parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_pal_time(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
        @@BB.times[id].R
    end

    #Get parameter 'id' of an RF chip.
    addHelpEntry("Calibration",
                 "calib_get_xcv_param",
                 "id, initCalib(true)",
                 "int32",
                 " Get the XCV id-th parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_xcv_param(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        @@XCV.param[id].R
    end

    addHelpEntry("Calibration",
                 "calib_get_pa_param",
                 "id, initCalib(true)",
                 "int32",
                 "Get the PA id-th parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_pa_param(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        @@PA.param[id].R
    end

    addHelpEntry("Calibration",
                 "calib_get_sw_param",
                 "id, initCalib(true)",
                 "int32",
                 " Get the SW id-th parameter.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_sw_param(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No SW context !" if(!@@SW)
        @@SW.param[id].R
    end
    
    #Get the value 'id' used to do the PA profile interpolation.
    addHelpEntry("Calibration",
                 "calib_get_pa_profile_interp",
                 "band,id, initCalib(true)",
                 "uint16",
                 " Get the id-th point of the PA profile for the given
                 band.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_pa_profile_interp(band,id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        if (@@C::CALIB_STUB_BAND_DCS1800 == band || @@C::CALIB_STUB_BAND_PCS1900 == band) then
            @@PA.palcust.profileInterpDp[id].R
        else
            # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            @@PA.palcust.profileInterpG[id].R
        end
    end
    
    #Get Rx-Tx frequency offset for any band.
    addHelpEntry("Calibration",
                 "calib_get_rxtx_freq_offset",
                 "band, initCalib(true)",
                 "int16",
                 "Get the Rx-Tx frequency offsets.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_rxtx_freq_offset(band, initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        @@XCV.palcust.rxTxFreqOffset[band].R
    end
    
    addHelpEntry("Calibration",
                 "calib_get_rxtx_time_offset",
                 "initCalib(true)",
                 "int16",
                 "Get Rx-Tx time offset.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_rxtx_time_offset(initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        @@XCV.palcust.rxTxTimeOffset.R
    end
    
    #Get Tx target power per PCL for any band.
    addHelpEntry("Calibration",
                 "calib_get_target_power_per_pcl",
                 "band,pclId, initCalib(true)",
                 "int16",
                 " Get the Tx target power per PCL (pclId) for the
                 band band.  
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_target_power_per_pcl(band,pclId, initCalib=true)
        calib_init() if(initCalib)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        
        #Read the current value from the global variable.
        case band
            when @@C::CALIB_STUB_BAND_PCS1900
                return @@TargetPclPowP[pclId];
            when @@C::CALIB_STUB_BAND_DCS1800
                return @@TargetPclPowD[pclId];
            else
                # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
                return @@TargetPclPowG[pclId];
        end
    end


    #Get Tx power per PCL per ARFCN for any band.
    addHelpEntry("Calibration",
                 "calib_get_power_per_pcl_per_arfcn",
                 "band,pclId,arfcn, initCalib(true)",
                 "uint16",
                 "Get Tx power per PCL per ARFCN for any band
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_power_per_pcl_per_arfcn(band,pclId,arfcn, initCalib=true)
        calib_init() if(initCalib)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        assert(arfcn >=0 && arfcn<=1, "ARFCN must be 0 or 1")
        raise "No PA context !" if(!@@PA)

        #Read the current value from the global variable.
        case band
            when @@C::CALIB_STUB_BAND_PCS1900
                @@PA.palcust.pcl2dbmArfcnP[pclId][arfcn].R

            when @@C::CALIB_STUB_BAND_DCS1800
                @@PA.palcust.pcl2dbmArfcnD[pclId][arfcn].R
            else
                # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
                @@PA.palcust.pcl2dbmArfcnG[pclId][arfcn].R
        end
    end

    #Get PA DAC profile for GSM or DCS-PCS.
    addHelpEntry("Calibration",
                 "calib_get_profile",
                 "band, id, initCalib(true)",
                 "uint16",
                 "Get PA DAC profile for a given band. The id-th 
                 parameter only is returned.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_profile(band,id, initCalib=true)
        calib_init() if(initCalib)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        raise "No PA context !" if(!@@PA)

        #Read the current values in the Target. 
        if (@@C::CALIB_STUB_BAND_DCS1800 == band ||  @@C::CALIB_STUB_BAND_PCS1900 == band) then
            @@PA.palcust.profileDp[id].R
        else
            # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            @@PA.palcust.profileG[id].R
        end
    end

    #Get PA DAC profile exterms for GSM or DCS-PCS.
    addHelpEntry("Calibration",
                 "calib_get_profile_extrems",
                 "band, initCalib(true)",
                 "uint16",
                 "Get PA DAC profile exterms for the given band.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_profile_extrems(band, initCalib=true)
        calib_init() if(initCalib)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        raise "No PA context !" if(!@@PA)
    
        #Read the current values in the Target.
        if (@@C::CALIB_STUB_BAND_DCS1800 == band ||  @@C::CALIB_STUB_BAND_PCS1900 == band) then
            min = @@PA.palcust.profileDbmMinDp.R
            max = @@PA.palcust.profileDbmMaxDp.R
        else
            # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            min = @@PA.palcust.profileDbmMinG.R
            max = @@PA.palcust.profileDbmMaxG.R
        end
        
        return [min,max]
    end
    
    
    #Get PA DAC ramp used for low or high power levels.
    addHelpEntry("Calibration",
                 "calib_get_ramp_low",
                 "id, initCalib(true)",
                 "uint16",
                 "Return the id-th value of the ramp for low power.
                 The first 16 values are the rising ramp.
                 The last 16 values are the falling ramp.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_ramp_low(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        assert(id>=0 && id < @@C::CALIB_PADAC_RAMP_QTY,"Id is not in good range");
        @@PA.palcust.rampLow[id].R
    end

    addHelpEntry("Calibration",
                 "calib_get_ramp_high",
                 "id, initCalib(true)",
                 "uint16",
                 "Return the id-th value of the ramp for high power.
                 The first 16 values are the rising ramp.
                 The last 16 values are the falling ramp.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_ramp_high(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        assert(id>=0 && id < @@C::CALIB_PADAC_RAMP_QTY,"Id is not in good range");
        @@PA.palcust.rampHigh[id].R
    end
    
    #Get PA DAC ramp switch from low to high power for GSM or DCS-PCS.
    addHelpEntry("Calibration",
                 "calib_get_ramp_switch",
                 "band, initCalib(true)",
                 "uint16",
                 "Get PA DAC ramp switch from low to high power for a given
                 band.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_ramp_switch(band, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        assert(band>=@@C::CALIB_STUB_BAND_GSM850 && band<@@C::CALIB_STUB_BAND_QTY,"Band must be between %d and %d" % [@@C::CALIB_STUB_BAND_GSM850, @@C::CALIB_STUB_BAND_QTY-1]);
        
        #Read the current values in the Target.
        if (@@C::CALIB_STUB_BAND_DCS1800 == band ||  @@C::CALIB_STUB_BAND_PCS1900 == band) then
            return @@PA.palcust.rampSwapDp.R
        else
            # @@C::CALIB_STUB_BAND_GSM850 or @@C::CALIB_STUB_BAND_GSM900
            return @@PA.palcust.rampSwapG.R
        end
    end

    #Get PA DAC low voltage limitations.
    addHelpEntry("Calibration",
                 "calib_get_low_volt",
                 "id, initCalib(true)",
                 "uint16",
                 "Get PA DAC low voltage limitations
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_low_volt(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        assert(id>=0 && id < @@C::CALIB_LOW_VOLT_QTY,"Id is not in good range");
        @@PA.palcust.lowVoltLimit[id].R
    end
    
    addHelpEntry("Calibration",
                 "calib_get_low_dac",
                 "id, initCalib(true)",
                 "uint16",
                 " Get PA DAC low DAC value limitation.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_low_dac(id, initCalib=true)
        calib_init() if(initCalib)
        raise "No PA context !" if(!@@PA)
        assert(id>=0 && id < @@C::CALIB_LOW_VOLT_QTY,"Id is not in good range");
        @@PA.palcust.lowDacLimit[id].R
    end
    
    #Get VoC Echo Cancelling parameters for any audio interface.
    addHelpEntry("Calibration",
                 "calib_get_voc_ec",
                 "itf, io, initCalib(true)",
                 "CH__CALIB_AUDIO_VOC_EC",
                 "Get VoC parameters for any IO set on any Audio Interface.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_voc_ec(itf, io, initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
        vocEc = CH__CALIB_AUDIO_VOC_EC.new(0)
    
        vocEc.ecMu.setl(@@BB.audio[itf].vocEc[io].ecMu.R)
        vocEc.ecRel.setl(@@BB.audio[itf].vocEc[io].ecRel.R)
        vocEc.ecMin.setl(@@BB.audio[itf].vocEc[io].ecMin.R)
        vocEc.esOn.setl(@@BB.audio[itf].vocEc[io].esOn.R)
        vocEc.esDtd.setl(@@BB.audio[itf].vocEc[io].esDtd.R)
        vocEc.esVad.setl(@@BB.audio[itf].vocEc[io].esVad.R)
        vocEc.enableField.setl(@@BB.audio[itf].vocEc[io].enableField.R)
        
        return vocEc
    end

    #Get VoC Filters parameters for any audio interface.
    addHelpEntry("Calibration",
                 "calib_get_voc_filters",
                 "itf, io, initCalib(true)",
                 "CH__CALIB_AUDIO_VOC_EC",
                 "Get VoC parameters for any IO set on any Audio Interface.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_voc_filters(itf, io, initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
        vocFilters = CH__CALIB_AUDIO_VOC_FILTERS.new(0)
    
        @@C::CALIB_VOC_MDF_QTY.times { |i| vocFilters.mdfFilter[i].setl(@@BB.audio[itf].vocFilters[io].mdfFilter[i].R) }
        @@C::CALIB_VOC_SDF_QTY.times { |i| vocFilters.sdfFilter[i].setl(@@BB.audio[itf].vocFilters[io].sdfFilter[i].R) }    
        return vocFilters
    end

    
    #Get gains for any audio interface.
    addHelpEntry("Calibration",
                 "calib_get_gains",
                 "itf, io, initCalib(true)",
                 "CH_CALIB_AUDIO_GAIN",
                 "Get gains for any IO set of any Audio Interface
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_gains(itf, io, initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
    
        gains = CH__CALIB_AUDIO_GAINS.new(0)
        gains.inGain.setl(@@BB.audio[itf].audioGains[io].inGain.R)
            
        @@C::CALIB_AUDIO_GAIN_QTY.times { |i| gains.outGain[i].setl(@@BB.audio[itf].audioGains[io].outGain[i].R) }
        @@C::CALIB_AUDIO_GAIN_QTY.times { |i| gains.sideTone[i].setl(@@BB.audio[itf].audioGains[io].sideTone[i].R) }    
        @@C::CALIB_AUDIO_GAIN_QTY.times { |i| gains.amplGain[i].setl(@@BB.audio[itf].audioGains[io].amplGain[i].R) }

        return gains
    end

    #Get GPADC bandgap.
    addHelpEntry("Calibration",
                 "calib_get_gpadc_bandgap",
                 "initCalib(true)",
                 "uint8",
                 "Get GPADC bandgap
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_gpadc_bandgap(initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
        @@BB.gpadc.bandgap.R
    end

    #Get GPADC sensor gains.
    addHelpEntry("Calibration",
                 "calib_get_gpadc_sensor",
                 "initCalib(true)",
                 "uint16, uint16",
                 "Get GPADC sensor gains.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_gpadc_sensor(initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
        return @@BB.gpadc.sensorGainA.R, @@BB.gpadc.sensorGainB.R
    end
    
    #Get RF analog parameters.
    addHelpEntry("Calibration",
                 "calib_get_rf_analog",
                 "initCalib(true)",
                 "CH__CALIB_RF_ANALOG",
                 "Get RF analog parameters.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_rf_analog(initCalib=true)
        calib_init() if(initCalib)
        raise "No BB context !" if(!@@BB)
        rf_analog = CH__CALIB_RF_ANALOG.new(0)
        rf_analog.txRefVolt.setl(@@BB.rfAnalog.txRefVolt.R)
        rf_analog.txDacCurQ.setl(@@BB.rfAnalog.txDacCurQ.R)
        rf_analog.txDacCurI.setl(@@BB.rfAnalog.txDacCurI.R)
        rf_analog.rxRefCal.setl(@@BB.rfAnalog.rxRefCal.R)
        return rf_analog
    end
    
    #Get I/Q time offset.   
    addHelpEntry("Calibration",
                 "calib_get_rx_iq_time_offset",
                 "initCalib(true)",
                 "int16",
                 "Get Rx I/Q time offset
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_rx_iq_time_offset(initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        @@XCV.palcust.rxIqTimeOffset.R
    end
    
    addHelpEntry("Calibration",
                 "calib_get_tx_iq_time_offset",
                 "initCalib(true)",
                 "int16",
                 "Get Tx I/Q time offset
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable with both the Stub and the Daemon.
                ")
    def calib_get_tx_iq_time_offset(initCalib=true)
        calib_init() if(initCalib)
        raise "No XCV context !" if(!@@XCV)
        @@XCV.palcust.txIqTimeOffset.R      
    end
    
    #===========================================#
    #               Mode Functions              #
    #===========================================#

    #Set the Target in stop mode.
    addHelpEntry("Calibration",
                 "calib_mode_stop",
                 "initCalib(true)",
                 "",
                 " Stop Ces Activity
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_stop(initCalib=true)
        calib_init() if(initCalib)
        calib_send_mode(@@C::CALIB_STUB_NO_STATE)
    end 
    
    #Set the Target in the rx monitor frequency mode
    addHelpEntry("Calibration",
                 "calib_mode_rx_freq",
                 "band,arfcn,expPow, initCalib(true)",
                 "",
                 "Set the Target in the rx monitor frequency mode.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_rx_freq(band,arfcn,expPow, initCalib=true)
        calib_init() if(initCalib)
        calib_mode_rx(band,arfcn,expPow,@@C::CALIB_STUB_FOF_STATE)
    end

    #Set the Target in the rx monitor power mode
    addHelpEntry("Calibration",
                 "calib_mode_rx_power",
                 "band,arfcn,expPow, initCalib(true)",
                 "",
                 "Set the Target in the rx monitor power mode
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_rx_power(band,arfcn,expPow, initCalib=true)
        calib_init() if(initCalib)
        calib_mode_rx(band,arfcn,expPow,@@C::CALIB_STUB_MONIT_STATE)
    end
    
    #Set the Target in the rx monitor statistics mode
    addHelpEntry("Calibration",
                 "calib_mode_rx_stat",
                 "band,arfcn,expPow, initCalib(true)",
                 "",
                 "Set the Target in the rx monitor statistics mode
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_rx_stat(band,arfcn,expPow, initCalib=true)
        calib_init() if(initCalib)
        calib_mode_rx(band,arfcn,expPow,@@C::CALIB_STUB_SYNCH_STATE)
    end
    
    #Set the Target in the Tx send PCL bursts mode
    addHelpEntry("Calibration",
                 "calib_mode_tx_pcl",
                 "band,arfcn,pcl,tsc, initCalib(true)",
                 "",
                 "Set the Target in the Tx send PCL bursts mode
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_tx_pcl(band,arfcn,pcl,tsc, initCalib=true)
        calib_init() if(initCalib)
        calib_mode_tx(band,arfcn,pcl,0,tsc,@@C::CALIB_STUB_TX_STATE)
    end
    
    #Set the Target in the Tx send PADAC index bursts mode
    addHelpEntry("Calibration",
                 "calib_mode_tx_pa_idx",
                 "band,arfcn,dacId,tsc, initCalib(true)",
                 "",
                 "Set the Target in the Tx send PADAC index bursts mode
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_tx_pa_idx(band,arfcn,dacId,tsc, initCalib=true)
        calib_init() if(initCalib)
        #The bit 15 is set to 0 to say to the XCV that the u16 is a DAC index and not a DAC value
        calib_mode_tx(band,arfcn,0,dacId,tsc,@@C::CALIB_STUB_PA_STATE)
    end
    
    #Set the Target in the Tx send PADAC value bursts mode
    addHelpEntry("Calibration",
                 "calib_mode_tx_pa_val",
                 "band,arfcn,dacVal,tsc, initCalib(true)",
                 "",
                 "Set the Target in the Tx send PADAC value bursts mode
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_tx_pa_val(band,arfcn,dacVal,tsc, initCalib=true)
        calib_init() if(initCalib)
        #The bit 15 is set to 1 to say to the XCV that the UINT16 is a DAC value and not a DAC index
        calib_mode_tx(band,arfcn,0,dacVal | (1<<15),tsc,@@C::CALIB_STUB_PA_STATE)
    end
    
    #Set the Target in the Audio sinewave output mode.
    addHelpEntry("Calibration",
                 "calib_mode_audio_out",
                 "itf, outSel, outGain, polyGain,
                  outFreq,outAmpl, initCalib(true)",
                 "",
                 "Set the Target in the Audio sinewave output mode.
                 <br>itf is the audio interface to use
                 <br>outSel is the output to use
                 <br>outGain is the speaker gain
                 <br>polyGain is the gain of the polyphonic chip
                 <br>outFreq is the frequency to emit.
                 <br>outAmp is TODO
                 <br>initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_mode_audio_out(itf, outSel,outGain,polyGain,outFreq,outAmpl, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        
        #Send the parameters to the Target
        @@CTX.itfSel.w(itf)
        @@CTX.outSel.w(outSel)
        @@CTX.outGain.w(outGain)
        @@CTX.polyGain.w(polyGain)
        @@CTX.outFreq.w(outFreq)
        @@CTX.outAmpl.w(outAmpl)
        
        #Send the command to CES
        calib_send_mode(@@C::CALIB_STUB_AUDIO_OUT)
    end

=begin
    #Set the Target in the Audio side loop mode.
    addHelpEntry("Calibration",
                 "calib_mode_audio_side",
                 "itf, inSel, inGain, outSel, outGain, 
                  polyGain,sideGain, initCalib(true)",
                 "",
                 "Set the Target in the Audio side loop mode.
                 <br>itf is the audio interface to use
                 <br>outSel is the output to use
                 <br>outGain is the speaker gain
                 <br>inSel is the input to use
                 <br>inGain is the microphone gain
                 <br>polyGain is the gain of the polyphonic chip
                 <br>sideGain is the sideTone gain to use.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                ")
    def calib_mode_audio_side(itf, inSel,inGain,outSel,outGain,polyGain,sideGain, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        
        #Send the parameters to the Target
        @@CTX.itfSel.w(itf)
        @@CTX.inSel.w(inSel)
        @@CTX.inGain.w(inGain)
        @@CTX.outSel.w(outSel)
        @@CTX.outGain.w(outGain)
        @@CTX.polyGain.w(polyGain)
        @@CTX.sideGain.w(sideGain)
        
        #Send the mode command to CES
        calib_send_mode(@@C::CALIB_AUDIO_SIDE)
            
    end
=end

    #Set the crystal frequency offset, wait for it to be handled and return the status of the process.
    addHelpEntry("Calibration",
                 "calib_set_xtal_freq_offset",
                 "xtalFreqOffset, initCalib(true)",
                 "bool",
                 "Set the crystal frequency offset, wait for it to be handled
                 and return the status of the process.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform, and used recommanded 
                through the higher level calib_auto_crystal_calibration function.
                ")
    def calib_set_xtal_freq_offset(xtalFreqOffset, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)

        #Set the result of the calibration to pending.
        @@CTX.xtalCalibDone.w(@@C::CALIB_PROCESS_PENDING)
    
        #Send the parameters to Target.
        @@CTX.setXtalFreqOffset.w(xtalFreqOffset.to_i)
    
        #Wait for the result of the crystal calibration decision.
        xtalCalibDone = @@C::CALIB_PROCESS_PENDING
        while(xtalCalibDone == @@C::CALIB_PROCESS_PENDING) do
            xtalCalibDone = @@CTX.xtalCalibDone.R
        end

        #The result given by CES will say to the tools wether the
        #calibration of the crystal must continue or must stop.
        if(xtalCalibDone == @@C::CALIB_PROCESS_STOP) then
            return true
        elsif(xtalCalibDone == @@C::CALIB_PROCESS_CONTINUE)
            return false
        else
            raise "An error occured during the calib of the XTAL"
        end
    end

    #Set the AFC bound and wait for it to be handled.
    addHelpEntry("Calibration",
                 "calib_set_afc_bound",
                 "afcBound, initCalib(true)",
                 "",
                 "Set the AFC bound and wait for it to be handled. (uint8)
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                ")
    def calib_set_afc_bound(afcBound, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        @@CTX.setAfcBound.w(afcBound)
    end
    #Set the AFC bound and wait for it to be handled.
    addHelpEntry("Calibration",
                 "calib_set_changedacafc_bound",
                 "afcBound, initCalib(true)",
                 "",
                 "Set the AFC bound and wait for it to be handled. (uint8)
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                ")
    def calib_set_changedacafc_bound(afcBound, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        @@CTX.setChangeDACAfcBound.w(afcBound)
    end
    #Set the AFC frequency offset.
    addHelpEntry("Calibration",
                 "calib_set_changedacafc_freq_offset",
                 "afcFreqOffset, initCalib(true)",
                 "",
                 "Set the AFC frequency offset (int32)
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_changedacafc_freq_offset(afcFreqOffset, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        @@CTX.setChangeDACAfcFreqOffset.w(afcFreqOffset.to_i)
        # Apply the change for further use immediatly if in "manual" mode.
        calib_update() if(initCalib)
    end
        #Set the AFC frequency offset.
    addHelpEntry("Calibration",
                 "calib_set_changedacafc_midfreq_offset",
                 "afcFreqOffset, initCalib(true)",
                 "",
                 "Set the AFC frequency offset (int32)
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_changedacafc_midfreq_offset(afcFreqOffset, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        puts "%d" % afcFreqOffset
        @@CTX.setChangeDACAfcFreq.w(afcFreqOffset.to_i)
        # Apply the change for further use immediatly if in "manual" mode.
        calib_update() if(initCalib)
    end
    #Set the AFC frequency offset.
    addHelpEntry("Calibration",
                 "calib_set_afc_freq_offset",
                 "afcFreqOffset, initCalib(true)",
                 "",
                 "Set the AFC frequency offset (int32)
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_afc_freq_offset(afcFreqOffset, initCalib=true)
        calib_init() if(initCalib)
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        @@CTX.setAfcFreqOffset.w(afcFreqOffset.to_i)
        # Apply the change for further use immediatly if in "manual" mode.
        calib_update() if(initCalib)
    end
    

    # Set the insertion loss offset.
    # Return nextArfcn and if the process must stop.
    addHelpEntry("Calibration",
                 "calib_set_iloss_offset",
                 "iLossOffset, initCalib(true)",
                 "uint16, bool",
                 "Set the insertion loss offset (int8).
                 Return nextArfcn and if the process must stop.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_iloss_offset(iLossOffset, initCalib=true)
        calib_init() if(initCalib)
        # Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        
        # Set the result of the calibration to pending.
        @@CTX.iLossCalibDone.w(@@C::CALIB_PROCESS_PENDING)
    
        # Send the parameters to the Target.
        @@CTX.setILossOffset.w(iLossOffset)
    
        # Wait for the result of the insertion loss calibration decision.
        iLossCalibDone = @@C::CALIB_PROCESS_PENDING
        while (iLossCalibDone == @@C::CALIB_PROCESS_PENDING)  do
            iLossCalibDone = @@CTX.iLossCalibDone.R
        end
    
        # Get the next ARFCN to program, from CES
        iLossNextArfcn = @@CTX.iLossNextArfcn.R
        
        # The result given by JCES will say to the tools wether the
        # calibration of the crystal must continue or must stop.
        if (iLossCalibDone == @@C::CALIB_PROCESS_STOP)
            return iLossNextArfcn, TRUE
        elsif (iLossCalibDone == @@C::CALIB_PROCESS_CONTINUE)
            return iLossNextArfcn, FALSE
        else
            # An error occured.
            raise "An error occured during the iLoss calibration process"
        end
    end
    
    #Set a measurement for the PA profile calculation, wait for
    #it to be handled and return the status of the process.
    addHelpEntry("Calibration",
                 "calib_set_pa_profile_meas",
                 "band,measPower, initCalib(true)",
                 "uint16, CALIB_H_ENUM_0", #TODO Name the calib process status enum
                 "Set a measurement for the PA profile calculation,
                 wait for it to be handled and return the status 
                 of the process.
                 The returned values are the next dac val and the 
                 calibration status.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_pa_profile_meas(band,measPower, initCalib=true)
        calib_init() if(initCalib)
            
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)

        #Set the result of the calibration to pending.
        @@CTX.paProfCalibDone.w(@@C::CALIB_PROCESS_PENDING)
        
        #Send the parameters to the Target.
        @@CTX.txBand.w(band)
        @@CTX.setPAProfMeas.w(measPower)
    
        #Wait for the result of the crystal calibration decision.
        paProfCalibDone = @@C::CALIB_PROCESS_PENDING
        while (paProfCalibDone == @@C::CALIB_PROCESS_PENDING) do
            paProfCalibDone = @@CTX.paProfCalibDone.R
        end
        
        #Get the next PA DAC to program, from CES.
        paProfNextDacVal = @@CTX.paProfNextDacVal.R

        #The result given by CES will say to the tools wether the
        #calibration of the crystal must continue or must stop.
        case paProfCalibDone
            when @@C::CALIB_PROCESS_STOP
                return [paProfNextDacVal,@@C::CALIB_PROCESS_STOP]
            when @@C::CALIB_PROCESS_CONTINUE
                return [paProfNextDacVal,@@C::CALIB_PROCESS_CONTINUE]
            when @@C::CALIB_PROCESS_NEED_CALM
                return [paProfNextDacVal,@@C::CALIB_PROCESS_NEED_CALM]
            #Error management.
            when @@C::CALIB_PROCESS_ERR_BAD_POW
                raise "Bad Power Value !"
            when @@C::CALIB_PROCESS_ERR_NO_MONO_POW
                raise "No Mono Power Value !"
            when @@C::CALIB_PROCESS_ERR_ZERO_DAC
                raise "No Zero Dac Value !"
            else
                raise "Pa prof calibration unknown error !"
        end
        
    end
    
    # Start the PMD custom calibration process.
    # Return the status sent by the PMD custom calibration function.
    addHelpEntry("Calibration",
                 "calib_set_pmd_custom",
                 "customCommand, initCalib(true)",
                 "uint32",
                 "Start the PMD custom calibration process.
                 <br>customCommand Value passed to the PMD calibration function.
                 Must be different from zero to start the PMD custom calibration.
                 <br>initCalib is an optional parameter. If true 
                 (default) a calib_init is done before executing the script code.
                 Therefore, this parameter only needs to be set to false for higher
                 level scripts doing explicitly a calib init.
                 Only usable with the calibration Stub running on the platform.
                 <br>Return the status sent by the PMD custom calibration function.
                ")
    def calib_set_pmd_custom(customCommand=1, initCalib=true)
        calib_init() if(initCalib)
        # Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
    
        # Send the parameters to the Target.
        @@CTX.pmdCustomCalibStart.w(customCommand)
    
        # Wait for the result of the PMD calibration process.
        pmdCustomCalibStart = customCommand
        while (pmdCustomCalibStart != 0)  do
            pmdCustomCalibStart = @@CTX.pmdCustomCalibStart.R
        end
    
        # Get the status of the PMD calibration.
        return @@CTX.pmdCustomCalibStatus.R
    end

    #Send a calib update values request and wait for it to be done.
    addHelpEntry("Calibration",
                 "calib_set_update_values",
                 "time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib(true)",
                 "",
                 "Send a calib update values request and wait for it to be done.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_update_values(time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib=true)
        calib_init() if(initCalib)
    
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
    
        #Send the parameters to the Target, any none null value will do it.
        @@CTX.setCalibUpdate.w(0x1)

        begin
            Timeout::timeout(time_out) {
                #Wait for the command to be executed
                calibUpdateStatus = 1
                while(calibUpdateStatus!=0) do
                    calibUpdateStatus = @@CTX.setCalibUpdate.R
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowlege never received!"
        end
    end
    
    #Send a audio dictaphone command. This can be used when a "mode"
    #is running, but just don't use it when an "audio mode" is running.
    addHelpEntry("Calibration",
                 "calib_set_dictaphone_record",
                 "itf, inSel, inGain, 
                  time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT,
                 initCalib(true)",
                 "",
                 "Send a audio dictaphone command. This can be used
                 when a \"mode\" is running, but just don't use it
                 when an \"audio mode\" is running.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_dictaphone_record(itf, inSel,inGain,time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib=true)
        calib_init() if(initCalib)
        
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        
        @@CTX.itfSel.w(itf) 
        @@CTX.inSel.w(inSel)
        @@CTX.inGain.w(inGain)
    
        #Send the parameters to the Target.
        @@CTX.startAudioDictaphone.w(@@C::CALIB_AUDIO_DICTA_REC)

        begin
            Timeout::timeout(time_out) {
                #Wait for the command to be executed
                startAudioDictaphone = 1
                while(startAudioDictaphone != 0) do
                    startAudioDictaphone = @@CTX.startAudioDictaphone.R
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowlege never received!"
        end     
    end
    
    addHelpEntry("Calibration",
                 "calib_set_dictaphone_playback",
                 "itf, outSel,outGain,polyGain,time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib(true)",
                 "",
                 "
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_set_dictaphone_playback(itf, outSel,outGain,polyGain,time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib=true)
        calib_init() if(initCalib)
        
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)

        #Send the parameters to the Target.
        @@CTX.itfSel.w(itf) 
        @@CTX.outSel.w(outSel)
        @@CTX.outGain.w(outGain)
        @@CTX.polyGain.w(polyGain)
    
        #Send the parameters to the Target.
        @@CTX.startAudioDictaphone.w(@@C::CALIB_AUDIO_DICTA_PLAY)

        begin
            Timeout::timeout(time_out) {
                #Wait for the command to be executed
                startAudioDictaphone = 1
                while(startAudioDictaphone != 0) do
                    startAudioDictaphone = @@CTX.startAudioDictaphone.R
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowlege never received!"
        end             
    end
    
    #Get measured Rx FOf.
    addHelpEntry("Calibration",
                 "calib_get_rx_fof",
                 "initCalib(true)",
                 "int32",
                 "Get measured Rx FOf
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_rx_fof(initCalib=true)
        calib_init() if(initCalib)
        raise "No CES context !" if(!@@CTX)
        @@CTX.meanFOf.R
    end
    

    #Get Rx monitoring power.
    addHelpEntry("Calibration",
                 "calib_get_rx_mon_power",
                 "initCalib(true)",
                 "uint8",
                 "Get Rx monitoring power.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_rx_mon_power(initCalib=true)
        calib_init() if(initCalib)
        raise "No CES context !" if(!@@CTX)
        @@CTX.monPower.R
    end
    
    
    #Get Rx normal burst power.
    addHelpEntry("Calibration",
                 "calib_get_rx_nb_power",
                 "initCalib(true)",
                 "uint8",
                 "Get Rx normal burst power
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_rx_nb_power(initCalib=true)
        calib_init() if(initCalib)
        raise "No CES context !" if(!@@CTX)
        @@CTX.nbPower.R
    end
    
    #Get measured Rx statistics.
    addHelpEntry("Calibration",
                 "calib_get_rx_stat",
                 "initCalib(true)",
                 "uint16, uint8, uint8",
                 "Get measured Rx statistics.
                 <br> Returns snr, bitError, power
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_rx_stat(initCalib=true)
        calib_init() if(initCalib)
        raise "No CES context !" if(!@@CTX)
        snr         = @@CTX.snR.R
        bitError    = @@CTX.bitError.R
        power       = @@CTX.nbPower.R
        
        return [snr,bitError,power]
    end
    
    #Get measured GPADC value for a given channel. 
    addHelpEntry("Calibration",
                 "calib_get_gpadc_value_ch",
                 "chNumber,time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib(true)",
                 "uint8",
                 "Get measured GPADC value for a given channel.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_gpadc_value_ch(chNumber,time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib=true)
        calib_init() if(initCalib)

        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)

        #Send the parameters to the Target, any none null value will do it.
        @@CTX.setRestartGpadcMeasure.w(0x1)

        begin
            Timeout::timeout(time_out) {
                gpadcMeasStatus = 1
                #Wait for the command to be executed
                while(gpadcMeasStatus != 0) do
                    gpadcMeasStatus = @@CTX.setRestartGpadcMeasure.R
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowlege never received!"
        end             

        # This needs to be here, because the variable "measure" is used
        # outside the scope of the following loop.
        measure = @@C::CALIB_STUB_GPADC_IDLE
        
        begin
            Timeout::timeout(time_out) {
                #Wait for the command to be executed
                while(measure == @@C::CALIB_STUB_GPADC_IDLE) do
                    measure = @@CTX.gpadcAver[chNumber].R
                    #The DC offset fields have not been initialized, it means the 
                    #DC offset cannot be measured. 
                    raise "GPADC has not been initialized !!" if(measure == @@C::CALIB_STUB_GPADC_ERROR)
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowlege never received!"
        end     
                
        return measure
        
    end

    #Get measured GPADC values.
    addHelpEntry("Calibration",
                 "calib_get_gpadc_values",
                 "initCalib(true)",
                 "uint8, uint8, uint8, uint8",
                 "Get measured GPADC values.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_gpadc_values(initCalib=true)
        calib_init() if(initCalib)
        meas = []
        4.times { |chan| meas[chan] = calib_get_gpadc_value_ch(chan) }
        return meas
    end
    
    #Get measured I and Q DC offsets. 
    addHelpEntry("Calibration",
                 "calib_get_iq_dc_offsets",
                 "time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib(true)",
                 "int16, int16",
                 "Get measured I and Q DC offsets.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Only usable with the calibration Stub running on the platform.
                ")
    def calib_get_iq_dc_offsets(time_out=CALIB_SEND_CMD_DEFAULT_TIMEOUT, initCalib=true)
        calib_init() if(initCalib)
    
        #Don't do it when using the calibrable Proto.
        raise "No CES context !" if(!@@CTX)
        
        locDcoI = @@C::CALIB_DCO_IDLE
        locDcoQ = @@C::CALIB_DCO_IDLE
        
        begin
            Timeout::timeout(time_out) {
                while(locDcoI == @@C::CALIB_DCO_IDLE || locDcoQ == @@C::CALIB_DCO_IDLE) do
                    
                    locDcoI = @@CTX.dcoAverI.R
                    locDcoQ = @@CTX.dcoAverQ.R
                    
                    raise "DC offset fields have not been initialized!!" if(locDcoI == @@C::CALIB_DCO_ERROR || locDcoQ == @@C::CALIB_DCO_ERROR)
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Command acknowlege never received!"
        end             
        
        return [locDcoI,locDcoQ]
    end
    
    #Reset the calib parameters to default values. 
    addHelpEntry("Calibration",
                 "calib_reset_parameters",
                 "initCalib(true), rf(true), audio(false)",
                 "",
                 "Reset the calib parameters to default values
                 <br>initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                <br>rf tells if to reset RF parameters.
                <br>audio tells if to reset audio parameters.
                This command is a Daemon command, therefore usable by both the calib
                daemon and the calib stub.
                ")
    def calib_reset_parameters(initCalib=true, rf=true, audio=false)
        calib_init() if(initCalib)
        #Stop CES activity before executing the command to avoid
        #a FINT re-entrance assert. 
        calib_mode_stop()
        
        mark = CALIB_MARK_MASK(@@CODE_VERSION)
        if(mark == @@C::CALIB_MARK_VERSION && @@CODE_VERSION < (@@C::CALIB_MARK_VERSION|0x0205))
            # Code versions prior to V02.05 only recognize RF commands
            rf = true;
            audio = false;
        end
        
        #Send the calibration command to the Target and
        #wait for the parameters to be rested.
        if (rf && audio)
            calib_send_cmd(@@C::CALIB_CMD_RESET)
        elsif (rf)
            calib_send_cmd(@@C::CALIB_CMD_RF_RESET)
        elsif (audio)
            calib_send_cmd(@@C::CALIB_CMD_AUDIO_RESET)
        end
    end

    #Erase the calibrated values in flash.
    addHelpEntry("Calibration",
                 "calib_erase_flash",
                 "initCalib(true)",
                 "",
                 "Erase the calibrated values in flash.
                (ie, erase the calib flash sector)
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                This command is a Daemon command, therefore usable by both the calib
                daemon and the calib stub.
                ")
    def calib_erase_flash(initCalib=true)
        calib_init() if(initCalib)  
        #Stop CES activity before executing the command to avoid
        #a FINT re-entrance assert.
        calib_mode_stop()
        
        #Send the calibration command to the Target and
        #wait for the flash to be erased.
        calib_send_cmd(@@C::CALIB_CMD_FLASH_ERASE)
        puts "Done."
    end
    
    #Burn the calibrated values in flash.
    addHelpEntry("Calibration",
                 "calib_burn_flash",
                 "automaticTool(false), initCalib(true), rf(true), audio(false)",
                 "",
                 "Burn the calibrated values in flash.
                 <br>automaticTool tells if the calibration has been done by the
                 automatic tool (true) or manually (false).
                 <br>initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                <br>rf tells if to burn RF parameters.
                <br>audio tells if to burn audio parameters.
                This command is a Daemon command, therefore usable by both the calib
                daemon and the calib stub.
                ")
    def calib_burn_flash(automaticTool=false, initCalib=true, rf=true, audio=false)
        calib_init() if(initCalib)
        #Stop CES activity before executing the command to avoid
        #a FINT re-entrance assert.
        calib_mode_stop()
        
        #Set the calibration method and time.
        calib_set_method(automaticTool)
        
        mark = CALIB_MARK_MASK(@@CODE_VERSION)
        if(mark == @@C::CALIB_MARK_VERSION && @@CODE_VERSION < (@@C::CALIB_MARK_VERSION|0x0205))
            # Code versions prior to V02.05 only recognize RF commands
            rf = true;
            audio = false;
        end
        
        #Send the calibration command to the Target and wait for the flash
        #to be programmed.
        if (rf && audio)
            calib_send_cmd(@@C::CALIB_CMD_FLASH_BURN)
            puts "RF and audio calib data have been burnt."
        elsif (rf)
            calib_send_cmd(@@C::CALIB_CMD_RF_FLASH_BURN)
            puts "RF calib data has been burnt."
        elsif (audio)
            calib_send_cmd(@@C::CALIB_CMD_AUDIO_FLASH_BURN)
            puts "Audio calib data has been burnt."
        end
    end
    
    # FIXME Find a real Name
    # Convert a byte array into a 32-bits word array.
    # FIXME This function must disappear, for it is not optimal to use it.
    def byteToWord(byteArray)
        raise "Not a multiple of 4 bytes array" if ((byteArray.size%4)!=0)
        wordArray = []
        (byteArray.size/4).times {|i| wordArray[i] = (byteArray[4*i+0] << 0 | byteArray[4*i+1] << 8 | byteArray[4*i+2] << 16 | byteArray[4*i+3] << 24)
        }
        return wordArray
    end

    #Dump the calibrated values from FLASH to a file.
    addHelpEntry("Calibration",
                 "calib_dump_flash_parameters",
                 "filename,initCalib(true)",
                 "",
                 "Dump the calibrated values from FLASH to a file.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable by both the calib daemon and the calib stub.
                ")
    def calib_dump_flash_parameters(filename,initCalib=true)
        calib_init() if(initCalib) 

        flashVersion = @@FLASH.versionTag.RU
        if (@@C::CALIB_VERSION_NUMBER != flashVersion)
            puts "html><font color=red><b>WARNING: </b>Version mismatched!</font>"
            puts "Tool Version: 0x%08X" % @@C::CALIB_VERSION_NUMBER
            puts "Flash Calib Version: 0x%08X" % flashVersion
            puts ""
        end

        puts "Saving calib data from FLASH with version: 0x%08X ..." % flashVersion

        #Open the file where to dump.
        df = File.open(filename, "wb")

        #Dump the address of the calibration sector.
        df.lodWriteAddress( (~0x80000000) & @@FLASH.address() )

    if (@@C::CALIB_VERSION_NUMBER < (@@C::CALIB_MARK_VERSION|0x0207)) 

        #Dump the calibration sector buffer.
        data = byteToWord(RB(@@FLASH.address(), @@FLASH.sizeof()))
        structSize = (@@FLASH.sizeof() + 3) / 4 
        structSize.times { |i| df.lodWriteWord32(data[i]) }
        df.lodWriteWord32(structSize + 1)

    else
        if(chipHasAp?()) 
            #Dump the rf and old audio calibration sector buffer.
            data0 = byteToWord(RB(@@FLASH.address() + 0x2000,0x2000))
            structSize = (0x2000 + 3) / 4
            structSize.times { |i| df.lodWriteWord32(data0[i]) }

            #Dump the modem_factory_data and ap_factory_data sector buffer.
            data1 = byteToWord(RB(@@FLASH.address() + 0x4000,0x2000))
            structSize = (0x2000 + 3) / 4
            structSize.times { |i| df.lodWriteWord32(data1[i]) }

            #Dump the new audio and RF calibration sector buffer.
            data2 = byteToWord(RB(@@FLASH.address() - 0x2000,0x4000))
            structSize = (0x4000 + 3) / 4
            structSize.times { |i| df.lodWriteWord32(data2[i]) }

        else
            #Dump the calibration sector buffer.
            data = byteToWord(RB(@@FLASH.address(), @@FLASH.sizeof()))
            structSize = (@@FLASH.sizeof() + 3) / 4 
            structSize.times { |i| df.lodWriteWord32(data[i]) }
        end 
    end

        #Useless tag for the calibration, but it is useful to check
        #the size fo the sector manually manually. 
        #df.lodWriteWord32(structSize + 1)

        df.close()

        puts "Done."
    end

    #Dump the calibrated values from RAM to a file.
    addHelpEntry("Calibration",
                 "calib_dump_parameters",
                 "filename,automaticTool,initCalib(true)",
                 "",
                 "Dump the calibrated values from RAM to a file.
                 automaticTool is to be set to true if these data 
                 have been made by the automatic mode of the calibration.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable by both the calib daemon and the calib stub.
                ")
    def calib_dump_parameters(filename,automaticTool,initCalib=true)
        if (@@C::CALIB_VERSION_NUMBER < (@@C::CALIB_MARK_VERSION|0x0205))
            # Tool versions prior to V02.05
            raise "Software map is too old. Calib Tool Version: 0x%08x" % @@C::CALIB_VERSION_NUMBER
        end

        calib_init() if(initCalib)

        if (@@C::CALIB_VERSION_NUMBER != @@CODE_VERSION)
            puts "html><font color=red><b>WARNING: </b>Version mismatched!</font>"
            puts "Tool Version: 0x%08X" % @@C::CALIB_VERSION_NUMBER
            puts "Code Version: 0x%08X" % @@CODE_VERSION
            puts ""
        end

        puts "Saving calib data from RAM with version: 0x%08X ..." % @@CODE_VERSION

        #Open the file where to dump.
        df = File.open(filename, "wb")

        #Set the calibration method and time only in automatic mode.
        calib_set_method(automaticTool) if(automaticTool)

        #Dump the address of the calibration sector.
        df.lodWriteAddress( (~0x80000000) & @@FLASH.address() )

        $cfpCalibStruct = CH__CALIB_BUFFER.new(0)

        $cfpCalibStruct.versionTag.setl(@@CODE_VERSION)

        ####Dump buffers
        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206))        
            $cfpCalibStruct.audio_voc.memsetl(RB(@@VOC.address(),@@VOC.sizeof()))
            $cfpCalibStruct.audio_music.memsetl(RB(@@MUSIC.address(),@@MUSIC.sizeof()))
        end 

        $cfpCalibStruct.opInfo.memsetl(RB(@@OP_INFO.address(),@@OP_INFO.sizeof()))

        $cfpCalibStruct.xcv.memsetl(RB(@@XCV.address(),@@XCV.sizeof()))

        $cfpCalibStruct.pa.memsetl(RB(@@PA.address(),@@PA.sizeof()))

        $cfpCalibStruct.sw.memsetl(RB(@@SW.address(),@@SW.sizeof()))

        $cfpCalibStruct.bb.memsetl(RB(@@BB.address(),@@BB.sizeof()))

        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206)) 
            $cfpCalibStruct.iirParam.memsetl(RB(@@IIR.address(),@@IIR.sizeof()))
        end

        if (CALIB_MARK_MASK(@@CODE_VERSION) == @@C::CALIB_MARK_VERSION && @@CODE_VERSION < (@@C::CALIB_MARK_VERSION|0x0205))
            # Code versions prior to V02.05
            bbTail = byteToWord(RB(@@BB.address()+@@BB.sizeof(), 16))

            $cfpCalibStruct.reserved[0].setl(bbTail[0])

            $cfpCalibStruct.reserved[1].setl(bbTail[1])

            $cfpCalibStruct.audioVersionTag.setl(bbTail[2])

            $cfpCalibStruct.rfCrc.setl(bbTail[3])
        else
            # Code versions at V02.05 or later
            $cfpCalibStruct.reserved[0].setl(@@FLASH.reserved[0].RU)

            $cfpCalibStruct.reserved[1].setl(@@FLASH.reserved[0].RU)

            $cfpCalibStruct.audioVersionTag.setl(@@CODE_VERSION)

            $cfpCalibStruct.rfCrc.setl(@@FLASH.rfCrc.RU)

            if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206)) 
                $cfpCalibStruct.audioExpand1VersionTag.setl(@@FLASH_NEWAUDIO_VER)
            end
        end

        #Dump the calibration sector buffer.

        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206))
            if(chipHasAp?())         
                #Dump the rf and old audio calibration sector buffer.
                df.lodWriteWord32(@@CODE_VERSION)

                data0 = byteToWord($cfpCalibStruct.opInfo.memgetl())
                structSize = (@@OP_INFO.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data0[i]) }

                data1 = byteToWord($cfpCalibStruct.xcv.memgetl())
                structSize = (@@XCV.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data1[i]) }

                data2 = byteToWord($cfpCalibStruct.pa.memgetl())
                structSize = (@@PA.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data2[i]) }

                data3 = byteToWord($cfpCalibStruct.sw.memgetl())
                structSize = (@@SW.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data3[i]) }

                data4 = byteToWord($cfpCalibStruct.bb.memgetl())
                structSize = (@@BB.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data4[i]) }

                df.lodWriteWord32(@@FLASH.reserved[0].RU)
                df.lodWriteWord32(@@FLASH.reserved[1].RU)
                df.lodWriteWord32(@@CODE_VERSION)
                df.lodWriteWord32(@@FLASH.rfCrc.RU)

                data5 = byteToWord($cfpCalibStruct.iirParam.memgetl())
                structSize = (@@IIR.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data5[i]) }

                totalsize = 0x4 + @@OP_INFO.sizeof() + @@XCV.sizeof() + @@PA.sizeof() + @@SW.sizeof() + @@BB.sizeof() + 0x10 + @@IIR.sizeof()
                data6 = byteToWord(RB((@@FLASH.address() +0x2000 + totalsize) ,(0x2000 - totalsize)))
                structSize = ((0x2000 - totalsize) + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data6[i]) }

                #Dump the modem_factory_data and ap_factory_data sector buffer.
                data7 = byteToWord(RB(@@FLASH.address() + 0x4000,0x2000))
                structSize = (0x2000 + 3) / 4
                structSize.times { |i| df.lodWriteWord32(data7[i]) }

                #Dump the new RF calibration sector buffer.
                data8 = byteToWord(RB(@@FLASH.address() - 0x2000,0x2000))
                structSize = (0x2000 + 3) / 4
                structSize.times { |i| df.lodWriteWord32(data8[i]) }

                #Dump the new audio calibration sector buffer.
                df.lodWriteWord32(@@FLASH_NEWAUDIO_VER)

                data9 = byteToWord($cfpCalibStruct.audio_voc.memgetl())
                structSize = (@@VOC.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data9[i]) }

                data10 = byteToWord($cfpCalibStruct.audio_music.memgetl())
                structSize = (@@MUSIC.sizeof() + 3) / 4 
                structSize.times { |i| df.lodWriteWord32(data10[i]) }

                totalsize = 0x4 + @@VOC.sizeof() + @@MUSIC.sizeof()	
                data11 = byteToWord(RB((@@FLASH.address() + totalsize),(0x2000 - totalsize)))
                structSize = ((0x2000  - totalsize) + 3) / 4
                structSize.times { |i| df.lodWriteWord32(data11[i]) }

            else
                #Dump the calibration sector buffer.
                data = byteToWord($cfpCalibStruct.memgetl())
                structSize = ($cfpCalibStruct.sizeof() + 3) / 4
                structSize.times { |i| df.lodWriteWord32(data[i]) }
            end

        else
            #Dump the calibration sector buffer.
            data = byteToWord($cfpCalibStruct.memgetl())
            structSize = ($cfpCalibStruct.sizeof() + 3) / 4
            structSize.times { |i| df.lodWriteWord32(data[i]) }
            df.lodWriteWord32(structSize + 1)
        end

        #Useless tag for the calibration, but it is useful to check
        #the size fo the sector manually manually

        df.close()

        puts "Done."
    end
    
    # Start an Auto Call test
    addHelpEntry("Calibration",
                 "autoCallTest",
                 "cmd",
                 "int32",
                 "This function starts Auto Call.
                 It returns 1 if success, and returns 0 otherwise.
                 <br>'cmd' is a bit mask to indicate autocall parameters:
                 <br>0x10000 - auto dial 112,
                 <br>0x20000 - auto response,
                 <br>0x40000 - in bank GSM850,
                 <br>0x80000 - in bank GSM900,
                 <br>0x100000 - in bank GSM1800,
                 <br>0x200000 - in bank GSM1900.
                 ")
    def autoCallTest(cmd)
        begin
            globals = CH__stack_map_globals
        rescue
            puts "html><font color=red>Error: Failed to load stack map globals.</font>"
            puts "html><font color=red>Please load the correct xmd file for this connection.</font>"
            return 0
        end

        begin
            autoCallStructPtr = $map_table.>.stack_access.>.autoCallStructPtr.RU
            # Set auto call command
            $map_table.>.stack_access.>.autoCallStructPtr.>.flag.w(cmd)
            $map_table.>.stack_access.>.autoCallStructPtr.>.validity.w(globals::STACK_AUTO_CALL_VALID)
            if(chipHasAp?())
                enterAutocall()
            else
                # Reset the target
                restart()
                # Sometimes we need to unfreeze the phone's CPU (for example, when the code
                # loaded into a phone crashes early after the boot).
                unfreeze()
            end
            return 1
        rescue NoMethodError => e
            puts "html><font color=red>#{e}</font>"
            puts "html><font color=red>Please load the correct xmd file for this connection.</font>"
            return 0
        end
    end

    # Save the result of Auto Call
    addHelpEntry("Calibration",
                 "autoCallSaveResult",
                 "",
                 "int32",
                 "This function saves the result of Auto Call.
                 It returns 1 if success, and returns 0 otherwise.
                ")
    def autoCallSaveResult()
        begin
            globals = CH__stack_map_globals
        rescue
            puts "html><font color=red>Error: Failed to load stack map globals.</font>"
            puts "html><font color=red>Please load the correct xmd file for this connection.</font>"
            return 0
        end

        begin
            autoCallStructPtr = $map_table.>.stack_access.>.autoCallStructPtr.RU
            # Check whether flag is ready
            flag = $map_table.>.stack_access.>.autoCallStructPtr.>.flag.RU
            if(flag != 0)
                puts "html><font color=red>Phone does NOT start the Auto Call test</font>"
                return 0;
            end
        rescue NoMethodError => e
            puts "html><font color=red>#{e}</font>"
            puts "html><font color=red>Please load the correct xmd file for this connection.</font>"
            return 0
        end

        time_out = CALIB_SEND_CMD_DEFAULT_TIMEOUT
        flag = 1
        $map_table.>.stack_access.>.autoCallStructPtr.>.flag.w(flag)
        begin
            Timeout::timeout(time_out) {
                #Wait for the command to be executed
                while(flag == 1) do
                    flag = $map_table.>.stack_access.>.autoCallStructPtr.>.flag.RU
                end
            }
        rescue Timeout::Error
            raise CalibTimeout, "Timeout when saving Auto Test result!"
        end

        if(flag == 0x80000001)
            return 1
        end
        return 0
    end

    # Enter into the calibration stub romed into the code
    # running onto the target.
    addHelpEntry("Calibration",
                 "calib_start_stub",
                 "",
                 "",
                 " This function starts the Calibration Stub.
                 It uses the Target Executing features to do 
                 that, which implies that the board will be 
                 reseted by a call to this function.
                 ")
    def calib_start_stub()
        # If the chip has AP, the calib mode is started by AP
        if(chipHasAp?())
            enterCalib()
            return
        end

        require "target_executor"
        begin
            tgxtor = TargetXtor.new()
            tgxtor.enterHostMonitor()

            # Address of the entry point, where to jump to enter the calib stub.
            w(0xa1a25000,0x66)
            w(0xa1a25000,0x99)
            sleep 1
            tgxtor.targetExecuteAndJump($boot_sector_code.address, 0, @@C::CALIB_MAGIC_TAG)
        ensure
            tgxtor.closeConnection()
        end
    end


    # Read calibration parameters from a CFP file, load them into the 
    # host structure and update the calibration parameters to use them
    addHelpEntry("Calibration",
                 "calib_load_parameters",
                 "filename,initCalib(true)",
                 "",
                 "Load calibration parameters from a file into 
                 the target RAM.
                 initCalib is an optional parameter. If true 
                (default) a calib_init is done before executing the script code.
                Therefore, this parameter only needs to be set to false for higher
                level scripts doing explicitly a calib init.
                Usable by both the calib daemon and the calib stub.
                ")
    def calib_load_parameters(filename, initCalib=true)
        if (@@C::CALIB_VERSION_NUMBER < (@@C::CALIB_MARK_VERSION|0x0205))
            # Tool versions prior to V02.05
            raise "Software map is too old. Calib Tool Version: 0x%08x" % @@C::CALIB_VERSION_NUMBER
        end

        calib_init() if(initCalib)

        # Read the Cfp file that is a lod file in fact,
        # splitting it into packets.
        cfpPackets = LOD_Read(filename)

        # Arrange the Cfp into an array of bytes.
        # The 4 last bytes are a size tag.
        $cfpArray = []
        cfpPackets.each() { |packet|  $cfpArray += packet.data }
        $cfpTailArray = $cfpArray[-4..-1]
        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206)) 
            $cfpArray.slice!($cfpArray.size, 4)
        else
            $cfpArray.slice!($cfpArray.size-4, 4)
        end

        # Check Calibration Version Number 
        cfpVersion = byteToWord($cfpArray[0..3])[0]
        if (cfpVersion != @@CODE_VERSION)
            puts "html><font color=red><b>WARNING: </b>Version mismatched!</font>"
            #puts "Tool Version: 0x%08X" % @@C::CALIB_VERSION_NUMBER
            puts "Code Version: 0x%08X" % @@CODE_VERSION
            puts "CFP Version: 0x%08X" % cfpVersion
            puts ""
        end

        bbSizeAdjusted = 0
        if (CALIB_MARK_MASK(@@CODE_VERSION) == @@C::CALIB_MARK_VERSION && @@CODE_VERSION < (@@C::CALIB_MARK_VERSION|0x0205))
            # Code versions prior to V02.05
            bbSizeAdjusted = 16
        end

        # Create a structure of the type Flash Calibration Sector
        # and set its fields magically
        $cfpCalibStruct = CH__CALIB_BUFFER.new(0)
        structSize = $cfpCalibStruct.sizeof()
        cfpSize = $cfpArray.size()

        if (structSize != cfpSize)
            puts "html><font color=red><b>WARNING: </b>The tool structure size is different with the CFP content size!</font>"
            puts "The tool structure size: %d" % structSize
            puts "The CFP content size: %d" % cfpSize
            puts ""
            if (structSize == cfpSize + 4)
                $cfpArray += $cfpTailArray

            elsif (structSize > cfpSize)
                $cfpArray += Array.new(structSize-cfpSize, 0)
            elsif (structSize < cfpSize)
                $cfpArray.slice!($cfpArray.size-(cfpSize-structSize), cfpSize-structSize)
            end
        end
        $cfpCalibStruct.memsetl($cfpArray)

        puts "Loading a CFP file with version 0x%08X ..." % cfpVersion

        # Set the parameters, through the host structure.

        if (@@C::CALIB_VERSION_NUMBER > (@@C::CALIB_MARK_VERSION|0x0206))
                #### Operation iir buffer.
                @@HST_IIR.address.wb($cfpCalibStruct.iirParam.memgetl())

                #### Operation audio_voc buffer.
                @@HST_VOC.address.wb($cfpCalibStruct.audio_voc.memgetl())

                #### Operation audio_music buffer.
                @@HST_MUSIC.address.wb($cfpCalibStruct.audio_music.memgetl())
        end

        #### Operation information buffer.
        @@HST_OP_INFO.address.wb($cfpCalibStruct.opInfo.memgetl())
        
        #### Transceiver buffer.
        @@HST_XCV.address.wb($cfpCalibStruct.xcv.memgetl())

        #### PA buffer.
        @@HST_PA.address.wb($cfpCalibStruct.pa.memgetl())
    
        #### Switch buffer.
        @@HST_SW.address.wb($cfpCalibStruct.sw.memgetl())
    
        #### Baseband buffer.
        bbBuffer = $cfpCalibStruct.bb.memgetl()
        if (bbSizeAdjusted < 0)
            bbBuffer.slice!(bbBuffer.size+bbSizeAdjusted, -bbSizeAdjusted)
        elsif (bbSizeAdjusted > 0)
            bbBuffer += $cfpArray[-bbSizeAdjusted..-1]
        end
        @@HST_BB.address.wb(bbBuffer)

        # Apply the new config
        calib_update()

        puts "Done."
    end


end


include CoolCalib
