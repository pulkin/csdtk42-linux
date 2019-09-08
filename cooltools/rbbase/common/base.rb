#!/usr/bin/env ruby

module CoolHostBase

protected
    begin
        FLOWMODE = FlowMode::FLOWMODE
    rescue Exception
        FLOWMODE = CoolHost::FLOWMODE_HARDWARE
    end
end

require 'coolhost.rb'
require 'coolhost_connections.rb'
require 'help.rb'
require 'timeout'
require 'lod.rb'

def coolwatcher?()
    return $COOLWATCHER
end


def errputs(str)      
    if(coolwatcher?)
        puts "html><table border=1 style='border-color:red'><tr><td width=100% bgcolor=#FFFFCC style='padding:2px'><img src='resources/pirate.png'> <font color=red>#{str}</font> </td></tr></table>"
    else
        puts str
    end
end

def wputs(str)      
    if(coolwatcher?)
        puts "html><table border=1 style='border-color:red'><tr><td width=100% bgcolor=#FFFFCC style='padding:2px'><img src='resources/error16.png'> <font color=red>#{str}</font> </td></tr></table>"
    else
        puts str
    end
end

def okputs(str)      
    if(coolwatcher?)
        puts "html><table border=1 style='border-color:darkgreen'><tr><td width=100% bgcolor=#a0e684 style='padding:2px'><img src='resources/ok16.png'> <font color=darkgreen>#{str}</font> </td></tr></table>"
    else
        puts str
    end
end

def enforce()
    begin
        yield()
    rescue Exception
    end
end


##########################################
#	SHORTCUTS METHODS FOR BASIC CLASSES	 #
##########################################

class CHXMLElement
    def parent()
        return eval(self.parentPath())
    end
end


class CHValuable

    def read(connection,forced=false)
        raise RegisterReadProtected,"#{path()}: Read protected!" if(!forced && self.readProtected?())
        val = 0
        vdata = []
        if(self.internal?())
            val = connection.readInternal(self.address())
            vdata = [val]
        else
            case sizeof()
                when 1:
                    val = connection.read8(self.address)
                    vdata = [val]
                when 2:
                    val = connection.read16(self.address)
                    vdata = [val].from16to8bits()
                when 4:
                    val = connection.read32(self.address)
                    vdata = [val].from32to8bits()
            end
        end
        self.memsetl(vdata)
        return self.getl
    end

    def write(connection,val,forced=false)
        raise RegisterWriteProtected,"#{path()}: Write protected!" if(!forced && self.writeProtected?())
        if(self.internal?())
            connection.writeInternal(self.address(),val)
        else
            case sizeof()
                when 1:
                    val = connection.write8(self.address,val)
                when 2:
                    val = connection.write16(self.address,val)
                when 4:
                    val = connection.write32(self.address,val)
            end
        end
    end

	def R
		self.read($CURRENTCONNECTION)
	end

	def r
	    val = self.R
		puts "0x%0#{sizeof()*2}X" % val
		return val
	end

	def RU
		R() & 0xFFFFFFFF
	end

	def ru
	    val = self.RU
		puts "0x%0#{sizeof()*2}X" % val
		return val
	end

	def RF
		self.read($CURRENTCONNECTION,true)
	end
	
	def rf
	    val = self.RF
		puts "0x%0#{sizeof()*2}X" % val
		return val
	end
	
	def w(val)
		self.write($CURRENTCONNECTION,val)
	end
	
	def wf(val)
		self.write($CURRENTCONNECTION,val,true)	
	end
end

class CHPointer
protected
    def dereferenceLocal()
	    raise NullPointer, "Trying to dereference Null Pointer!" if(self.getl==0)
		return self.derefAt(self.getl)   
    end

    def readAndDerefence(connection)
 	    self.read(connection)
	    return dereferenceLocal()
    end

public
    # Local dereferencing (using value in cache)
    def >>()
        return dereferenceLocal()
    end

    # Read and dereference
	def >(connection = $CURRENTCONNECTION)
        return readAndDerefence(connection)
	end
end

class CHStruct

    def readContent(connection)
        if(hasInternals())
            a = []
            members.each{ |reg|
                a << reg.R
            }
            return a
        else
            res = connection.readBlockMasked(address(),sizeof(),mask())
            self.memsetl(res)
            return res
        end
    end

    def writeContent(connection)
        if(hasInternals())
            members.each{ |reg|
                reg.W(reg.getl)
            }      
        else
            connection.writeBlock(address(),self.memgetl())
        end
    end
