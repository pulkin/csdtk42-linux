#####################################################################
# tc_msvcrt_buffer.rb
#
# Test case for the Windows::MSVCRT::Buffer module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require "windows/msvcrt/buffer"
require "test/unit"

class Foo
   include Windows::MSVCRT::Buffer
end

class TC_Windows_MSVCRT_Buffer < Test::Unit::TestCase
   def setup
      @foo = Foo.new
   end
   
   def test_method_constants
      assert_not_nil(Foo::Memcpy)
      assert_not_nil(Foo::Memccpy)
      assert_not_nil(Foo::Memchr)
      assert_not_nil(Foo::Memcmp)
      assert_not_nil(Foo::Memicmp)
      assert_not_nil(Foo::Memmove)
      assert_not_nil(Foo::Memset)
      assert_not_nil(Foo::Swab)
   end
   
   def test_memcpy
      assert_respond_to(@foo, :memcpy)
   end
   
   def test_memccpy
      assert_respond_to(@foo, :memccpy)
   end
   
   def test_memchr
      assert_respond_to(@foo, :memchr)
   end
   
   def test_memcmp
      assert_respond_to(@foo, :memcmp)
   end
   
   def test_memicmp
      assert_respond_to(@foo, :memicmp)
   end
   
   def test_memmove
      assert_respond_to(@foo, :memmove)
   end
   
   def test_memset
      assert_respond_to(@foo, :memset)
   end
   
   def test_swab
      assert_respond_to(@foo, :swab)
   end
   
   def teardown
      @foo  = nil
   end
end