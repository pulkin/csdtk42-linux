require "base.rb"
require "tutorial1/tutorial1.rb"

# Register the plugin in CoolWatcher
cwRegisterPlugin(File.dirname(__FILE__)+"/tutorial1.dll")

module Tutorial1ModuleGUI
    # Do some stuff in CoolWatcher's GUI if you want to (like adding buttons, menus, etc)
end
include Tutorial1ModuleGUI