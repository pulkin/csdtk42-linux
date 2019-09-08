#####################################################################
# tc_memory.rb
#
# Test case for the Windows::Memory module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/memory'
require 'test/unit'

class Foo
   include Windows::Memory
end

class TC_Windows_Path < Test::Unit::TestCase
   def setup
      @foo  = Foo.new
      @path = "C:\\"
   end

   def test_numeric_constants
      assert_not_nil(Foo::GHND)
      assert_not_nil(Foo::GMEM_FIXED)
      assert_not_nil(Foo::GMEM_MOVABLE)
      assert_not_nil(Foo::GMEM_ZEROINIT)
      assert_not_nil(Foo::GPTR)
   end

   def test_method_constants
      assert_not_nil(Foo::GlobalAlloc)
      assert_not_nil(Foo::GlobalFlags)
      assert_not_nil(Foo::GlobalFree)
      assert_not_nil(Foo::GlobalHandle)
      assert_not_nil(Foo::GlobalLock)
      assert_not_nil(Foo::GlobalMemoryStatus)
      assert_not_nil(Foo::GlobalMemoryStatusEx)
      assert_not_nil(Foo::GlobalReAlloc)
      assert_not_nil(Foo::GlobalSize)
      assert_not_nil(Foo::GlobalUnlock)
   end

   def teardown
      @foo  = nil
      @path = nil
   end
end
