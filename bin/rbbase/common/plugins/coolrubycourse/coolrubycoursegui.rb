require "base.rb"
require "coolrubycourse/coolrubycourse.rb"

# Register the plugin in CoolWatcher
cwRegisterPlugin(File.dirname(__FILE__)+"/coolrubycourse.dll")

module CoolRubyCourseModuleGUI
    # Do some stuff in CoolWatcher's GUI if you want to (like adding buttons, menus, etc)
end
include CoolRubyCourseModuleGUI