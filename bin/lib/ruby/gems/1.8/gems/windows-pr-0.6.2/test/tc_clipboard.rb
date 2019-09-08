#####################################################################
# tc_clipboard.rb
#
# Test case for the Windows::Clipboard module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require "windows/clipboard"
require "test/unit"

class Foo
   include Windows::Clipboard
end

class TC_Windows_Clipboard < Test::Unit::TestCase
   def setup
      @foo = Foo.new
   end
   
   def test_numeric_constants
      assert_equal(1, Foo::CF_TEXT)
      assert_equal(2, Foo::CF_BITMAP)
      assert_equal(3, Foo::CF_METAFILEPICT)
      assert_equal(4, Foo::CF_SYLK)
      assert_equal(5, Foo::CF_DIF)
      assert_equal(6, Foo::CF_TIFF)
      assert_equal(7, Foo::CF_OEMTEXT)
      assert_equal(8, Foo::CF_DIB)
      assert_equal(9, Foo::CF_PALETTE)
      assert_equal(10, Foo::CF_PENDATA)
      assert_equal(11, Foo::CF_RIFF)
      assert_equal(12, Foo::CF_WAVE)
      assert_equal(13, Foo::CF_UNICODETEXT)
      assert_equal(14, Foo::CF_ENHMETAFILE)
   end
   
   def test_method_constants
      assert_not_nil(Foo::OpenClipboard)
      assert_not_nil(Foo::CloseClipboard)
      assert_not_nil(Foo::GetClipboardData)
      assert_not_nil(Foo::EmptyClipboard)
      assert_not_nil(Foo::SetClipboardData)
      assert_not_nil(Foo::CountClipboardFormats)
      assert_not_nil(Foo::IsClipboardFormatAvailable)
      assert_not_nil(Foo::GetClipboardFormatName)
      assert_not_nil(Foo::EnumClipboardFormats)
      assert_not_nil(Foo::RegisterClipboardFormat)
   end
   
   def teardown
      @foo = nil
   end
end
