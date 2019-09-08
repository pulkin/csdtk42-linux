#!/usr/bin/env ruby
require 'gpib.rb'
require 'help.rb'

require 'benchmark'


module ABFS

private
	
	@@ABFSHBASE=-1

public

	addHelpEntry("ABFS","ABFSW","str","","Writes string 'str' to the ABFS")
	def ABFSW(str)
		GPIBwrt(@@ABFSHBASE,str)
	end
	
	addHelpEntry("ABFS","ABFSR","","str","Reads string from ABFS and returns it")
	def ABFSR()
		return GPIBrd(@@ABFSHBASE)
	end
	
	def ABFSBERInit()
		#Get Handle
		@@ABFSHBASE = GPIBdev(0, 28, 0 , 17, 1, 0)

		#get a thread-local version of the status
		status = GPIBThreadIbsta()
		raise "ERROR : Unable to open device ABFS" if( (status & ERR)!=0)
	end
	
	
	addHelpEntry("ABFS","ABFSClose","","","Closes the ABFS.")
	def ABFSClose()
		GPIBonl(@@ABFSHBASE,0)
		@@ABFSHBASE = -1
	end

end

include ABFS