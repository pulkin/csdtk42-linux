#####################################################################
# tc_library.rb
#
# Test case for the Windows::Library module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/library'
require 'test/unit'

class Foo
   include Windows::Library
end

class TC_Windows_Library < Test::Unit::TestCase
   def setup
      @foo  = Foo.new
   end

   def test_numeric_constants
      assert_equal(0, Foo::DLL_PROCESS_DETACH)
      assert_equal(1, Foo::DLL_PROCESS_ATTACH)
      assert_equal(2, Foo::DLL_THREAD_ATTACH)
      assert_equal(3, Foo::DLL_THREAD_DETACH)
   end

   def test_method_constants
      assert_not_nil(Foo::FreeLibrary)
      assert_not_nil(Foo::GetModuleFileName)
      assert_not_nil(Foo::GetModuleHandle)
      assert_not_nil(Foo::LoadLibrary)
      assert_not_nil(Foo::LoadLibraryEx)
      assert_not_nil(Foo::LoadModule)
   end

   def teardown
      @foo  = nil
   end
end
