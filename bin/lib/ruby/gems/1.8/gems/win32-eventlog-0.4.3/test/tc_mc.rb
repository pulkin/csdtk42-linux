############################################################################
# tc_mc.rb
#
# Test suite for the win32-mc package.  The tests need to run in a specific
# order, hence the numerics added to the method names.
############################################################################
Dir.chdir('..') if File.basename(Dir.pwd) == 'test'
$LOAD_PATH.unshift Dir.pwd
$LOAD_PATH.unshift Dir.pwd + '/lib'
Dir.chdir('test') rescue nil

require 'test/unit'
require 'win32/mc'
include Win32

class TC_Win32_MC < Test::Unit::TestCase
   def setup
      @mc = MC.new('foo.mc')
   end
   
   def test_01_version
      assert_equal('0.1.2', MC::VERSION)
   end

   def test_02_create_header
      assert_respond_to(@mc, :create_header)
      assert_nothing_raised{ @mc.create_header }
   end

   def test_03_create_res_file
      assert_respond_to(@mc, :create_res_file)
      assert_nothing_raised{ @mc.create_res_file }
   end
   
   def test_04_create_dll_file
      assert_respond_to(@mc, :create_dll_file)
      assert_nothing_raised{ @mc.create_dll_file }
   end
   
   def test_05_clean
      assert_respond_to(@mc, :clean)
      assert_nothing_raised{ @mc.clean }
   end

   def teardown
      @mc = nil
      File.delete('foo.dll') rescue nil
   end
end