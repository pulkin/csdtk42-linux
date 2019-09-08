require "base.rb"
require "usrdatatool/usrdatatool.rb"

# Register the plugin in CoolWatcher
cwRegisterPlugin(File.dirname(__FILE__)+"/usrdatatool.dll")

module UsrDataToolModule
    # Do some stuff in CoolWatcher's GUI if you want to (like adding buttons, menus, etc)
end
include UsrDataToolModule
