Dir.chdir('..') if File.basename(Dir.pwd) == 'test'
$LOAD_PATH.unshift Dir.pwd
$LOAD_PATH.unshift Dir.pwd + '/lib'
Dir.chdir('test') rescue nil

require 'tc_eventlog'
require 'tc_mc'