#############################################################################
# tc_file_encryption.rb
#
# Test case for the encryption related methods of win32-file.
#############################################################################
base = File.basename(Dir.pwd)

if base == 'test' || base =~ /win32-file/
	Dir.chdir('..') if base == 'test'
	$LOAD_PATH.unshift(Dir.pwd + '/lib')
	Dir.chdir('test') rescue nil
end

require "test/unit"
require "win32/file"

class TC_Win32_File_Encryption < Test::Unit::TestCase
   def setup
      @file  = 'sometestfile.txt'
      @msg   = 'IGNORE - may not work due to security setup of your system'
   end
   
   def test_encrypt
      assert_respond_to(File, :encrypt)
      assert_nothing_raised(@msg){ File.encrypt(@file) }
   end
   
   def test_decrypt
      assert_respond_to(File, :decrypt)
      assert_nothing_raised(@msg){ File.decrypt(@file) }
   end
   
   def teardown
      @file = nil
   end
end