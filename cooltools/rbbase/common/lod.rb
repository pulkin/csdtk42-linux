#############################################################
#															#
#    Lod Utilies for reading and writing lod files          #
#															#
#############################################################
require 'rexml/document'
require 'elflib'
include REXML

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

def dump_start_rst()
        # If the chip has AP, the calib mode is started by AP
        if(chipHasAp?())
            enterCalib()
            return
        end

        require "target_executor"
        begin
            tgxtor = TargetXtor.new()
            tgxtor.enterHostUSBMonitor()

            # Address of the entry point, where to jump to enter the calib stub.
            w(0xa1a25000,0x66)
            w(0xa1a25000,0x99)
            sleep 1
            #tgxtor.targetExecuteAndJump($boot_sector_code.address, 0, @@C::CALIB_MAGIC_TAG)
        ensure
            tgxtor.closeConnection()
        end
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
    
    #return ret
    
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

addHelpEntry("writing","ldbin","address,str_filename,bool_verify=false","bool","Loads a binary file and verifies the loading operation if bool_verify==true. 
for example 8809 and 8809e ldbin 0xa1e00000,xcpu.bin  ldbin 0xa1e80000, bcpu.bin ")
def ldbin(address,str_filename,bool_verify=false)
    puts "Loading binary file ..."
    file = File.open(str_filename, "rb")
    data = file.read.unpack("C*")
    file.close
    ldBinary($CURRENTCONNECTION,address,data,bool_verify)
    puts "Done."
end
addHelpEntry("writing","ldbinxcpu","str_filename","bool","ldbin 0xa1e00000,xcpu.bin ")
def ldbinxcpu(str_filename)
    puts "Loading binary file ..."
    file = File.open(str_filename, "rb")
    data = file.read.unpack("C*")
    file.close
    ldBinary($CURRENTCONNECTION,0xa1e00000,data,false)
    puts "Done."
end
addHelpEntry("writing","ldbinbcpu","str_filename","bool","ldbin 0xa1e80000,bcpu.bin ")
def ldbinbcpu(str_filename)
    puts "Loading binary file ..."
    file = File.open(str_filename, "rb")
    data = file.read.unpack("C*")
    file.close
    ldBinary($CURRENTCONNECTION,0xa1e80000,data,false)
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

###############################
#          ELF UTILES          #
###############################

   class PhdrAddrAligh
      ALIGN=0x1000
      OFFSET=0x1000
   end

  def elf_headerSize(h)
    h_size = 0
    h.instance_variables.each do |varname| 
      h_size += h.instance_variable_get(varname).size
    end
    return h_size
  end
  
  addHelpEntry("reading","elfdump","str_filename,xml_fileneme","bool","dump memory according to the information in 'xml_fileneme' and build elf file 'str_filename' from the memory")
  def elfdump(str_filename="xcpucore.elf", xml_fileneme="")
    if xml_fileneme == ""
      xml_fileneme = cwGetProfileEntry("elfdumpSetting", "chipgen/Modem2G/toolpool/map/elfdump/default.xml")
    end

    xmlfile = File.new(xml_fileneme)
    xmldoc = Document.new(xmlfile)
    xmlfile.close
    # Now get the root element
    root = xmldoc.root
    puts "building elf for " + root.attributes["elfdump"] +"..."
    
    mem_size = PhdrAddrAligh::ALIGN
    
    root.elements.each {|e| 
           size=e.elements["size"].text.to_i(16)
            if ((size % PhdrAddrAligh::ALIGN) != 0)
                
                size += size % PhdrAddrAligh::ALIGN
            end
            mem_size += size;
    }
    
    elf = 0x0.chr*mem_size;
    phdr = Program_Header.new()
    ehdr = Elf_Header.new()
    shdr = Section_Header.new()
    
    ehdr_size = elf_headerSize(ehdr)
    phdr_size = elf_headerSize(phdr)
    shdr_size = elf_headerSize(shdr)
    
    ehdr.write_ident 1,1,1
    ehdr.write_version 1
    ehdr.write_machine 8
    ehdr.write_type 4
    ehdr.write_entry 0
    ehdr.write_offsets ehdr_size, ehdr_size+phdr_size*root.elements.size() 
    ehdr.write_flags 0x48f1001
    ehdr.write_sizes ehdr_size, phdr_size, shdr_size
    ehdr.write_nums root.elements.size(), 1
    ehdr.write_strndx 0  
    ehdr_h = ehdr.get_header
    elf[0..ehdr_size-1] = ehdr_h

    mem_offset = PhdrAddrAligh::OFFSET

    phdr_h = ""
    
    root.elements.each {|e| 
       mem_addr = e.elements["base"].text.to_i(16)
       size = e.elements["size"].text.to_i(16)
       skip = 0
       if e.elements["skip"]
       	skip = e.elements["skip"].text.to_i 
       end
       if skip == 0
           puts  "Reading #{e.attributes["title"]} @0x#{mem_addr.to_s(16)}..."
           data = $CURRENTCONNECTION.batchRead(mem_addr, size) {|prog| dumpProgress(prog)}
           puts  "done"
       else
           data=Array.new(size,0)
           if e.elements["rom"]
	           begin
	           File.open(e.elements["rom"].text, "rb")do |f|
	                 puts "reading #{e.attributes["title"]} from #{e.elements["rom"].text}..."
	                 str = f.read()
	                  i = 0
	                  loopsize = size
	                  if(str.size < size)
			      loopsize = str.size()
	                  end
	                  while (i < loopsize)
	                     data[i] = str[i]
	                     i+=1
	                  end
	           end
	           rescue
	              puts "...Can't open file in section #{e.attributes["title"]},skip" 
	           end
	     else
	         puts "Skip reading #{e.attributes["title"]}"
	     end
       end
       elf[mem_offset..mem_offset+size-1]=data.pack("c*")
       dumpfilename=e.attributes["title"]+".bin"
       while dumpfilename[" "]
       	dumpfilename[" "]="_"
       end
       if skip == 0
           myfile=File.open(dumpfilename, "wb")
           myfile << data.pack("c*")
           myfile.close
       end
	phdr.write_type 1
	phdr.write_offset mem_offset
	phdr.write_flags 7
	phdr.write_addr mem_addr, mem_addr
	phdr.write_fsize size
	phdr.write_msize size
	phdr.write_align PhdrAddrAligh::ALIGN
	phdr_h += phdr.get_header 

	mem_offset += size
	
	if ((mem_offset % PhdrAddrAligh::ALIGN) != 0)
		mem_offset += mem_offset % PhdrAddrAligh::ALIGN
	end
    }

    elf[ehdr_size..ehdr_size+phdr_size*root.elements.size()-1]=phdr_h
    puts  "building #{str_filename}..."
    df = File.open(str_filename, "wb")do |f|
        f.puts elf
    end
    
    puts  "done"

  end
