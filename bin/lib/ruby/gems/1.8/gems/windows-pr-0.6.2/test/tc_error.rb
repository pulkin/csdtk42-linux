#####################################################################
# tc_error.rb
#
# Test case for the Windows::Error module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require "windows/error"
require "test/unit"

class Foo
   include Windows::Error
end

class TC_Windows_Error < Test::Unit::TestCase
  
   def setup
      @foo = Foo.new
   end
   
   def test_numeric_constants
      assert_equal(0x00000100, Foo::FORMAT_MESSAGE_ALLOCATE_BUFFER)
      assert_equal(0x00000200, Foo::FORMAT_MESSAGE_IGNORE_INSERTS)
      assert_equal(0x00000400, Foo::FORMAT_MESSAGE_FROM_STRING)
      assert_equal(0x00000800, Foo::FORMAT_MESSAGE_FROM_HMODULE)
      assert_equal(0x00001000, Foo::FORMAT_MESSAGE_FROM_SYSTEM)
      assert_equal(0x00002000, Foo::FORMAT_MESSAGE_ARGUMENT_ARRAY) 
      assert_equal(0x000000FF, Foo::FORMAT_MESSAGE_MAX_WIDTH_MASK) 
      assert_equal(0x0001, Foo::SEM_FAILCRITICALERRORS)
      assert_equal(0x0004, Foo::SEM_NOALIGNMENTFAULTEXCEPT)
      assert_equal(0x0002, Foo::SEM_NOGPFAULTERRORBOX)
      assert_equal(0x8000, Foo::SEM_NOOPENFILEERRORBOX)
   end
   
   def test_method_constants
      assert_not_nil(Foo::GetLastError)
      assert_not_nil(Foo::SetLastError)
      assert_not_nil(Foo::SetLastErrorEx) # Ignore for VC++ 6 or earlier.
      assert_not_nil(Foo::SetErrorMode)
      assert_not_nil(Foo::FormatMessage)
   end
   
   def test_get_last_error
      assert_respond_to(@foo, :get_last_error)
      assert_nothing_raised{ @foo.get_last_error }
      assert_kind_of(String, @foo.get_last_error)
   end
   
   def teardown
      @foo = nil
   end
end