end

class CHArray

    def readContent(connection)
        res = connection.readBlockMasked(address(),sizeof(),mask())
        self.memsetl(res)
        return res
    end
    
    def writeContent(connection)
        connection.writeBlock(address(),self.memgetl())
    end    
end

class CHBitfield

public
    def readModifyWrite(connection,val,forced=false)
        #Read, modify, write
        raise RegisterWriteProtected,"#{path()}: Parent is write protected!" if(!forced && self.parent().writeProtected?())
        raise RegisterReadProtected, "#{path()}: Parent is read protected!" if(!forced && self.parent().readProtected?())
 
        pval = self.parent.read(connection,forced) # Parent Value
        pval = self.parent.prepl(pval)           # Clear set/clear fields if needed.
        mval = self.wl(pval,val)                 # Modified Value, transform it from bitfield base to register base
        self.parent().write(connection,mval,forced)    
    end
    
    def readLocalModifyWrite(connection,val,forced=false)
        raise RegisterWriteProtected,"#{path()}: Parent is write protected!" if(!forced && self.parent().writeProtected?())
 
        pval = self.parent.getl
        pval = self.parent.prepl(pval)           # Clear set/clear fields if needed.
        mval = self.wl(pval,val)                 # Modified Value, transform it from bitfield base to register base
        self.parent().write(connection,mval,forced)    
    end

    def read(connection,forced=false)
        raise BitfieldReadProtected,"#{path()}: Read protected!" if(!forced && self.readProtected?())
        raise RegisterReadProtected,"#{path()}: Parent is read protected!" if(!forced && self.parent().readProtected?())
    
        val = self.parent().read(connection,forced)
        return rl(val)  #Transform value from register base to bitfield base
    end

    def write(connection,val,forced=false)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is SETTABLE, not WRITABLE." if(!forced && writeAccess() == WASet)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is CLEARABLE, not WRITABLE." if(!forced && writeAccess() == WAClear)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is write protected!" if(!forced && writeAccess() == WANone)
        readModifyWrite(connection,val,forced)
    end
    
    def set!(connection,forced=false)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is WRITABLE, not SETTABLE." if(!forced && writeAccess() == WAWrite)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is CLEARABLE, not SETTABLE." if(!forced && writeAccess() == WAClear)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is write protected!" if(!forced && writeAccess() == WANone)
        readModifyWrite(connection,0xFFFFFFFF,forced) 
    end
    
    def clear!(connection,forced=false)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is WRITABLE, not CLEARABLE." if(!forced && writeAccess() == WAWrite)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is SETTABLE, not CLEARABLE." if(!forced && writeAccess() == WASet)
        raise BitfieldWriteProtected, "#{path()}: Bitfield is write protected!" if(!forced && writeAccess() == WANone)
        readModifyWrite(connection,0xFFFFFFFF,forced)
    end    

	def R
		self.read($CURRENTCONNECTION)
	end

	def r
		val = self.R
		puts self.svalue()
		return val
	end

	def RU
		self.R & 0xFFFFFFFF
	end

	def RF
		self.read($CURRENTCONNECTION,true)
	end
	
	def rf
		val = self.RF
		puts self.svalue()		
		return val
	end
	
	def w(val)
		self.write($CURRENTCONNECTION,val)
	end
	
	def wf(val)
		self.write($CURRENTCONNECTION,val,true)	
	end	
	
	def set()
	    self.set!($CURRENTCONNECTION)
    end
	
	def setf()
	    self.set!($CURRENTCONNECTION,true)
    end

	def clear()
	    self.clear!($CURRENTCONNECTION)
    end
    
    def clearf()
        self.clear!($CURRENTCONNECTION,true)
    end
    
end


module CoolHostBase

public 

#############################
#		UTILITIES			#
#############################
def xp(arg1)
	puts "0x%08X" % arg1
end

def bp(arg1)
    puts "0b%032b" % arg1
end

#############################
# COOLHOST BASIC FUNCTIONS  #
#############################	

def cstatus()
    puts "You have currently #{$COOLHOSTCONNECTIONS.size()} connections registered."
    puts "These are:"
    $COOLHOSTCONNECTIONS.each() { |k,v| 
        puts v.name   
    }
    if($COOLHOSTCONNECTIONS.size()>10)
        puts "More than 10 connections opened? Maybe you have a connection leak in a script."
    end
