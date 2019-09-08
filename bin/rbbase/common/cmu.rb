#!/usr/bin/env ruby
require 'gpib.rb'
require 'help.rb'

require 'benchmark'


module CMU

    private

    @@CMUHBASE = -1
    @@CMU_BAND_GSM850  = 0
    @@CMU_BAND_GSM900  = 1
    @@CMU_BAND_DCS1800 = 2
    @@CMU_BAND_PCS1900 = 3

    public

    #    addHelpEntry("CMU","CMUW","str","","Writes string str to the CMU")
    def CMUW(str)
        GPIBwrta(@@CMUHBASE,str) {sleep(0)}
    end

    #    addHelpEntry("CMU","CMUR","","str","Reads string from CMU and returns it")
    def CMUR()
        return GPIBrda(@@CMUHBASE) {sleep(0)}
    end

    #    addHelpEntry("CMU", "CMUCaliInit", "inloss=[0.5,0.5,0.6,0.7], outloss=[0.5,0.5,0.6,0.7]", "",
    #    "Initializes the CMU-200 for the calibration needs.")
    def CMUCalibInit(inloss=[0.5,0.5,0.6,0.7],outloss=[0.5,0.5,0.6,0.7])
        puts "Init the CMU-200 tester..."

        #Get Handle
        @@CMUHBASE = GPIBdev(0,20,96,15,1,0)

        #Get a thread local version of the status and check if device is connected correctly
        status = GPIBThreadIbsta()
        raise "ERROR: Unable to open device CMU" if( (status & ERR)!=0)

        #Devices connected to this board are automatically put into local lockout mode
        GPIBconfig(0,IbcSendLLO,1)

        #Get a thread local version of the status and check if device is connected correctly
        status = GPIBThreadIbsta()
        raise "ERROR: Unable to open device CMU" if( (status & ERR)!=0)

        #Set the timeout value on the CMU
        GPIBtmo(@@CMUHBASE,T3s)

        #Query identification
        CMUW("*IDN?")

        #Query the options included in the instrument and get the identification
        status = GPIBThreadIbsta()
        raise "ERROR: unable to communicate with CMU! Probably your CMU is not set to default primary GPIB addr(20)" if( (status & ERR)!=0)
        CMUR()

        #Query the options included in the instrument and get the information
        CMUW("*OPT?")
        CMUR()

        #Disable saving measurement settings to the non volatile ram
        CMUW("SYST:NONV:DIS")

        #When CMU change from local to remote, the current signaling state or generator state maintained
        CMUW("SYST:GTRM:COMP OFF")

        #Display remote report
        CMUW("TRAC:REM:MODE:DISP ON")

        #Use the internal reference frequence
        CMUW("CONF:SYNC:FREQ:REF:MODE INT")

        #Clear output buffer, reset instrument, prevent the following command to JCES to be executed before "RST" is completed, query information
        CMUW("*CLS;*RST;*OPC?")
        CMUR()

        #set second address "GSM850MS_NSIG"
        CMUW("SYST:REM:ADDR:SEC 1,\"GSM850MS_NSig\"")
        #set second address "GSM900MS_NSIG"
        CMUW("SYST:REM:ADDR:SEC 2,\"GSM900MS_NSig\"")
        #set second address "DCS1800MS_NSIG"
        CMUW("SYST:REM:ADDR:SEC 3,\"GSM1800MS_NSig\"")
        #set second address "PCS1900MS_NSIG"
        CMUW("SYST:REM:ADDR:SEC 4,\"GSM1900MS_NSig\"")
        #set second address "GSM900MS_SIG"
        CMUW("*CLS;SYST:REM:ADDR:SEC 5,\"GSM900MS_Sig\"")

        puts "GSM850\t%0.2fdB\t%0.2fdB " % [inloss[0],outloss[0]]
        puts "GSM900\t%0.2fdB\t%0.2fdB " % [inloss[1],outloss[1]]
        puts "DCS1800\t%0.2fdB\t%0.2fdB " % [inloss[2],outloss[2]]
        puts "PCS1900\t%0.2fdB\t%0.2fdB " % [inloss[3],outloss[3]]
        puts " "

        #external attenuations
        CMUW(sprintf("1;SENS:CORR:LOSS:INP2 %0.2f",inloss[0]))
        CMUW(sprintf("1;SENS:CORR:LOSS:OUTP2 %0.2f",outloss[0]))
        CMUW(sprintf("2;SENS:CORR:LOSS:INP2 %0.2f",inloss[1]))
        CMUW(sprintf("2;SENS:CORR:LOSS:OUTP2 %0.2f",outloss[1]))
        CMUW(sprintf("3;SENS:CORR:LOSS:INP2 %0.2f",inloss[2]))
        CMUW(sprintf("3;SENS:CORR:LOSS:OUTP2 %0.2f",outloss[2]))
        CMUW(sprintf("4;SENS:CORR:LOSS:INP2 %0.2f",inloss[3]))
        CMUW(sprintf("4;SENS:CORR:LOSS:OUTP2 %0.2f",outloss[3]))

        CMUW("1;RFAN:TSEQ OFF")
        CMUW("2;RFAN:TSEQ OFF")
        CMUW("3;RFAN:TSEQ OFF")
        CMUW("4;RFAN:TSEQ OFF")
    end

    def CMUBERInit(band, codec, amr_mode,  ext_inloss, bcch_channel, bcch_level, bcch_tn, arfcn, tch_tn, tch_pcl, level)
        puts "Init the CMU-200 tester..."

        #Get Handle
        @@CMUHBASE = GPIBdev(0, 20, 96, 17, 1, 0);

        #get a thread-local version of the status
        status = GPIBThreadIbsta()
        raise "ERROR: Unable to open device CMU" if( (status & ERR)!=0)

        #Devices connected to this board are automatically put into local lockout mode
        GPIBconfig(0,IbcSendLLO,1)

        #Get a thread local version of the status and check if device is connected correctly
        status = GPIBThreadIbsta()
        raise "ERROR: Unable to open device CMU" if( (status & ERR)!=0)

        #query identification
        CMUW("*IDN?")

        #get the identification in InBuffer
        CMUR()

        #query the options included in the instrument 
        CMUW("*OPT?")

        #get the query information
        CMUR()

        #disable saving measurement settings to the non volatile ram
        CMUW("SYST:NONV:DIS")

        #when CMU change from local to remote, the current signaling state or generator state maintained
        CMUW("SYST:GTRM:COMP OFF")

        #display remote report
        CMUW("TRAC:REM:MODE:DISP ON")

        #use the internal reference frequence
        CMUW("CONF:SYNC:FREQ:REF:MODE INT")

        #clear output buffer, reset instrument, prevent the following command
        #to be executed before "RST" is completed
        CMUW("*CLS;*RST;*OPC?")

        #get the query information
        CMUR()

        #set first address to GSM850
        CMUW("*CLS;SYST:REM:ADDR:SEC 1,\"GSM850MS_Sig\"")

        #set first address to GSM900
        CMUW("SYST:REM:ADDR:SEC 2,\"GSM900MS_Sig\"")

        #set second address to DCS
        CMUW("SYST:REM:ADDR:SEC 3,\"GSM1800MS_Sig\"")

        #set third address to PCS
        CMUW("SYST:REM:ADDR:SEC 4,\"GSM1900MS_Sig\"")

        #Read status by query
        CMUW("*STB?")

        #get the query information
        CMUR()

        ###############################

        #RF input is According to power control level of the mobile
        CMUW("%d;LEV:MODE PCL" % band)

        #query the current signaling state
        CMUW("%d;SIGN:STAT?" % band)

        #get the query information
        CMUR()

        #Correspond to Network tab and popup menu connect
        CMUW("%d;CONF:NETW:NSUP GSM" % band)

        #speech coding and voice transmission in the traffic channel
        CMUW("%d;CONF:NETW:SMOD:TRAF %s" % [band,codec])

        if(codec == "AMRH")
            CMUW("%d;CONF:NETW:AMR:HRAT:RSET OFF,OFF,OFF,%s,15.0,17.0,12.5,15.0,11.0,13.0" % [band,amr_mode])
            CMUW("%d;CONF:NETW:AMR:HRAT:DLCM %s" % [band,amr_mode])
            CMUW("%d;CONF:NETW:AMR:HRAT:ULCM %s" % [band,amr_mode])       
        elsif (codec == "AMRF")
            CMUW("%d;CONF:NETW:AMR:FRAT:RSET OFF,OFF,OFF,%s,16.5,18.5,11.5,13.5,6.5,8.5" % [band,amr_mode])
            CMUW("%d;CONF:NETW:AMR:FRAT:DLCM %s" % [band,amr_mode])
            CMUW("%d;CONF:NETW:AMR:FRAT:ULCM %s" % [band,amr_mode])
        end 

        #type of data transmitted in the traffic channel
        CMUW("%d;CONF:NETW:SMOD:BITS PR9" % band)

        #the sequence which the CMU transmit to MS in RXQ mesurement
        CMUW("%d;CONF:RXQ:BITS PR9" % band)

        #define Mobile contry code
        CMUW("%d;CONF:NETW:IDEN:MCC 1" % band)

        #define Mobile Network code
        CMUW("%d;CONF:NETW:IDEN:MNC 1" % band)

        #the connector to be used for RF input signal
        CMUW("%d;INP:STAT RF2" % band)

        #the connector to be used for RF output signal
        CMUW("%d;OUTP:STAT RF2" % band)

        #external attenuation value to the input defined before
        CMUW("%d;SENS:CORR:LOSS:INP2 %s" % [band, ext_inloss])

        #BCCH is switched off as soon as CEST is reached
        #so that all 8 timeslot are available for the traffic channel
        CMUW("%d;CONF:BSS:CCH:MODE BATC" % band)

        #control channel for CMU (BCCH)
        CMUW("%d;CONF:BSS:CCH:CHAN %d"  % [band, bcch_channel])

        #the level of the control channel
        CMUW("%d;CONF:BSS:CCH:LEV -%d.0" % [band, bcch_level])

        #number of the traffic channel
        CMUW("%d;CONF:BSS:CHAN %d" % [band, arfcn])

        #timeslot the MS and BS/CMU used for signaling
        CMUW("%d;CONF:BSS:MSL:MTIM %d" % [band, bcch_tn])

        #timeslot for the BS traffic channel
        CMUW("%d;CONF:BSS:SSL:TIM %d" % [band, tch_tn])

        #CMU not uses the used/unused level scheme(2 different levels) 
        CMUW("%d;CONF:BSS:MSL:LMOD IND" % band)

        #mobile power level upon registration in the network
        CMUW("%d;CONF:MSS:MS:PCL %d" % [band, tch_pcl])

        #absolute level in the used timeslot
        CMUW("%d;CONF:BSS:LEV:UTIM -%d.0"  % [band, level])

        CMUW("%d;CONF:SIGN:SMOD SSL" % band)

        #defines BS active time slot and power levels
        CMUW("%d;CONF:BSS:MSL:SCON:IND OFF,OFF,OFF,ON,OFF,OFF,OFF,OFF,0.0,-20.0,-20.0,0.0,-20.0,-20.0,-20.0,-20.0" % band)

        #defines MS active time slot and power levels
        CMUW("%d;CONF:MSS:MSL:SCON OFF,OFF,OFF,ON,OFF,OFF,OFF,OFF,15,15,15,15,15,15,15,15" % band)

    end

    def CMUBLERInit(band, cs, ext_inloss, bcch_channel, bcch_level, bcch_tn, arfcn)
        puts "Init the CMU-200 tester..."

        #Get Handle
        @@CMUHBASE = GPIBdev(0, 20, 96, 17, 1, 0);

        #get a thread-local version of the status
        status = GPIBThreadIbsta()
        raise "ERROR: Unable to open device CMU" if( (status & ERR)!=0)

        #Devices connected to this board are automatically put into local lockout mode
        GPIBconfig(0,IbcSendLLO,1)

        #Get a thread local version of the status and check if device is connected correctly
        status = GPIBThreadIbsta()
        raise "ERROR: Unable to open device CMU" if( (status & ERR)!=0)

        #query identification
        CMUW("*IDN?")

        #get the identification in InBuffer
        CMUR()

        #query the options included in the instrument 
        CMUW("*OPT?")

        #get the query information
        CMUR()

        #disable saving measurement settings to the non volatile ram
        CMUW("SYST:NONV:DIS")

        #when CMU change from local to remote, the current signaling state or generator state maintained
        CMUW("SYST:GTRM:COMP OFF")

        #display remote report
        CMUW("TRAC:REM:MODE:DISP ON")

        #use the internal reference frequence
        CMUW("CONF:SYNC:FREQ:REF:MODE INT")

        #clear output buffer, reset instrument, prevent the following command
        #to be executed before "RST" is completed
        CMUW("*CLS;*RST;*OPC?")

        #get the query information
        CMUR()

        #set first address to GSM850
        CMUW("*CLS;SYST:REM:ADDR:SEC 1,\"GSM850MS_Sig\"")

        #set first address to GSM900
        CMUW("SYST:REM:ADDR:SEC 2,\"GSM900MS_Sig\"")

        #set second address to DCS
        CMUW("SYST:REM:ADDR:SEC 3,\"GSM1800MS_Sig\"")

        #set third address to PCS
        CMUW("SYST:REM:ADDR:SEC 4,\"GSM1900MS_Sig\"")

        #Read status by query
        CMUW("*STB?")

        #get the query information
        CMUR()

        ###############################

        #RF input is According to power control level of the mobile
        CMUW("%d;LEV:MODE PCL" % band)

        #query the current signaling state
        CMUW("%d;SIGN:STAT?" % band)

        #get the query information
        CMUR()

        #Correspond to Network tab and popup menu connect
        CMUW("%d;CONF:NETW:NSUP GGPR" % band)

        #speech coding and voice transmission in the traffic channel
        CMUW("%d;CONF:NETW:PDAT:CSCH %s" % [band,cs])

        #type of data transmitted in the tbf
        CMUW("%d;CONF:NETW:PDAT:BITS PR9" % band)

        #define Mobile contry code
        CMUW("%d;CONF:NETW:IDEN:MCC 1" % band)

        #define Mobile Network code
        CMUW("%d;CONF:NETW:IDEN:MNC 1" % band)

        #the connector to be used for RF input signal
        CMUW("%d;INP:STAT RF2" % band)

        #the connector to be used for RF output signal
        CMUW("%d;OUTP:STAT RF2" % band)

        #external attenuation value to the input defined before
        CMUW("%d;SENS:CORR:LOSS:INP2 %s" % [band, ext_inloss])

        #BCCH is switched off as soon as CEST is reached
        #so that all 8 timeslot are available for the traffic channel
        CMUW("%d;CONF:BSS:CCH:MODE BOTC" % band)

        #control channel for CMU (BCCH)
        CMUW("%d;CONF:BSS:CCH:CHAN %d"  % [band, bcch_channel])

        #the level of the control channel
        CMUW("%d;CONF:BSS:CCH:LEV -%d.0" % [band, bcch_level])

        #number of the traffic channel
        CMUW("%d;CONF:BSS:PDAT:MSL:CHAN %d" % [band, arfcn])

        #timeslot the MS and BS/CMU used for signaling
        #CMUW("%d;CONF:BSS:MSL:MTIM %d" % [band, bcch_tn])

        #timeslot for the BS traffic channel
        #CMUW("%d;CONF:BSS:SSL:TIM %d" % [band, tch_tn])

        #CMU not uses the used/unused level scheme(2 different levels) 
        #CMUW("%d;CONF:BSS:MSL:LMOD IND" % band)

        #mobile power level upon registration in the network
        #CMUW("%d;CONF:MSS:MS:PCL %d" % [band, tch_pcl])

        #absolute level in the used timeslot
        #CMUW("%d;CONF:BSS:LEV:UTIM -%d.0"  % [band, level])

        CMUW("%d;CONF:SIGN:SMOD MSL" % band)

        #defines BS active time slot and power levels
        #CMUW("%d;CONF:BSS:MSL:SCON:IND OFF,OFF,OFF,ON,OFF,OFF,OFF,OFF,0.0,-20.0,-20.0,0.0,-20.0,-20.0,-20.0,-20.0" % band)

        #defines MS active time slot and power levels
        #CMUW("%d;CONF:MSS:MSL:SCON OFF,OFF,OFF,ON,OFF,OFF,OFF,OFF,15,15,15,15,15,15,15,15" % band)

    end

    def CMU_CH2FREQ(band,arfcn,dw1up0)
        # Calculate the Tx frequency depending on the band and arfcn.
        case band
        when (@@CMU_BAND_PCS1900) # PCS 1900
            return ((18502.0 + 2.0 * (arfcn-512) + (dw1up0 ? 800: 0)) / 10.0)

        when (@@CMU_BAND_DCS1800) # DCS 1800
            return ((17102.0 + 2.0 * (arfcn-512) + (dw1up0 ? 950: 0)) / 10.0)

        when (@@CMU_BAND_GSM850) # GSM 850
            if (128<=arfcn && arfcn <= 251) then
                return (( 8242.0 + 2.0 * (arfcn-128) + (dw1up0 ? 450: 0)) / 10.0)
            else
                raise "ARFCN: %d invalid for GSM850" % arfcn
            end
        else # GSM 900
            if (0<= arfcn && arfcn <=124) then
                return (( 8900.0 + 2.0 * arfcn      + (dw1up0 ? 450: 0)) / 10.0)
            elsif (975 <= arfcn && arfcn <= 1023) then
                return (( 8900.0 + 2.0 * (arfcn-1024)      + (dw1up0 ? 450: 0)) / 10.0)
            else
                raise "ARFCN: %d invalid for GSM900" % arfcn
            end
        end
    end

    def CMU_BAND(band)
        # GSM 850 is secondary address 1
        # GSM 900 is secondary address 2
        # DCS1800 is secondary address 3
        # PCS1900 is secondary address 4
        return (band + 1)
    end 

    addHelpEntry("CMU","CMUClose","","","Closes the CMU.")
    def CMUClose()
        GPIBonl(@@CMUHBASE,0)
        @@CMUHBASE = -1
    end

    def CMUGeneratorMode(band,arcfn,power)
        #CMU generator channel, 30 = 941 Mhz
        CMUW(sprintf("%d;SOUR:RFG:FREQ:CHAN %.1f",CMU_BAND(band),CMU_CH2FREQ(band,arcfn,true)))
        #Used power level, in dBm
        CMUW(sprintf("%d;SOUR:RFG:LEV:UTIM %d",CMU_BAND(band),power))
        #Unused power to reference power
        CMUW(sprintf("%d;SOUR:RFG:LEV:UNT %d",CMU_BAND(band),0))
        #Set the CMU in generator mode.
        CMUW(sprintf("%d;INIT:RFG",CMU_BAND(band)))
    end

    def CMUGeneratorStop(band)
        #CMU generator oss.
        CMUW(sprintf("%d;ABOR:RFG",CMU_BAND(band)))
    end

    def CMUMeasurePower(band,arcfn)
        #Init the CMU analyser.
        CMUW(sprintf("%d;CONF:POW:CONT SCAL,1",CMU_BAND(band)))
        #Set the CMU analyser frequenbandcy.
        CMUW(sprintf("%d;RFAN:CHAN %.1fE6", CMU_BAND(band),CMU_CH2FREQ(band,arcfn,false)))
        #Request a read power to the CMU.
        CMUW(sprintf("%d;READ:POW?", CMU_BAND(band)))
        #Read the power from the CMU.

        return CMUR().tr(',',' ').split(' ')[0].to_f()
    end

    def CMUMeasureFOf(band,arfcn)
        #Setup the RF analyser frequency.
        CMUW(sprintf("%d;RFAN:CHAN %.1fE6", CMU_BAND(band),CMU_CH2FREQ(band,arfcn,false)))
        #Configure to test N bursts.
        CMUW(sprintf("%d;CONF:MOD:PERR:CONT ARR,20", CMU_BAND(band)))
        #Init the special mode of the CMU.
        CMUW(sprintf("%d;INIT:MOD:PERR", CMU_BAND(band)))
        #Request a read frequency error to the CMU.
        CMUW(sprintf("%d;FETC:MOD:PERR?", CMU_BAND(band)))

        return CMUR().tr(',',' ').split(' ')[7].to_f()
    end

end

include CMU
