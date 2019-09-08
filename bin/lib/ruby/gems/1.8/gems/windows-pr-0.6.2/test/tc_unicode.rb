#####################################################################
# tc_unicode.rb
#
# Test case for the Windows::Unicode module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require "windows/unicode"
require "test/unit"

class Foo
   include Windows::Unicode
end

class TC_Windows_Unicode < Test::Unit::TestCase
   def setup
      @foo = Foo.new
   end

   def test_numeric_constants
      assert_equal(0, Foo::CP_ACP)
      assert_equal(1, Foo::CP_OEMCP)
      assert_equal(2, Foo::CP_MACCP)
      assert_equal(3, Foo::CP_THREAD_ACP)
      assert_equal(42, Foo::CP_SYMBOL)
      assert_equal(65000, Foo::CP_UTF7)
      assert_equal(65001, Foo::CP_UTF8)

      assert_equal(0x00000001, Foo::MB_PRECOMPOSED)
      assert_equal(0x00000002, Foo::MB_COMPOSITE)
      assert_equal(0x00000004, Foo::MB_USEGLYPHCHARS)
      assert_equal(0x00000008, Foo::MB_ERR_INVALID_CHARS)

      assert_equal(0x00000200, Foo::WC_COMPOSITECHECK)
      assert_equal(0x00000010, Foo::WC_DISCARDNS)
      assert_equal(0x00000020, Foo::WC_SEPCHARS)
      assert_equal(0x00000040, Foo::WC_DEFAULTCHAR)
      assert_equal(0x00000400, Foo::WC_NO_BEST_FIT_CHARS)
   end

   def test_method_constants
      assert_not_nil(Foo::GetTextCharset)
      assert_not_nil(Foo::GetTextCharsetInfo)
      assert_not_nil(Foo::IsDBCSLeadByte)
      assert_not_nil(Foo::IsDBCSLeadByteEx)
      assert_not_nil(Foo::IsTextUnicode)
      assert_not_nil(Foo::MultiByteToWideChar)
      assert_not_nil(Foo::TranslateCharsetInfo)
      assert_not_nil(Foo::WideCharToMultiByte)
   end

   def teardown
      @foo  = nil
      @unicode = nil
   end
end
