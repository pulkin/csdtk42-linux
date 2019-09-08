#####################################################################
# tc_pipe.rb
#
# Test case for the Windows::Pipe module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/pipe'
require 'test/unit'

class Foo
   include Windows::Pipe
end

class TC_Windows_Pipe < Test::Unit::TestCase
  
   def setup
      @foo = Foo.new
   end
   
   def test_numeric_constants
      assert_equal(0x00000001, Foo::NMPWAIT_NOWAIT)
      assert_equal(0xffffffff, Foo::NMPWAIT_WAIT_FOREVER)
      assert_equal(0x00000000, Foo::NMPWAIT_USE_DEFAULT_WAIT)
      assert_equal(0x00000000, Foo::PIPE_WAIT)
      assert_equal(0x00000001, Foo::PIPE_NOWAIT)
      assert_equal(0x00000001, Foo::PIPE_ACCESS_INBOUND)
      assert_equal(0x00000002, Foo::PIPE_ACCESS_OUTBOUND)
      assert_equal(0x00000003, Foo::PIPE_ACCESS_DUPLEX)
      assert_equal(0x00000000, Foo::PIPE_TYPE_BYTE)
      assert_equal(0x00000004, Foo::PIPE_TYPE_MESSAGE)
      assert_equal(0x00000000, Foo::PIPE_READMODE_BYTE)
      assert_equal(0x00000002, Foo::PIPE_READMODE_MESSAGE)
      assert_equal(0x00000000, Foo::PIPE_CLIENT_END)
      assert_equal(0x00000001, Foo::PIPE_SERVER_END)
   end
   
   def test_method_constants
      assert_not_nil(Foo::CallNamedPipe)
      assert_not_nil(Foo::ConnectNamedPipe)
      assert_not_nil(Foo::CreateNamedPipe)
      assert_not_nil(Foo::CreatePipe)
      assert_not_nil(Foo::DisconnectNamedPipe)
      assert_not_nil(Foo::GetNamedPipeHandleState)
      assert_not_nil(Foo::GetNamedPipeInfo)
      assert_not_nil(Foo::PeekNamedPipe)
      assert_not_nil(Foo::SetNamedPipeHandleState)
      assert_not_nil(Foo::TransactNamedPipe)
      assert_not_nil(Foo::WaitNamedPipe)
   end
   
   def teardown
      @foo = nil
   end
end
