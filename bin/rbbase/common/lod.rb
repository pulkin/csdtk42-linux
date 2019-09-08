#############################################################
#															#
#    Lod Utilies for reading and writing lod files          #
#															#
#############################################################

class File
	public
	def lodWriteAddress(address)
		self << "@%08x\n" % (address & 0xFFFFFFFF)
	end
	
	def lodWriteWord32(word)	
		self << "%08x\n" % (word & 0xFFFFFFFF)
	end
	
	def lodWritePacket(p)
		lodWriteAddress(p.address)
		data = p.data.from8to32bits()
		data.each{ |w|
		    lodWriteWord32(w)
		}
	end
end

###############################
#          LOD FILES          #
###############################

class LdVerifyFailed < Exception
end

def ldVerifyPackets(connection,packets)
    vfailed = false
    puts "Verifying (may take a while)..."
    packets.each{ |packet|
        data = connection.readBlock(packet.address,packet.data.size)   
        if(data != packet.data)
            vfailed = true
            msg = ""
            data.size.times { |i|
                if(data[i]!=packet.data[i])
                    msg += "Verify failed! First failure address : 0x%08X." % (packet.address+i)
                    break
                end
            }
            raise LdVerifyFailed, msg
            break
        end
    }
    return true
end

def ldPackets(connection,packets,bool_verify)
    packets.each{ |packet|
        connection.writeBlock(packet.address,packet.data)
    }
    
    #Verifying
    if(bool_verify)
        success = ldVerifyPackets(connection,packets)
        puts "Verify OK!" if(success)
    end   
    return true
end

addHelpEntry("writing","ld","str_filename,bool_verify=false","bool","Loads a lod file and verifies the loading operation if bool_verify==true")
def ld(str_filename,bool_verify=false)
    puts "Loading lod file ..."
    packets = LOD_Read(str_filename)
    ldPackets($CURRENTCONNECTION,packets,bool_verify)
    puts "Done."
end

addHelpEntry("reading","verifylod","str_filename","bool","Verifies if the lod file 'str_filename' matches the memory")
def verifylod(str_filename)
    packets = LOD_Read(str_filename)
    ldVerifyPackets($CURRENTCONNECTION,packets)
end

def dumpProgress(prog)
    begin 
        cwSetProgress(prog*100,100,"%p% (dumping)")
    rescue Exception
        puts( prog*100 )
    end
end

addHelpEntry("reading", "dump", "str_filename, address, nbwords", "bool", "Dumps 'nbwords' 32-bits words from memory, starting at address 'address', inside file 'str_filename'. If the filename ends with '.lod' the lod file format will be used, else the binary file format will be used.")
def dump(str_filename, address, nbwords)
    dumpClassic(str_filename, address, nbwords)
end

addHelpEntry("reading", "dumpfast", "str_filename, address, nbwords", "bool", "Dumps 'nbwords' 32-bits words in batch read mode from memory, starting at address 'address', inside file 'str_filename'. If the filename ends with '.lod' the lod file format will be used, else the binary file format will be used.")
def dumpfast(str_filename, address, nbwords)
    dumpBatch(str_filename, address, nbwords)
end

addHelpEntry("reading", "dumpsuperfast", "str_filename, address, nbwords", "bool", "Dumps 'nbwords' 32-bits words by library in batch read mode from memory, starting at address 'address', inside file 'str_filename'. If the filename ends with '.lod' the lod file format will be used, else the binary file format will be used.")
def dumpsuperfast(str_filename, address, nbwords)
    dumpByLibBatch(str_filename, address, nbwords)
end

def dumpClassic(str_filename, address, nbwords)
	puts "Dumping ..."
	lodorbin=false
	exten = str_filename.slice(-4..-1)
	exten = "" if(!exten)
	exten = exten.downcase
	lodorbin = true if(exten==".lod")

puts Benchmark.measure {
	File.open(str_filename,"wb") { |f|
	    data = address.RB(nbwords*4) {|prog| dumpProgress(prog)}
    	if(lodorbin)
    	    f << "@%08X\n" % address

    	    data.from8to32bits.each{ |w|
    	        f << "%08X\n" % w
    	    }
    	else
    	    f << data.pack("c*")
        end
	}
	puts "Done.\n"
}
end

