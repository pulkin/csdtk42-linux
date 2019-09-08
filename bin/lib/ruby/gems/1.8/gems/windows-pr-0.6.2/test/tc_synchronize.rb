##################################################################
# tc_synchronize.rb
#
# Test case for the windows/synchronize package.
##################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require "windows/synchronize"
require "test/unit"

class Foo
   include Windows::Synchronize
end

class TC_Windows_Synchronize < Test::Unit::TestCase
   def setup
      @handle = (0.chr * 16).unpack('LLLL')
      @foo    = Foo.new
   end
   
   def test_numeric_constants
      assert_not_nil(Foo::INFINITE)
      assert_not_nil(Foo::WAIT_OBJECT_0)
      assert_not_nil(Foo::WAIT_TIMEOUT)
      assert_not_nil(Foo::WAIT_ABANDONED)
      assert_not_nil(Foo::WAIT_FAILED)
      assert_not_nil(Foo::QS_ALLEVENTS)
      assert_not_nil(Foo::QS_ALLINPUT)
      assert_not_nil(Foo::QS_ALLPOSTMESSAGE)
      assert_not_nil(Foo::QS_HOTKEY)
      assert_not_nil(Foo::QS_INPUT)
      assert_not_nil(Foo::QS_KEY)
      assert_not_nil(Foo::QS_MOUSE)
      assert_not_nil(Foo::QS_MOUSEBUTTON)
      assert_not_nil(Foo::QS_MOUSEMOVE)
      assert_not_nil(Foo::QS_PAINT)
      assert_not_nil(Foo::QS_POSTMESSAGE)
      assert_not_nil(Foo::QS_RAWINPUT)
      assert_not_nil(Foo::QS_SENDMESSAGE)
      assert_not_nil(Foo::QS_TIMER)
      assert_not_nil(Foo::MWMO_ALERTABLE)
      assert_not_nil(Foo::MWMO_INPUTAVAILABLE)
      assert_not_nil(Foo::MWMO_WAITALL)
      assert_not_nil(Foo::EVENT_ALL_ACCESS)
      assert_not_nil(Foo::EVENT_MODIFY_STATE)
      assert_not_nil(Foo::MUTEX_ALL_ACCESS)
      assert_not_nil(Foo::MUTEX_MODIFY_STATE)
      assert_not_nil(Foo::SEMAPHORE_ALL_ACCESS)
      assert_not_nil(Foo::SEMAPHORE_MODIFY_STATE)
   end
   
   def test_method_constants
      assert_not_nil(Foo::CreateEvent)
      assert_not_nil(Foo::CreateMutex)
      assert_not_nil(Foo::CreateSemaphore)
      assert_not_nil(Foo::GetOverlappedResult)
      assert_not_nil(Foo::MsgWaitForMultipleObjects)
      assert_not_nil(Foo::MsgWaitForMultipleObjectsEx)
      assert_not_nil(Foo::OpenEvent)
      assert_not_nil(Foo::OpenMutex)
      assert_not_nil(Foo::OpenSemaphore)
      assert_not_nil(Foo::ReleaseMutex)
      assert_not_nil(Foo::ReleaseSemaphore)
      assert_not_nil(Foo::ResetEvent)
      assert_not_nil(Foo::SetEvent)
      assert_not_nil(Foo::WaitForMultipleObjects)
      assert_not_nil(Foo::WaitForMultipleObjectsEx)
      assert_not_nil(Foo::WaitForSingleObject)
      assert_not_nil(Foo::WaitForSingleObjectEx)
   end
   
   def teardown
      @foo = nil
      @handle = nil
   end
end