end

def cclear()
    puts "Closing all registered connections."
    $COOLHOSTCONNECTIONS.each() { |k,v| 
        v.close
    }    
    puts "Done. You should do a copen, uopen, or whatever to make CoolWatcher work again."
end

def gopen(abortOnError=true, replaceTraceConnection=true)
    begin
        cwDisableAutoPoll()
    rescue Exception
    end
    
    goterror = false
    
    $CURRENTCONNECTION.close() # Close old connection.
    $CURRENTCONNECTION = yield()
    $CURRENTCONNECTION.setName("$CURRENTCONNECTION")
    begin
        $CURRENTCONNECTION.open(true) # Force coolhost reconnection
        okputs "Connection #{$CURRENTCONNECTION.name} opened (#CURRENTCONNECTION)."
    rescue Exception => e
        errputs "Connection #{$CURRENTCONNECTION.name} could not be opened. <br> Reason: #{e.message} #{e.class}."
        goterror = true
        if (abortOnError)
            puts "Aborted."
            return goterror
        end
    end
    if $CURRENTCONNECTION.implementsUsb()
        # If the phone is in GDB, it will take longer to setup the first USB connection
        sleep CHUSBConnection::OPEN_STABLE_TIME
    end
    $TRACEPLUGCONNECTION = $CURRENTCONNECTION if(replaceTraceConnection)
    $TOOLCONNECTIONS.each{ |k,v|
        if (!v.recreate())
            goterror = true
            if (abortOnError)
                puts "Aborted."
                return goterror
            end
        end
    }
    puts "Done."
    return goterror
end

addHelpEntry("com","copen","NUM,BR=BR_AUTOMATIC,abortOnError=true","$CURRENTCONNECTION","Opens a CoolHost or a socket to a CoolHost on the COM port 'NUM' and with the baudrate 'BR'. Returns the constructed connection. Caution : if successful, this function modifies $CURRENTCONNECTION.")
def copen(arg1,arg2=BR_AUTOMATIC,abortOnError=true)
    goterror = gopen(abortOnError) {CHHostConnection.new(arg1,arg2,CoolHostBase::FLOWMODE)}
    return !goterror
end

addHelpEntry("com","uopen","abortOnError=true","$CURRENTCONNECTION","Opens a CoolHost or a socket to a CoolHost on the USB (TODO : this function needs more parameters). Returns the constructed connection. Caution : if successful, this function modifies $CURRENTCONNECTION.")
def uopen(abortOnError=true)
    goterror = gopen(abortOnError){CHUSBConnection.new()}
    return !goterror
end

addHelpEntry("com","jopen","NUM,BR=115200,abortOnError=true","$CURRENTCONNECTION","Opens a jadeCoolHost or a socket to a jadeCoolHost on the COM port 'NUM' and with the baudrate 'BR'. Returns the constructed connection. Caution : if successful, this function modifies $CURRENTCONNECTION.")
def jopen(arg1,arg2=115200,abortOnError=true)
    goterror = gopen(abortOnError, false){CHJConnection.new(arg1,arg2)}
    return !goterror
end

addHelpEntry("com","jtopen","NUM,BR=460800","$TRACEPLUGCONNECTION","Opens a coolhost for managing traces for jade.")
def jtopen(arg1,arg2=460800)
	$TRACEPLUGCONNECTION = CHHostConnection.new(arg1,arg2)
    begin
        $TRACEPLUGCONNECTION.open(false)
        okputs "Connection #{$TRACEPLUGCONNECTION.name} opened (#TRACECONNECTION)."
    rescue Exception => e
        errputs "Connection #{$TRACEPLUGCONNECTION.name} could not be opened. Reason: #{e.message} #{e.name}."
    end
    return $TRACEPLUGCONNECTION
end


#HIDDEN SECRET CONCEILED FUNCTION
addHelpEntry("com","reop","abortOnError=true","$CURRENTCONNECTION","Re-opens the current com port.")
def reop(abortOnError=true)
    puts "Reopening all registered connections (#{$COOLHOSTCONNECTIONS.size} to reopen)..."
    toreop = $COOLHOSTCONNECTIONS.clone
    toreop.each() { |k,v| 
        begin
            v.reopen() 
            okputs "Connection #{v.name} reopened."
            if $CURRENTCONNECTION.implementsUsb()
                # If the phone is in GDB, it will take longer to reopen all the USB connections
                sleep CHUSBConnection::OPEN_STABLE_TIME
            end
        rescue Exception
            errputs "Connection #{v.name} failed to be reopened."
            if (abortOnError)
                puts "Aborted."
                return
            end
        end
    }
    puts "Done."
