#####################################################################
# tc_registry.rb
#
# Test case for the Windows::Registry module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/registry'
require 'test/unit'

class Foo
   include Windows::Registry
end

class TC_Windows_Registry < Test::Unit::TestCase
   def setup
      @foo  = Foo.new
   end

   def test_numeric_constants
      assert_equal(0x80000000, Foo::HKEY_CLASSES_ROOT)
   end

   def test_method_constants
      assert_not_nil(Foo::RegCloseKey)
   end

   def teardown
      @foo  = nil
   end
end
