#####################################################################
# tc_eventlog.rb
#
# Test case for the Windows::EventLog module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/eventlog'
require 'test/unit'

class Foo
   include Windows::EventLog
end

class TC_Windows_EventLog < Test::Unit::TestCase
   def setup
      @foo = Foo.new
   end
   
   def test_numeric_constants
      assert_equal(1, Foo::EVENTLOG_SEQUENTIAL_READ)
      assert_equal(2, Foo::EVENTLOG_SEEK_READ)
      assert_equal(4, Foo::EVENTLOG_FORWARDS_READ)
      assert_equal(8, Foo::EVENTLOG_BACKWARDS_READ)
      assert_equal(0, Foo::EVENTLOG_SUCCESS)
      assert_equal(1, Foo::EVENTLOG_ERROR_TYPE)
      assert_equal(2, Foo::EVENTLOG_WARNING_TYPE)
      assert_equal(4, Foo::EVENTLOG_INFORMATION_TYPE)
      assert_equal(8, Foo::EVENTLOG_AUDIT_SUCCESS)
      assert_equal(16, Foo::EVENTLOG_AUDIT_FAILURE)
      assert_equal(0, Foo::EVENTLOG_FULL_INFO)
   end
   
   def test_method_constants
      assert_not_nil(Foo::BackupEventLog)
      assert_not_nil(Foo::BackupEventLogW)
      assert_not_nil(Foo::ClearEventLog)
      assert_not_nil(Foo::ClearEventLogW)
      assert_not_nil(Foo::CloseEventLog)
      assert_not_nil(Foo::DeregisterEventSource)
      assert_not_nil(Foo::GetEventLogInformation)
      assert_not_nil(Foo::GetNumberOfEventLogRecords)
      assert_not_nil(Foo::GetOldestEventLogRecord)
      assert_not_nil(Foo::NotifyChangeEventLog)
      assert_not_nil(Foo::OpenBackupEventLog)
      assert_not_nil(Foo::OpenBackupEventLogW)
      assert_not_nil(Foo::OpenEventLog)
      assert_not_nil(Foo::OpenEventLogW)
      assert_not_nil(Foo::ReadEventLog)
      assert_not_nil(Foo::ReadEventLogW)
      assert_not_nil(Foo::RegisterEventSource)
      assert_not_nil(Foo::RegisterEventSourceW)
      assert_not_nil(Foo::ReportEvent)
      assert_not_nil(Foo::ReportEventW)
   end
   
   def teardown
      @foo = nil
   end
end
