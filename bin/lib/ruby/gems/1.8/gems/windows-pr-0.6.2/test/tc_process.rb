#####################################################################
# tc_process.rb
#
# Test case for the Windows::Process module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/process'
require 'test/unit'

class Foo
   include Windows::Process
end

class TC_Windows_Process < Test::Unit::TestCase
   def setup
      @foo  = Foo.new
   end

   def test_numeric_constants
      assert_equal(0x1F0FFF, Foo::PROCESS_ALL_ACCESS)
   end

   def test_method_constants
      assert_not_nil(Foo::CreateProcess)
   end

   def teardown
      @foo  = nil
   end
end
