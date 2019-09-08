#!/usr/bin/env ruby

#Cool requiries
require 'gpib.rb'
require 'help.rb'
require 'cmu.rb'
require 'abfs.rb'

#Std requiries
require 'scanf.rb'

module BERTESTER

private
	
	@@BERHBASE = -1
	
	CMU_CODEC 			= ["FRV1","FRV2","HRV1"]

	EXT_IN_LOSS			= "0.5"
	EXT_OUT_LOSS_STATIC	= "0.5"
	EXT_OUT_LOSS_TU50	= "8.5"
	EXT_OUT_LOSS_RA250	= "10.4"
	EXT_OUT_LOSS_HT100	= "8.8" 	
	
	BCCH_CHANNEL	= [0,32,600,600]
	BCCH_LEVEL		= 85
	BCCH_TN			= 3
	
	TCH_TN			= 3
	TCH_PCL			= 5	
	
	MODE_STATIC	= 0
	MODE_TU50	= 1
	MODE_RA250	= 2
	MODE_HT100	= 3	
	
	POS_FER		= 0
	POS_RBER_Ib = 1
	POS_RBER_II = 2
	
	
	BERSpeechStr 	= ["FS ","EFS","HS "]
	BERBandStr		= ["???","GSM","DCS","PCS"]
	
	BERLimit 		=  [
	[	#Full Rate Speech
		#FER  RBER Ib 	RBER II
		[0.1, 	0.4, 	2], #STATIC
		[6,   	0.4, 	8], #TU 50 (no FH)
		[2,   	0.2, 	7], #RA 250 (no FH)
		[7,   	0.5, 	9], #HT 100 (no FH)
		[3,   	0.3, 	8], #PTU 50 (no FH)
		[2,   	0.2, 	7], #PRA 130 (no FH)
		[7,   	0.5, 	9]  #PHT 100 (no FH)
	],
	[	#Enhanced Full Rate Speech
		# FER  RBER Ib  RBER II
		[0.1, 	0.1, 	2], #STATIC
		[8,   	0.21,	7], #TU 50 (no FH)
		[3,   	0.1, 	7], #RA 250 (no FH)
		[7,   	0.2, 	9], #HT 100 (no FH)
		[4,   	0.12,	8], #PTU 50 (no FH)
		[3,   	0.1, 	7], #PRA 130 (no FH)
		[7,   	0.24,	9]  #PHT 100 (no FH)
	],
	[	#Half Rate Speech
		# FER  RBER Ib 	RBER II
		[0.025, 0.001, 	0.72], #STATIC
		[4.1,   0.36,  	6.9], #TU 50 (no FH)
		[4.1,   0.28,  	6.8], #RA 250 (no FH)
		[4.5,   0.56,  	7.6], #HT 100 (no FH)
		[4.2,   0.38,  	6.9], #PTU 50 (no FH)
		[4.1,   0.28,  	6.8], #PRA 130 (no FH)
		[5.0,   0.63,  	7.8]  #PHT 100 (no FH)
	]]
	
	class BERUserOptions
		attr_accessor :band, :level, :arfcn, :freq, :speech, :do_static, :do_tu50, :do_ra250, :do_ht100, :nb_frames
	end