end

addHelpEntry("com","ckill","","$CURRENTCONNECTION","Quits the CoolHost program handling the current com port.")
def ckill()
    $CURRENTCONNECTION.killCoolhost()
end

def killAllCoolhosts()
    $COOLHOSTCONNECTIONS.each() { |k,v| v.killCoolhost() }    
end

addHelpEntry("com","cbreak","","$CURRENTCONNECTION","Send a serial break on the current com port.")
def cbreak()
    $CURRENTCONNECTION.comBreak()
end


addHelpEntry("com","cclose","","","Closes the socket to the CoolHost corresponding to $CURRENTCONNECTION.")
def cclose()
	$CURRENTCONNECTION.close()
end

def chipComEnable(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

addHelpEntry("com","creconnect","","","Reconnect current com port.")
def creconnect()
    puts "Breaking COM ..."
    cbreak()
    sleep(2)
    chipComEnable($CURRENTCONNECTION)
    puts "Done."
end

addHelpEntry("com","showAllEvents","enable=1","","Print a event once it is popped out.")
def showAllEvents(enable=1)
    if(enable == 1)
        CHConnection.showAllEvents(true)
        puts "showAllEvents ENABLED"
    else
        CHConnection.showAllEvents(false)
        puts "showAllEvents DISABLED"
    end
end

#############################
#	   READ FUNCTIONS		#
#############################

addHelpEntry("reading","r","addr","val","Reads a 32-bit word at address 'addr'. Prints the result in hex.")
addHelpEntry("reading","R","addr","val","Reads a 32-bit word at address 'addr'. Does not print the result.")
def R(arg1) 
	if(arg1.is_a?(CHRegister)) then
		arg1.R
	elsif(arg1.is_a?(CHBitfield)) then
		arg1.R
	else
		$CURRENTCONNECTION.read32(arg1)
	end
end

def r(arg1) 
	if(arg1.is_a?(CHRegister)) then
		arg1.r
	elsif(arg1.is_a?(CHBitfield)) then
		arg1.r
	else
		a = $CURRENTCONNECTION.read32(arg1)
		puts "0x%08X" % a
		a
	end
end

def RU(arg1) 
    R(arg1) & 0xFFFFFFFF
end

def ru(arg1) 
    r(arg1) & 0xFFFFFFFF
end


#Read forced (force read on registers and bitfields when protected)
def rf(arg1)
	if(arg1.is_a?(CHRegister)) then
		arg1.rf
	elsif(arg1.is_a?(CHBitfield)) then
		arg1.rf
	else
		r(arg1)
	end	
end

########## READ32 ###########
addHelpEntry("reading","r32","addr","val","Reads a 32-bit word at address 'addr'. Prints the result in hex.")
addHelpEntry("reading","R32","addr","val","Reads a 32-bit word at address 'addr'. Does not print the result.")
def r32(arg1) 
	r(arg1)
end

def R32(arg1) 
	R(arg1)
end

########## READ16 ###########
addHelpEntry("reading","r16","addr","val","Reads a 16-bit word at address 'addr'. Prints the result in hex.")
addHelpEntry("reading","R16","addr","val","Reads a 16-bit word at address 'addr'. Does not print the result.")
def R16(arg1) 
	$CURRENTCONNECTION.read16(arg1)
end

def r16(arg1) 
	a = R16(arg1)
	puts "0x%04X" % a
	a
end

########## READ8 ###########
addHelpEntry("reading","r8","addr","val","Reads a 16-bit word at address 'addr'. Prints the result in hex.")
addHelpEntry("reading","R8","addr","val","Reads a 16-bit word at address 'addr'. Does not print the result.")
def R8(arg1) 
	$CURRENTCONNECTION.read8(arg1)
end

def r8(arg1) 
	a = R8(arg1)
	puts "0x%02X" % a
	a
end

####### READ INTERNAL ######
addHelpEntry("reading","ri","innerreg","val","Reads the value of the internal register 'innerreg'. Prints the result in hex.")
addHelpEntry("reading","RI","innerreg","val","Reads the value of the internal register 'innerreg'. Does not print the result.")
def RI(arg1) 
	$CURRENTCONNECTION.readInternal(arg1)
end

def ri(arg1) 
	a = RI(arg1)
	puts "0x%02X" % a
	a
end

######## READBLOCK #########
addHelpEntry("reading","rb","addr,bytes_to_read","[array_of_bytes]","Reads a block of 'bytestoread' bytes at address 'addr'. Prints the result in hex.")
addHelpEntry("reading","RB","addr,bytes_to_read","[array_of_bytes]","Reads a block of 'bytestoread' bytes at address 'addr'. Does not print the result.")

def RB(address,bytestoread)
	$CURRENTCONNECTION.readBlock(address,bytestoread) {|f| yield(f) if(block_given?()) }
end

def rb(address,bytestoread)
	values = RB(address,bytestoread) {|f| yield(f) if(block_given?()) }
	vprint = values.clone
	
	#complete first bytes
	firstbytes = address%4
	firstbytes.times {vprint.unshift ".."}
	
	#complete last bytes
	if(vprint.size()%4 != 0)
	    (4 -(vprint.size()%4)).times { vprint << ".." }
    end

	(vprint.size/4).times { |val|
        str = "0x"
        3.downto(0) { |i|
            str += (vprint[4*val+i]!="..")?("%02X" % vprint[4*val+i]):".."
        }
        puts str
	}
        	
	values
end

######## READSTRING #########
addHelpEntry("reading","rs","addr,bytes_to_read","string","Reads a string of max size 'bytestoread' bytes at address 'addr'. Prints the string.")
addHelpEntry("reading","RS","addr,bytes_to_read","string","Reads a string of max size 'bytestoread' bytes at address 'addr' Does not print the result.")

def RS(address,bytestoread)
    $CURRENTCONNECTION.readString(address,bytestoread)
end

def rs(address,bytestoread)
    puts RS(address,bytestoread)
end

#############################
#	   WRITE FUNCTIONS		#
#############################

addHelpEntry("writing","w","addr,val","","Writes a 32-bit word at address 'addr'.")
def w(address,val) 
	if(address.is_a?(CHRegister)) then
		address.w(val)
	elsif(address.is_a?(CHBitfield)) then
		address.w(val)
	else
		$CURRENTCONNECTION.write32(address,val)
	end
end

#Read forced (force read on registers and bitfields when protected)
def wf(address,val)
	if(address.is_a?(CHRegister)) then
		address.wf(val)
	elsif(address.is_a?(CHBitfield)) then
		address.wf(val)
	else
		w(address,val)
	end	
end

########## WRITE32 ###########
addHelpEntry("writing","w32","addr,val","","Writes a 32-bit word at address 'addr'.")
def w32(address,val) 
	w(address,val)
end

########## WRITE16 ###########
addHelpEntry("writing","w16","addr,val","","Writes a 16-bit word at address 'addr'.")
def w16(address,val) 
	$CURRENTCONNECTION.write16(address,val)
end

########## WRITE8 ###########
addHelpEntry("writing","w8","addr,val","","Writes a 8-bit word at address 'addr'.")
def w8(address,val) 
	$CURRENTCONNECTION.write8(address,val)
end

####### READ INTERNAL ######
addHelpEntry("writing","wi","innerreg,val","","Writes a 8-bit word in the internal register 'innerreg'.")
def wi(innerreg,val) 
	$CURRENTCONNECTION.writeInternal(innerreg,val)
end

######## WRITEBLOCK #########
addHelpEntry("writing","wb","addr,[array_of_bytes]","","Writes a block of bytes (each element of the array is a byte) at address addr.'")
def wb(address,array)
	$CURRENTCONNECTION.writeBlock(address,array)
end

##############################
#		CHIP FUNCTIONS		 #
##############################
# Next 2 functions are overloaded by the chip base.rb specific script
def chipRestart(connection, bool_xcpufreeze, bool_powerStayOn, bool_reEnableUart, bool_intRegOnly)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipUnfreeze(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

addHelpEntry("chip","crestart","connection=$CURRENTCONNECTION,bool_xcpufreeze=false,bool_powerStayOn=false,bool_reEnableUart=true,bool_intRegOnly=false","","Same as restart except that it takes a connection as an additional parameter, in order to perform a reset on whatever connection you want.")
def crestart(connection=$CURRENTCONNECTION, bool_xcpufreeze=false, bool_powerStayOn=false, bool_reEnableUart=true, bool_intRegOnly=false)
    chipRestart(connection,bool_xcpufreeze,bool_powerStayOn,bool_reEnableUart,bool_intRegOnly)
end

addHelpEntry("chip","restart","bool_xcpufreeze=false,bool_powerStayOn=false,bool_reEnableUart=true,bool_intRegOnly=false","","Restarts the chip. If 'bool_xcpufreeze' is true, it is frozen until a call to 'unfreeze'.\n\tIf 'bool_powerStayOn' is true, the power is kept on during the restart. If 'bool_reEnableUart' is true, the trace Uart will be reenabled after restart. If 'bool_intRegOnly' is true, only debug host internal registers will be accessed during the restart.")
def restart(bool_xcpufreeze=false,bool_powerStayOn=false,bool_reEnableUart=true,bool_intRegOnly=false)
    crestart($CURRENTCONNECTION,bool_xcpufreeze,bool_powerStayOn,bool_reEnableUart,bool_intRegOnly)
end

addHelpEntry("chip","rst","bool_xcpufreeze=false,bool_powerStayOn=false,bool_reEnableUart=true","","Restarts the chip. If 'bool_xcpufreeze' is true, it is frozen until a call to 'unfreeze'.\n\tIf 'bool_powerStayOn' is true, the power is kept on during the restart.  If 'bool_reEnableUart' is true, the trace Uart will be reenabled after restart.")
def rst(bool_xcpufreeze=false,bool_powerStayOn=false,bool_reEnableUart=true)
    crestart($CURRENTCONNECTION,bool_xcpufreeze,bool_powerStayOn,bool_reEnableUart,false)
end

addHelpEntry("chip","dbgrst","","","Restarts the chip by accessing debug host internal registers only. Xcpu will be unfrozen, the power will be kept on during the restart, and the trace Uart will be reenabled after restart.")
def dbgrst()
    crestart($CURRENTCONNECTION,false,false,true,true)
end

addHelpEntry("chip","cunfreeze","connection=$CURRENTCONNECTION","","Same as 'unfreeze' except that you can chose the connection on which to perform the unfreeze.")
def cunfreeze(connection=$CURRENTCONNECTION)
    chipUnfreeze(connection)
end

addHelpEntry("chip","unfreeze","","","Unfreezes the chip.")
def unfreeze()
    cunfreeze($CURRENTCONNECTION)
end

def chipBusReset(connection,bool_powerStayOn)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

addHelpEntry("chip","busreset","","","Reset the whole chip but halt XCPU - the memory content survives.")
def busreset(bool_powerStayOn=false)
    puts "Breaking COM ..."
    cbreak()
    sleep(2)
    chipBusReset($CURRENTCONNECTION,bool_powerStayOn)
    puts "Done."
end

##############################
#		BREAKP FUNCTIONS	 #
##############################

# Next 6 functions are overloaded by the chip base.rb specific script
def chipXfbp(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipXrbp(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipXcbp(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipXDbgIrq(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipBfbp(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipBrbp(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipBcbp(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

def chipBDbgIrq(connection)
    raise "FUNCTION HAS TO BE OVERLOADED!"
end

addHelpEntry("breakpoint","xfbpInfinite","","","Send command to force XCPU breakpoint infinitely.")
def xfbpInfinite()
    loop do
        chipXfbp($CURRENTCONNECTION)
    end
end

addHelpEntry("breakpoint","xfbp","","","Force XCPU breakpoint.")
def xfbp()
	chipXfbp($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","xrbp","","","Release XCPU breakpoint (forced only).")
def xrbp()
	chipXrbp($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","xcbp","","","Check XCPU breakpoint.")
def xcbp()
	chipXcbp($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","bfbp","","","Force BCPU breakpoint.")
def bfbp()
	chipBfbp($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","brbp","","","Release BCPU breakpoint (forced only).")
def brbp()
	chipBrbp($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","bcbp","","","Check BCPU breakpoint.")
def bcbp()
	chipBcbp($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","xgdb","","","Set XCPU Debug IRQ.")
def xgdb()
    chipXDbgIrq($CURRENTCONNECTION)
end

addHelpEntry("breakpoint","bgdb","","","Set BCPU Debug IRQ.")
def bgdb()
    chipBDbgIrq($CURRENTCONNECTION)
end

$CRVERBOSE = 1
#For Debugging purpose
def CRVERBOSE(str,level=1)
    if($CRVERBOSE>=level)
        puts str
    end
end

end #end module CoolHostBase

#################################
#	 Array Class Enhancement	#
#################################

class Array

    def from8to32bits()
        a = []
        lastAligned = (self.length()/4)*4
        0.step(lastAligned-1,4) { |i|
            a << (self[i] | (self[i+1] << 8) | (self[i+2] << 16) | (self[i+3] << 24) )
        }
        
        if(lastAligned!=self.length())
            last = 0
            last |= self[lastAligned]         if(self[lastAligned])
            last |= (self[lastAligned+1]<<8)    if(self[lastAligned+1])
            last |= (self[lastAligned+2]<<16)   if(self[lastAligned+2])
            a << last
        end
                
        return a
    end
    
    def from8to16bits()
        a = []
        lastAligned = (self.length()/2)*2
        0.step(lastAligned-1,2) { |i|
            a << (self[i] | (self[i+1] << 8))
        }  
        if(lastAligned!=self.length())
            last = 0
            last |= self[lastAligned]         if(self[lastAligned])
            a << last
        end        
        return a
    end   
     
    def from32to8bits()
        a = []
        self.each() { |e|
            a << (e & 0xFF)
            a << ((e>>8) & 0xFF)
            a << ((e>>16) & 0xFF)
            a << ((e>>24) & 0xFF)
        }
        return a
    end
 
     def from16to8bits()
        a = []
        self.each() { |e|
            a << (e & 0xFF)
            a << ((e>>8) & 0xFF)
        }
        return a
    end   

end


#################################
#	Integer Class Enhancement	#
#################################

#Add a read block method to the Integer class
class Integer
	addHelpEntry("reading","Integer.rb","bytestoread","[array_of_bytes]","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def rb(wordsToRead)
		CoolHostBase.rb(self,wordsToRead) {|f| yield(f) if(block_given?()) }
	end
	
	addHelpEntry("reading","Integer.RB","bytestoread","[array_of_bytes]","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def RB(wordsToRead)
		CoolHostBase.RB(self,wordsToRead) {|f| yield(f) if(block_given?()) }
	end
	
	addHelpEntry("reading","Integer.r","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def r
		CoolHostBase.r(self)
	end
	
	addHelpEntry("reading","Integer.R","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def R
		CoolHostBase.R(self)
	end

	addHelpEntry("reading","Integer.ru","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def ru
		CoolHostBase.ru(self)
	end
	
	addHelpEntry("reading","Integer.RU","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def RU
		CoolHostBase.RU(self)
    end

	addHelpEntry("reading","Integer.r8","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def r8
		CoolHostBase.r8(self)
	end
	
	addHelpEntry("reading","Integer.R8","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def R8
		CoolHostBase.R8(self)
	end
	
	addHelpEntry("reading","Integer.r16","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def r16
		CoolHostBase.r16(self)
	end
	
	addHelpEntry("reading","Integer.R16","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def R16
		CoolHostBase.R16(self)
	end
	
	addHelpEntry("reading","Integer.r32","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def r32
		CoolHostBase.r32(self)
	end
	
	addHelpEntry("reading","Integer.R32","","value","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def R32
		CoolHostBase.R32(self)
	end	
	
	addHelpEntry("reading","Integer.rs","bytestoread","string","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def rs(bytestoread)
	    CoolHostBase.rs(self,bytestoread)
    end
	
	addHelpEntry("reading","Integer.rs","bytestoread","string","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def RS(bytestoread)
	    CoolHostBase.RS(self,bytestoread)
    end	
	
	addHelpEntry("writing","Integer.wb","[array_of_bytes]","","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def wb(array)
		CoolHostBase.wb(self,array)
	end
	
	addHelpEntry("writing","Integer.w","val","","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def w(val)
		CoolHostBase.w(self,val)
	end
	
	addHelpEntry("writing","Integer.w8","val","","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def w8(val)
		CoolHostBase.w8(self,val)
	end
	
	addHelpEntry("writing","Integer.w16","val","","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def w16(val)
		CoolHostBase.w16(self,val)
	end
	
	addHelpEntry("writing","Integer.w32","val","","This method is provided for convenience. See corresponding function (the Integer value is the address where this operation is performed)",false)
	def w32(val)
		CoolHostBase.w32(self,val)
	end
	
end


include CoolHostBase
