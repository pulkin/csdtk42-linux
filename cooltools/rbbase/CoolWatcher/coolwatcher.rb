#!/usr/bin/env ruby
include CoolWatcher

#Let's define $COOLWATCHER.
$COOLWATCHER = true

#For debugging init scripts, uncomment the following line :
#cwShowGuiElement(DebugArea)

include CoolHost
CR_RegisterXMLMemoryMap()

if(RUBY_PLATFORM =~ /linux/)
    $LOAD_PATH << "./lib/ruby/site_ruby/1.8"
    $LOAD_PATH << "./lib/ruby/site_ruby/1.8/x86_64-linux"
    $LOAD_PATH << "./lib/ruby/site_ruby"
    $LOAD_PATH << "./lib/ruby/vendor_ruby/1.8"
    $LOAD_PATH << "./lib/ruby/vendor_ruby/1.8/x86_64-linux"
    $LOAD_PATH << "./lib/ruby/vendor_ruby"
    $LOAD_PATH << "./lib/ruby/1.8/x86_64-linux"
end

# Decompress ruby library if needed under windows
if(ENV["OS"] && ENV["OS"].downcase =~ /windows/)
    if(!File.exists?("lib"))
        puts "UHOH!! You are under WIN32 and do not have the ruby library. Uncompressing it from libinstall, don't close the cmd window during this operation!"
        begin
            Dir.chdir("libinstall")
            `bsdtar.exe -xf \"rubystdlib.tar.gz\" -C \"..\"`
            Dir.chdir("..")
        rescue Exception
            puts "Process failed!!! CoolWatcher will not work properly."
        end
    end
end

require "base.rb"
require "help.rb"
require "yaml.rb"

# Require additional modules
require "CoolWatcher/cwwatch.rb"

# INTERNAL FUNCTION NEITHER TO BE USED EXTERNALLY NOR REMOVED!!
def cwWatchExecute(watchCommand,resBuffer,resBufferSize,wantResult,timtimeout=10.0)
    begin
        Timeout::timeout(timtimeout) {   
              res = eval(watchCommand).to_s + "\000"
              cwSetWatchResult(resBuffer,resBufferSize,res.to_s) if(wantResult && resBuffer.to_i!=0)
              return true
        }
    rescue Exception => e
        res = e.class.to_s + "\000"
        cwSetWatchResult(resBuffer,resBufferSize,res.to_s) if(resBuffer.to_i!=0)
        return false
    end
end

def cwDisableAutoPoll()
    cwDisableAutoPollRequest()
    enforce{
        Timeout::timeout(5){
            while(cwIsAutoPolling())
                sleep(0.001)
            end
            return true
        }
    }
    return false
end

class CWWatchCounter
protected
    @@counter = 0
public
    def self.oneMore()
        @@counter += 1
        return "$CWWATCH#{@@counter-1}"
    end
end

def cwAddWatch(element)
    vartowatch = element
    if( (element.path =~ /\$RubyInstanciated/) == 0)
        # Redefine the element as a global variable to prevent the GC from removing the element
        newvarname = CWWatchCounter.oneMore()
        eval( "#{newvarname}=element" )
        cwAddWatchInternal(element,newvarname) 
    else
        cwAddWatchInternal(element,nil) 
    end
end


def cwSaveWindowState(file="cwstate.cws")
    File.open(file,"wb") { |f|
        a = cwGetWindowState()
        f << a.pack("c*")
    }
end

def cwRestoreWindowState(file="cwstate.cws")
    File.open(file,"rb") { |f|
        cwSetWindowState(f.read().unpack("c*"))
    }
end

def cwSaveHistory(file="cwhistory.cwh")
    File.open(file,"wb") { |f|
        a = cwGetCommandHistory()
        a = a[0..99] #limit to 100 entries
        f << a.to_yaml()
    }    
end

def cwRestoreHistory(file="cwhistory.cwh")
    File.open(file,"rb") { |f|
        his = YAML::load(f)
        his.reverse_each{ |c|
            cwAddCommandHistoryEntry(c)
        }
    }
end


def cwAboutToQuit()
    begin
        cwSaveWindowState("profiles/"+cwGetProfileEntry("name","unknown")+".cws")
    rescue Exception
    end
    begin
        cwSaveHistory("profiles/"+cwGetProfileEntry("name","unknown")+".cwh")
    rescue Exception
    end    
    killAllCoolhosts()
end

def cwgets()
    begin
        cwSetCommandLineColor(255,255,140)
        a = gets    
        return a
    ensure
        cwSetCommandLineColor(255,255,255)
    end
end




addHelpEntry("CoolWatcher","cwAddWatch","chip_or_soft_variable","","Adds a watch on chip_or_soft_variable")
addHelpEntry("CoolWatcher","cwAddRubyWatch","str_ruby_expression, str_label","","Adds a watch on the ruby_expression (with label 'str_label')")
addHelpEntry("CoolWatcher","cwDumpWatches","","str","Returns a string representation of the current watches with their values.");
addHelpEntry("CoolWatcher","cwRefreshWatches","","","Toggles the refresh of the watches.")
addHelpEntry("CoolWatcher","cwClearWatches","","","Clears CoolWatcher watch table")