public

	addHelpEntry("BERTESTER","BERW","str","","Writes string str to the BERTESTER")
	def BERW(str)
		GPIBwrt(@@BERHBASE,str)
	end
	
	addHelpEntry("BERTESTER","BERW","","str","Reads string from BERTESTER and returns it")
	def BERR()
		return GPIBrd(@@BERHBASE)
	end
	
	def BERMakeOptions(band, level, arfcn, speech, bool_static, bool_tu50, bool_ra250, bool_ht100, frames)
		options = BERUserOptions.new
	
		case band
			when 3
				options.band = 3
			when 2
				options.band = 2
			else
				options.band = 1
		end
		
		options.level = level
		options.arfcn = arfcn
		
		case options.band
			when 3
				options.freq = 18502.0 + 2*(options.arfcn-512) + 800
			when 2
				options.freq = 17102.0 + 2*(options.arfcn-512) + 950
			else
				options.freq = 8900.0 + 2*options.arfcn + 450
		end
		
		case speech
			when 3
				options.speech = 2
			when 2
				options.speech = 1
			else
				options.speech = 0
		end
		
		options.do_static 	= bool_static
		options.do_tu50 	= bool_tu50
		options.do_ra250 	= bool_ra250
		options.do_ht100 	= bool_ht100
		options.nb_frames 	= frames
		
		return options
	end
	
	def berputs(str)
		@@BERLOG << str
		puts str
	end
	
	def BERSetFadingMode(mode,options)
	
		case mode
			when MODE_STATIC
				#set ABFS stat=OFF
				ABFSW("FSIM:STAT OFF")
				#set CMU IQIF to fading Path
				CMUW("%d;CONF:IQIF:RXTX BYP" % options.band)
				#set external attenuation
				CMUW("%d;SENS:CORR:LOSS:OUTP2 %s" % [options.band,EXT_OUT_LOSS_STATIC])
					
			when MODE_TU50
				#set CMU external attenuation
				CMUW("%d;SENS:CORR:LOSS:OUTP2 %s" % [options.band,EXT_OUT_LOSS_TU50])
				
				#set ABFS stat=ON
				ABFSW("FSIM:STAT ON")
				
				#set ABFS RF Frequ
				ABFSW("FSIM:CHANNEL:RF %dE5" % options.freq)
				
				#set Fading mode
				if(options.band == 1) 
					ABFSW("FSIM:STANDARD G6TU50")
				else	
				 	ABFSW("FSIM:STANDARD P6TU50")
				end
				
				#ABFS wait for synchronism 
				ABFSW("*WAI")
				
				#set CMU IQIF to fading Path
				CMUW("%d;CONF:IQIF:RXTX FPAT" % options.band)
								
			when MODE_RA250
				#set external attenuation
				CMUW("%d;SENS:CORR:LOSS:OUTP2 %s" % [options.band,EXT_OUT_LOSS_RA250])
				#set ABFS stat=ON
				ABFSW("FSIM:STAT ON")
				#set ABFS RF Frequ
				ABFSW("FSIM:CHANNEL:RF %dE5" % options.freq)
				#set Fading mode
				if(options.band == 1)
				 	ABFSW("FSIM:STANDARD GRA250")
				else
					ABFSW("FSIM:STANDARD PRA130")
				end
				
				#ABFS wait for synchronism 
		    	ABFSW("*WAI")
		    	#set CMU IQIF to fading Path
				CMUW("%d;CONF:IQIF:RXTX FPAT" % options.band)
								
			when MODE_HT100
				#set external attenuation
				CMUW("%d;SENS:CORR:LOSS:OUTP2 %s" % [options.band, EXT_OUT_LOSS_HT100])
				#set ABFS stat=ON
				ABFSW("FSIM:STAT ON")
				#set ABFS RF Frequ
				ABFSW("FSIM:CHANNEL:RF %dE5" % options.freq)
				#set Fading mode
				if(options.band == 1) 
					ABFSW("FSIM:STANDARD G6HT100")
				else
					ABFSW("FSIM:STANDARD P6HT100")
				end
				#ABFS wait for synchronism 
		    	ABFSW("*WAI")
		    	#set CMU IQIF to fading Path
				CMUW("%d;CONF:IQIF:RXTX FPAT" % options.band)		
			else
		end
	end
	
	
	def BERWaitForMsSyncAndMsConnect(band)
	
		#switch on BCCH channel
		CMUW("%d;PROC:SIGN:ACT SON;*OPC?" % band)
		#get the query information
		CMUR()
		
		#query the current signaling state
	    CMUW("%d;SIGN:STAT?" % band)
		rdbuf = CMUR()
	
		puts "\n--> Switch OFF and ON the phone\n"
	
		#Wait for Signaling ON
		while(rdbuf.slice(0..2) != "SON")
			#query the current signaling state
	   		CMUW("%d;SIGN:STAT?" % band)
			rdbuf = CMUR()
		end 
	
		#Wait for MS Sync
		while(rdbuf.slice(0..3) != "SYNC")
			#query the current signaling state
			CMUW("%d;SIGN:STAT?" % band)
			rdbuf = CMUR()
		end
	
		puts "\n--> Set the call \n"
	
		#Wait for Call Established
		while(rdbuf.slice(0..3) != "CEST")
			#query the current signaling state
			CMUW("%d;SIGN:STAT?" % band)
			rdbuf = CMUR()
		end
		
	end
	

	
	def DOFERRBERMeasurement(mode, options)
		
		alpha 	= 1.0
		
		case mode
			when MODE_STATIC
				temp_str = "STATIC"
			when MODE_TU50
				temp_str = "TU50"
			when MODE_RA250
			
				if(options.band==1) 
					temp_str = "RA250"
				else
					temp_str = "RA130"
				end
				
			when MODE_HT100
				temp_str = "HT100"
			else
				temp_str = "UNKNOWN"
		end
			
		#add 3 to mode to address the PCS/DCS ber values in the table
		mode += 3 if(options.band != 1) 
	
		berputs "\n*****************************************************"
		berputs "\n     TEST Conditions : %s -%d dBm\n" % [temp_str, options.level]
		#set UNUSED = 20dB
		CMUW("%d;PROC:BSS:TCH:LEV:UNT 20" % options.band)
	
		#defines hold off times during which the mobile can adapt
		#itself to the new RF level at the beginning of the RXQ
		#mesurement and send back the bit stream received
		CMUW("%d;CONF:RXQ:CONT:HTIM 0.80,0.20" % options.band)
	
		#defines the measured timeslot of the MS signal
		CMUW("%d;CONF:MCON:MSL:MESL %d" % [options.band,TCH_TN])
	
		#set stop condition and step mode: not abort
		CMUW("%d;CONF:RXQ:BER1:CONT:REP NONE,NONE" % options.band)
	
		#RFER test on ? frames
		CMUW("%d;CONF:RXQ:BER1:CONT RFER,%d" % [options.band,options.nb_frames])
	
		#set USED = -104dB in GSM or -102dBm in D(P)CS 
		CMUW("%d;CONF:RXQ:BER1:CONT:LEV:UTIM -%d.0" % [options.band,options.level])
	
		#apply the above parameters
	    CMUW("%d;INIT:RXQ:BER" % options.band)
	
		#Query result
		CMUW("%d;FETC:RXQ:BER?" % options.band)
			
		#change all ',' to ' ' in the buffer
		res = CMUR().split(',')
		
		progtime 	= res[0].to_f
		iires 		= res[1].to_f
		ibres			= res[2].to_f
		ferres 		= res[3].to_f
			
		#Derive alpha if needed
		if(ferres > BERLimit[options.speech][mode][POS_FER])
			alpha = ferres * 1.0 / (BERLimit[options.speech][mode][POS_FER])
		end
		
		if (options.speech != 0) 
			alpha = 1.0 		#Alpha term exists for FS only 
		end
		
		if(alpha > 1.6)
			ibLimit 	= BERLimit[options.speech][mode][POS_RBER_Ib] 	/ 1.6
			ferLimit 	= BERLimit[options.speech][mode][POS_FER] 		* 1.6
		else
			ibLimit 	= BERLimit[options.speech][mode][POS_RBER_Ib] 	/ alpha
			ferLimit 	= BERLimit[options.speech][mode][POS_FER] 		* alpha
		end
		
		#print measurement result
		berputs "\nNumber of frames evaluated : %d" % options.nb_frames
		berputs "\nFrame error rate           : %6.3f%% [limit %4.3f%%]" % [ferres, ferLimit]
		berputs "\nClass Ib residual bit error: %6.3f%% [limit %4.3f%%]" % [ibres, ibLimit]
		berputs "\nClass II residual bit error: %6.3f%% [limit %4.3f%%]" % [iires, BERLimit[options.speech][mode][POS_RBER_II] ]
	
		if (options.speech == 0) #Alpha term exists for FS only
			berputs "\nValue of alpha = %5.2f - FER test " % alpha
			#check limit
			if( alpha > 1.6 )
			   	berputs "INVALID"
			else
				berputs "VALID"
			end
		end
	
		if(	(iires  >= BERLimit[options.speech][mode][POS_RBER_II]) || (ibres  >= ibLimit) || (ferres >= ferLimit) || (alpha > 1.6) )
			berputs "\n\n\t\t!!! TEST FAIL !!!\n"
		else
			berputs "\n\n\t\tTEST PASS\n"
		end
		berputs "\n*****************************************************\n"
		
	end
	
	def BERGetOffline()
		CMUClose()
		ABFSClose()
	end
		
	def BERLaunchTest(title,band, level, arfcn, speech, bool_static, bool_tu50, bool_ra250, bool_ht100, frames)
		
		@@BERLOG = File.open(title+".ber","wb")
		
		#User test setup
		useroptions = BERMakeOptions(band, level, arfcn, speech, bool_static, bool_tu50, bool_ra250, bool_ht100, frames)
		
		berputs "*****************************************************"
		berputs "* BER Test : %s" 		% title
		berputs "* Level    : -%d dBm" 	% useroptions.level
		berputs "* Channel  : %d %s" 	% [useroptions.arfcn, BERBandStr[useroptions.band] ]
		berputs "* Speech   : %s"		% BERSpeechStr[useroptions.speech]
		berputs "* Nb Frames: %d"		% useroptions.nb_frames
		berputs "*****************************************************"
		
		ABFSBERInit()
		CMUBERInit(useroptions.band,CMU_CODEC[useroptions.speech],EXT_IN_LOSS,BCCH_CHANNEL[useroptions.band],BCCH_LEVEL,BCCH_TN,useroptions.arfcn,TCH_TN,TCH_PCL,useroptions.level)
		
		BERSetFadingMode(MODE_STATIC,useroptions)
		BERWaitForMsSyncAndMsConnect(useroptions.band)
		
		#STATIC Test
		if (useroptions.do_static)
			DOFERRBERMeasurement(MODE_STATIC,useroptions)
		end
			
		#TU50 Test
		if(useroptions.do_tu50)
			BERSetFadingMode(MODE_TU50,useroptions)
			DOFERRBERMeasurement(MODE_TU50,useroptions)
		end
		
		#RA250 Test
		if (useroptions.do_ra250)
			BERSetFadingMode(MODE_RA250,useroptions)
			DOFERRBERMeasurement(MODE_RA250,useroptions)
		end
		
		#HT100 Test
		if (useroptions.do_ht100)
			BERSetFadingMode(MODE_HT100,useroptions)
			DOFERRBERMeasurement(MODE_HT100,useroptions)
		end
		
		#Terminate call & switch on BCCH channel
		CMUW("%d;PROC:SIGN:ACT CREL;*OPC?" % useroptions.band)
		
		#Back to Static
		BERSetFadingMode(MODE_STATIC,useroptions)
		
		@@BERLOG.close()
		
		BERGetOffline()
		
	end
	
end

include BERTESTER