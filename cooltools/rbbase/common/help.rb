#!/usr/bin/env ruby
include CoolHost

# Ruby config
require 'rbconfig'
include Config

begin
   include CoolWatcher 
rescue
end

#################################
#		HELP		#
#################################

module CoolHelp

	private
	
	class HelpSection
		def initialize()
			@entries = Hash.new
			@description = ""
		end
		
		def description
			@description
		end
		
		def description=(des)
			@description = des
		end
	
		def entries
			@entries
		end
	end
	
	class HelpEntry
	  attr_reader :section,:funcname,:help,:arguments,:returns
	  def initialize(str_section,str_funcname,str_args,str_returns,str_help)
		@section = str_section
		@funcname = str_funcname
	    @help = str_help
	    @arguments = str_args
	    @returns = str_returns
	  end
	end

	$HELP =  Hash.new()

	def addHelpSection(str_section)
		$HELP[str_section] = HelpSection.new()
	end


	def helpFuncStr(name,args,returns,help)
		return "<tr bgcolor=#d0d0d0 id='"+name+"'><td><font color=red>%s</font> <b>%s</b>&nbsp;&nbsp;&nbsp;(<font color=blue>%s</font>)</td></tr><tr bgcolor=#f0f0f0><td><font color=darkgreen>&nbsp;&nbsp;&nbsp;%s</font></td></tr>" % [returns,name,args,help]
	end

	def helpUsage()
		puts 
		str = "html>"
		str += "Please type <font color=darkgreen>help \"function\"</font>," \
			"or <font color=darkgreen>help \"section\"</font> " \
			"with one of the following section names to get the list of functions it provides :"
		
		dark = true
		str += "<center><table>"
		$HELP.sort.each {|x| 
		    
		    str+= "<tr bgcolor=%s><td><font color=red>%s</font></td></tr>" % [ (dark)?("#d0d0d0"):("#f0f0f0") , x[0]]
		    dark = !dark
		}
		str += "</table></center>"
		puts str	
	end


	public
	def setHelpSectionDesription(str_section,str_description)
		if(!$HELP[str_section]) then
			addHelpSection(str_section)
		end
		$HELP[str_section].description = str_description
	end

	def addHelpEntry(str_section,str_funcname,str_args,str_returns,str_help,bool_cwcompletion=true)
		if(!$HELP[str_section]) then
			addHelpSection(str_section)
		end
		$HELP[str_section].entries[str_funcname] = HelpEntry.new(str_section,str_funcname,str_args,str_returns,str_help)
		
		begin
		    cwAddCompletionFunction(str_funcname,str_args,str_returns,str_help) if(bool_cwcompletion)
		rescue
	    end
	end

	def help(arg="")
		if(arg=="") then
			helpUsage()
		elsif(!$HELP[arg]) #Section does not exist, tries to find matching function.
			$HELP.each_value { |section| 
				if(section.entries[arg]) then
					puts "html><table>"+helpFuncStr(section.entries[arg].funcname,section.entries[arg].arguments,section.entries[arg].returns,section.entries[arg].help)+"</table>"
					return
				end
			}
			puts "Section or function does not exist."
		else #Section exists, display its content
			str = "html>"
			str += $HELP[arg].description
			str += "<table>"
			$HELP[arg].entries.sort.each {|x| str+= helpFuncStr(x[1].funcname,x[1].arguments,x[1].returns,x[1].help) }
			str+= "</table>"
			puts str	
		end
	end
	
	def helpSaveSectionString(sectionName)
	    str = ""
		str += "<a name='%s'><h1>%s</h1></a>\n" % [sectionName,sectionName]
		str += $HELP[sectionName].description
		
		funcs = $HELP[sectionName].entries.sort
		funcs.each{ |x| str+= "<li><a href='#%s'>%s</a></li>\n" % [x[1].funcname,x[1].funcname]	}
		
		str += "<br>"    
		    
		str += "<table>\n"
		funcs.each{ |x| str+= helpFuncStr(x[1].funcname,x[1].arguments,x[1].returns,x[1].help)+"\n" }
		str += "</table>\n"
    end
	
	def helpSaveToFile(filename,sectionname=nil,withheader=false)
		str = ""
		
		if(withheader)
		    str+= "<h1>Coolruby Available Functions</h1>"
		    str+= "This document has been automatically generated. It explains the available ruby functions in your coolruby environment: their usage, their utility. You can click on one of the categories below to get specific help on one topic. If you are in CoolWatcher, you can type \"help\" in the command line to get on-the-fly help; you can also press the tab key to get a completion list of the available functions and variables."
	    end
		
		if(!sectionname)
		    str+="<h1>Categories</h1>"
		    #Index of sections
		    $HELP.sort.each { |section|	
		        str+= "<li><a href='#%s'>%s</a></li>\n" % [section[0],section[0]]	
		    }
    		
    		#Section content
    		$HELP.sort.each { |section|
    			 str += "<hr>\n"
    			 str += helpSaveSectionString(section[0])
    		}
	    elsif($HELP[sectionname])
	        str += helpSaveSectionString(sectionname)
	    else
	        puts "No section found!"
	    end
	    	    
		f = File.open(filename,"wb")
		f << str
		f.close()
	end
	
	def openUrl(url)
        begin
            require 'win32ole'
            shell = WIN32OLE.new('Shell.Application')
            begin
                shell.ShellExecute(url, '', '', 'open', 3)
            rescue Exception
                puts "Failed opening URL: %s." %url
            end
        rescue Exception
            if (CONFIG["target_vendor"] == "apple")
                # Under Mac OS.
                `open #{url}`
            else
                # Under Linux.
                Thread.new {
                    `firefox #{url}`
                }
            end
        end	    
    end
	
	def showHelp()
	    url = "./DOC/coolwatcher.html"
	    begin
	        # Only if DOC/ does not exist.
	        Dir.mkdir("./DOC/")
	    rescue
	    end
	    puts "Generating help..."
	    helpSaveToFile(url,nil,true)
	    puts "Done."
	    openUrl(url)
    end
    
    def showUserGuide()
       	begin
	        require 'win32ole'
            shell = WIN32OLE.new('Shell.Application')
            manual = "./DOC/Coolwatcher User Guide.pdf"
            begin
                shell.ShellExecute(manual, '', '', 'open', 3)
            rescue Exception
                puts "Failed opening User Manual."
            end
        rescue Exception
            Thread.new {
                `acroread #{manual}`
            }
        end
    end
    
    def showUserManual()
        openUrl("DOC/Manual/index.html")
    end
    
end

include CoolHelp