addHelpEntry("CoolWatcher","cwClear","","","Clears CoolWatcher script output")
addHelpEntry("CoolWatcher","cwAddScriptButton","toolbarname_string, code_string, pixmap_string_array, tooltip_string","","Adds a button to the toolbar 'toolbarname_string' (which is created if not exists). When the button is pressed, 'code_string' is executed by the Ruby interpreter. The default pixmap can be replaced by giving an unempty 'pixmap_string_array' which must be in the XPM format (see XPM format documentation")
addHelpEntry("CoolWatcher","cwAddToggleButton","toolbarname_string, button_name_string, toggled_at_start_bool, show_button_name_bool, pixmap_string_array, tooltip_string","","Adds a toggle button to the toolbar 'toolbarname_string' (which is created if not exists). The button is created in 'on' state if 'toggled_at_start_bool' is true. The button name is shown beside the button if 'show_button_name_bool' is true. The default pixmap can be replaced by giving an unempty 'pixmap_string_array' which must be in the XPM format (see XPM format documentation")
addHelpEntry("CoolWatcher","cwSetToolbarOrientation","toolbarname_string, orientation","","Change the orientation of the toolbar 'toolbarname_string'. Orientation can be one of the following constants : DockLeft, DockRight, DockTop, or DockBottom")
addHelpEntry("CoolWatcher","cwIsButtonToggled","button_name_string","bool","Returns true if the button is on. Otherwise, returns false.")
addHelpEntry("CoolWatcher","cwShowGuiElement","gui_element","","Shows the specified GUI Widget of CoolWatcher. gui_element can be one of the following : 	WatchArea, ModuleLibrary, TypedefLibrary, CommandLine, DebugArea, DescriptionArea, OutputArea, or DrawArea")
addHelpEntry("CoolWatcher","cwHideGuiElement","gui_element","","Hides the specified GUI Widget of CoolWatcher. gui_element can be one of the following : 	WatchArea, ModuleLibrary, TypedefLibrary, CommandLine, DebugArea, DescriptionArea, OutputArea, or DrawArea")
addHelpEntry("CoolWatcher","cwAddMenuCommand","menu_name_string, item_name_string, code_string, accelerator","","Creates a menu entry in the menus of CoolWatcher. If menu_name_string does not correspond to any menu, it is created. The entry has item_name_string as name. When activated, the menu entry executes code_string. An accelerator can be attached to this entry. See the Qt documentation for a list of keyboard keys that can be used to define accelerators.")
addHelpEntry("CoolWatcher","cwAddMenuSeparator","menu_name_string","","Adds a separator in the menu called 'menu_name_string'.")
addHelpEntry("CoolWatcher","cwSetProgress","num_steps, num_total, format=\"%p%\"","","Sets progress bar to num_steps over num_total. Shows it if num_steps is in the range 0 to num_total, else, hide it.")
addHelpEntry("CoolWatcher","cwSetTitle","string_title","","Sets the caption of the title bar CoolWatcher's main window.")

addHelpEntry("CoolWatcher","cwGetSaveFileName","display_string, path_string, filter_string","str","Opens a dialog box asking for a file to save, with 'display_string' as dialog title, path_string as default path where to save, and filter_string as file format filter (e.g. '(*.bmp;*.jpg)')")
addHelpEntry("CoolWatcher","cwGetOpenFileName","display_string, path_string, filter_string","str","Opens a dialog box asking for a file to load, with 'display_string' as dialog title, path_string as default path from where to load, and filter_string as file format filter (e.g. '(*.bmp;*.jpg)')")

addHelpEntry("CoolWatcher","cwQuit","","","Quits CoolWatcher.")
addHelpEntry("CoolWatcher","cwSetProfileEntry","profile_entry_string, value","","Adds an entry with 'profile_entry_string' as name and 'value' as value in the current loaded profile. Useful if various profiles use generic script which must load custom parameters.")
addHelpEntry("CoolWatcher","cwGetProfileEntry","profile_entry_string, default_value","str","Gets the entry value of the profile entry 'profile_entry_string'. Returns default_value if the profile_entry_string does not exist in the current profile.")

addHelpEntry("CoolWatcher","cwAddCommandHistoryEntry","command_string","command_string","Adds the command 'command_string' to the command history.")
addHelpEntry("CoolWatcher","cwGetCommandHistory","","string_array","Gets the table containing the complete command history.")

if(!$CWCREGWATCH)
    $CWCREGWATCH = registerNewToolConnection(ToolConnection.new("RegWatch Connection"))
end

if(!$CWCBUFWATCH)
    $CWCBUFWATCH = registerNewToolConnection(ToolConnection.new("BufWatch Connection"))
end


