######################################################################
# tc_console.rb
#
# Test case for the Windows::Console module.
######################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/console'
require 'test/unit'

class Foo
   include Windows::Console
end

class TC_Windows_Console < Test::Unit::TestCase
   def setup
      @foo = Foo.new
   end

   def test_numeric_constants
      assert_equal(0, Foo::CTRL_C_EVENT)
      assert_equal(1, Foo::CTRL_BREAK_EVENT)
      assert_equal(5, Foo::CTRL_LOGOFF_EVENT)
      assert_equal(6, Foo::CTRL_SHUTDOWN_EVENT)

      assert_equal(0x0001, Foo::ENABLE_PROCESSED_INPUT)
      assert_equal(0x0002, Foo::ENABLE_LINE_INPUT)
      assert_equal(0x0002, Foo::ENABLE_WRAP_AT_EOL_OUTPUT)
      assert_equal(0x0004, Foo::ENABLE_ECHO_INPUT)
      assert_equal(0x0008, Foo::ENABLE_WINDOW_INPUT)
      assert_equal(0x0010, Foo::ENABLE_MOUSE_INPUT)
      assert_equal(0x0020, Foo::ENABLE_INSERT_MODE)
      assert_equal(0x0040, Foo::ENABLE_QUICK_EDIT_MODE)

      assert_equal(-10, Foo::STD_INPUT_HANDLE)
      assert_equal(-11, Foo::STD_OUTPUT_HANDLE)
      assert_equal(-12, Foo::STD_ERROR_HANDLE)
   end

   def test_method_constants
      assert_not_nil(Foo::AddConsoleAlias)
      assert_not_nil(Foo::AllocConsole)
      assert_not_nil(Foo::AttachConsole)
      assert_not_nil(Foo::CreateConsoleScreenBuffer)
      assert_not_nil(Foo::FillConsoleOutputAttribute)
      assert_not_nil(Foo::FillConsoleOutputCharacter)
      assert_not_nil(Foo::FlushConsoleInputBuffer)
      assert_not_nil(Foo::FreeConsole)
      assert_not_nil(Foo::GenerateConsoleCtrlEvent)
      assert_not_nil(Foo::GetConsoleAlias)
      assert_not_nil(Foo::GetConsoleAliases)
      assert_not_nil(Foo::GetConsoleAliasesLength)
      assert_not_nil(Foo::GetConsoleAliasExes)
      assert_not_nil(Foo::GetConsoleAliasExesLength)
      assert_not_nil(Foo::GetConsoleCP)
      assert_not_nil(Foo::GetConsoleCursorInfo)
      assert_not_nil(Foo::GetConsoleDisplayMode)
      assert_not_nil(Foo::GetConsoleFontSize)
      assert_not_nil(Foo::GetConsoleMode)
      assert_not_nil(Foo::GetConsoleOutputCP)
      assert_not_nil(Foo::GetConsoleProcessList)
      assert_not_nil(Foo::GetConsoleScreenBufferInfo)
      assert_not_nil(Foo::GetConsoleSelectionInfo)
      assert_not_nil(Foo::GetConsoleTitle)
      assert_not_nil(Foo::GetConsoleWindow)
      assert_not_nil(Foo::GetCurrentConsoleFont)
      assert_not_nil(Foo::GetLargestConsoleWindowSize)
      assert_not_nil(Foo::GetNumberOfConsoleInputEvents)
      assert_not_nil(Foo::GetNumberOfConsoleMouseButtons)
      assert_not_nil(Foo::GetStdHandle)
      assert_not_nil(Foo::PeekConsoleInput)
      assert_not_nil(Foo::ReadConsole)
      assert_not_nil(Foo::ReadConsoleInput)
      assert_not_nil(Foo::ReadConsoleOutput)
      assert_not_nil(Foo::ReadConsoleOutputAttribute)
      assert_not_nil(Foo::ReadConsoleOutputCharacter)
      assert_not_nil(Foo::ScrollConsoleScreenBuffer)
      assert_not_nil(Foo::SetConsoleActiveScreenBuffer)
      assert_not_nil(Foo::SetConsoleCommandHistoryMode)
      assert_not_nil(Foo::SetConsoleCP)
      assert_not_nil(Foo::SetConsoleCtrlHandler)
      assert_not_nil(Foo::SetConsoleCursorInfo)
      assert_not_nil(Foo::SetConsoleCursorPosition)
      assert_not_nil(Foo::SetConsoleDisplayMode)
      assert_not_nil(Foo::SetConsoleMode)
      assert_not_nil(Foo::SetConsoleOutputCP)
      assert_not_nil(Foo::SetConsoleScreenBufferSize)
      assert_not_nil(Foo::SetConsoleTextAttribute)
      assert_not_nil(Foo::SetConsoleTitle)
      assert_not_nil(Foo::SetConsoleWindowInfo)
      assert_not_nil(Foo::SetStdHandle)
      assert_not_nil(Foo::WriteConsole)
      assert_not_nil(Foo::WriteConsoleInput)
      assert_not_nil(Foo::WriteConsoleOutput)
      assert_not_nil(Foo::WriteConsoleOutputAttribute)
      assert_not_nil(Foo::WriteConsoleOutputCharacter)
   end

   def teardown
      @foo = nil
   end
end
