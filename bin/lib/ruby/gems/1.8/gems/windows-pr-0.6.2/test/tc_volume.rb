#####################################################################
# tc_volume.rb
#
# Test case for the Windows::Volume module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/volume'
require 'test/unit'

class Foo
   include Windows::Volume
end

class TC_Windows_Clipboard < Test::Unit::TestCase
   def setup
      @foo = Foo.new
   end
   
   def test_method_constants
      assert_not_nil(Foo::GetVolumeInformation)
   end
   
   def teardown
      @foo = nil
   end
end
