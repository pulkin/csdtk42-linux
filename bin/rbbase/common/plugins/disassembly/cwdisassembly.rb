cwRegisterPlugin(File.dirname(__FILE__)+"/disassemblyplugin.dll")

if(!$CWDISLIST)
    $CWDISLIST = nil
    $CWDISLISTTIME = nil
end

class NoDisassemblyLoaded < Exception
end


include CoolDisassembly

addHelpEntry("disassembly", "cwUnloadDisassembly", "", "", "Unload the current disassembled code. Keep track of it, in case we should reload it in the futur (during a fastpf, for instance).")
def cwUnloadDisassembly()
    cwLoadDisassembly(nil)
end

addHelpEntry("disassembly", "cwLoadDisassembly", "", "", "Load the disassembled code from a .dis file. If nil is given, the current disassembly is unloaded. The disassembly is used to retrieve the name of a function out of a word of code's address.")
def cwLoadDisassembly(filename)
    if(filename.nil?)
        unloadDisassembly()
        $CWDISLIST = nil
        return
    end
    
    if( File.exists?(filename) && (File.mtime(filename)==$CWDISLISTTIME) && (!$CWDISLIST.nil?) )
        puts "[Disassembly loading not needed.]"
        return
    end
    
    # Unload old disassembly
    unloadDisassembly()
    $CWDISLIST      = loadDisassembly(filename) { |p| cwSetProgress(p,99,"%p% (.dis load)") }
    $CWDISLISTTIME  = File.mtime(filename)
    $CWDISLIST.setActiveInWidget()
end



def cwBeautifulDisassembly(address, scope=8)
    str = $CWDISLIST.disassemblyAt(address)
    lines = str.split("\n")
    linesarray = []
    
    #Extract disassembly info from lines
    lines.each{ |l|
        l =~ /([0-9a-f]*):\t([^\t]*)?\t(.*)/
        if($1)
            linesarray << [$1.hex,$2,$3.strip.gsub("\t"," ").gsub(">","&gt;").gsub("<","&lt;"),false] 
        end
    }

    #Search line corresponding to address
    goodindex = 0
    linesarray.each{ |l|
        if(linesarray[goodindex][0]>address)
            goodindex -= 1 if(goodindex>0)
            linesarray[goodindex][3]=true
            break
        else
            goodindex += 1
        end
    }

    #Build scope
    start       = [goodindex-scope,0].max
    stop        = [linesarray.size()-1,goodindex+scope].min
    filteredarray = linesarray[start..stop]
    
    #Print the beautiful result
    puts "html><br>In function <font color=red>%s:</font><br>" % $CWDISLIST.functionAt(address)
    res = "html><table>"
    res+= "<tr><td>...</td><td>...</td><td>...</td></tr>" if(start!=0)
    filteredarray.each { |l|
        inner = "<td><font color=blue>%08X  </font></td><td><font color=#FF6600>%s</font></td><td>%s</td>" % l
        if(l[3]) # The good one
            res+= "<tr bgcolor=#FFFF44>%s</tr>" % inner
        else
            res+= "<tr>%s</tr>" % inner
        end
    }
    res+= "<tr><td>...</td><td>...</td><td>...</td></tr>" if(stop!=linesarray.size()-1)
    res += "</table>"
    puts res
end

class Integer
    addHelpEntry("disassembly", "Integer.functionAt", "", "", "Returns the function name of the code existing at Integer.")
    def functionAt()
        raise NoDisassemblyLoaded, "No disassembly loaded." if(!$CWDISLIST)
        return $CWDISLIST.functionAt(self)
    end
    
    addHelpEntry("disassembly", "Integer.dis", "scope=8", "", "Prints the disassembly code around the Integer value.")
    def dis(scope=8)
        raise NoDisassemblyLoaded, "No disassembly loaded." if(!$CWDISLIST)
        return cwBeautifulDisassembly(self,scope)
    end
    
end

class String
    addHelpEntry("disassembly", "String.address", "", "", "Returns the address of the symbol represented by the String.")
    def address()
        raise NoDisassemblyLoaded, "No disassembly loaded." if(!$CWDISLIST)
        return $CWDISLIST.functionAddress(self)    
    end
end