def dumpByLib(str_filename, address, nbwords)
	puts "Dumping ..."
	lod0bin1=1
	exten = str_filename.slice(-4..-1)
	exten = "" if(!exten)
	exten = exten.downcase
	lod0bin1 = 0 if(exten==".lod")

puts Benchmark.measure {
	$CURRENTCONNECTION.dumpByLib(str_filename, address, nbwords, lod0bin1, false)
	puts "Done.\n"
}
end

def dumpByLibBatch(str_filename, address, nbwords)
	puts "Dumping ..."
	lod0bin1=1
	exten = str_filename.slice(-4..-1)
	exten = "" if(!exten)
	exten = exten.downcase
	lod0bin1 = 0 if(exten==".lod")

puts Benchmark.measure {
	$CURRENTCONNECTION.dumpByLib(str_filename, address, nbwords, lod0bin1, true)
	puts "Done.\n"
}
end

def dumpBatch(str_filename, address, nbwords)
	puts "Dumping ..."
	lodorbin=false
	exten = str_filename.slice(-4..-1)
	exten = "" if(!exten)
	exten = exten.downcase
	lodorbin = true if(exten==".lod")

puts Benchmark.measure {
	File.open(str_filename,"wb") { |f|
	    data = $CURRENTCONNECTION.batchRead(address, nbwords*4) {|prog| dumpProgress(prog)}
    	if(lodorbin)
    	    f << "@%08X\n" % address

    	    data.from8to32bits.each{ |w|
    	        f << "%08X\n" % w
    	    }
    	else
    	    f << data.pack("c*")
        end
	}
	puts "Done.\n"
}
end

addHelpEntry("reading", "fastdump", "str_filename, address, nbwords", "bool", "Dumps 'nbwords' 32-bits words from memory, starting at address 'address', inside file 'str_filename'. If the filename ends with '.lod' the lod file format will be used, else the binary file format will be used.")
def fastdump(str_filename,address,size)
    include SXS

    ret = [] 
    br = 921600    
    
	lodorbin=false
	exten = str_filename.slice(-4..-1)
	exten = "" if(!exten)
	exten = exten.downcase
	lodorbin = true if(exten==".lod")

    uartSxsConnection = CHBPConnection.new($CURRENTCONNECTION, [SXS_READ_RMC, SXS_DUMP_RMC, SXS_EXIT_RMC]) 
    uartSxsConnection.open(false)
    begin
        puts "Reading data..."
	    File.open(str_filename,"wb") { |f|    
		  ret = sxsRB(uartSxsConnection,address,4*size)  {|prog| dumpProgress(prog)}
    	  if(lodorbin)
    	    f << "@%08X\n" % address

    	    ret.from8to32bits.each{ |w|
    	        f << "%08X\n" % w
    	    }
    	  else
    	    f << ret.pack("c*")
          end
	    }		  
        puts "Done."
                       
    end
    
    return ret
    
end

###############################
#          BIN FILES          #
###############################

def ldVerifyBinary(connection,address,data)
    vfailed = false
    puts "Verifying (may take a while)..."
    readData = connection.readBlock(address,data.size)   
    if(readData != data)
        vfailed = true
        msg = ""
        readData.size.times { |i|
            if(readData[i]!=data[i])
                msg += "Verify failed! First failure address : 0x%08X" % (address+i)
                break
            end
        }
        raise LdVerifyFailed, msg
    end
    return true
end

def ldBinary(connection,address,data,bool_verify)
    connection.writeBlock(address,data)

    #Verifying
    if(bool_verify)
        success = ldVerifyBinary(connection,address,data)
        puts "Verify OK!" if(success)
    end   
    return true
end

addHelpEntry("writing","ldbin","address,str_filename,bool_verify=false","bool","Loads a binary file and verifies the loading operation if bool_verify==true")
def ldbin(address,str_filename,bool_verify=false)
    puts "Loading binary file ..."
    file = File.open(str_filename, "rb")
    data = file.read.unpack("C*")
    file.close
    ldBinary($CURRENTCONNECTION,address,data,bool_verify)
    puts "Done."
end

addHelpEntry("reading","verifybin","address,str_filename","bool","Verifies if the lod file 'str_filename' matches the memory")
def verifybin(address,str_filename)
    file = File.open(str_filename, "rb")
    data = file.read.unpack("C*")
    file.close
    success = ldVerifyBinary($CURRENTCONNECTION,address,data)
    puts "Verify OK!" if(success)
    return true
end
