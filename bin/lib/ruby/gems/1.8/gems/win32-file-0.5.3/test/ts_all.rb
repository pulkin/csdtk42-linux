$LOAD_PATH.unshift(Dir.pwd)
$LOAD_PATH.unshift(Dir.pwd + '/lib')
$LOAD_PATH.unshift(Dir.pwd + '/lib/win32')
$LOAD_PATH.unshift(Dir.pwd + '/test')

require 'tc_file_encryption'
require 'tc_file_path'
require 'tc_file_stat'
require 'tc_file_attributes'
require 'tc_file_security'
require 'tc_file_constants'