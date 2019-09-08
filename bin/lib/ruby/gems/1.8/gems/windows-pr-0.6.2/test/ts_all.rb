$LOAD_PATH.unshift(Dir.pwd)
$LOAD_PATH.unshift(Dir.pwd + '/lib')
$LOAD_PATH.unshift(Dir.pwd + '/lib/windows')
$LOAD_PATH.unshift(Dir.pwd + '/lib/windows/msvcrt')
$LOAD_PATH.unshift(Dir.pwd + '/test')

Dir.chdir('test') rescue nil

require 'tc_console'
require 'tc_error'
require 'tc_file'
require 'tc_memory'
require 'tc_msvcrt_buffer'
require 'tc_path'
require 'tc_pipe'
require 'tc_registry'
require 'tc_security'
require 'tc_synchronize'
require 'tc_unicode'
