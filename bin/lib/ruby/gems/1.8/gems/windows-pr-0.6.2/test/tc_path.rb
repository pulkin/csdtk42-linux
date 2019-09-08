#####################################################################
# tc_path.rb
#
# Test case for the Windows::Path module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require "windows/path"
require "test/unit"

class Foo
   include Windows::Path
end

class TC_Windows_Path < Test::Unit::TestCase
   def setup
      @foo  = Foo.new
      @path = "C:\\"
   end

   def test_numeric_constants
      assert_equal(0x0000, Foo::GCT_INVALID)
      assert_equal(0x0001, Foo::GCT_LFNCHAR)
      assert_equal(0x0002, Foo::GCT_SHORTCHAR)
      assert_equal(0x0004, Foo::GCT_WILD)
      assert_equal(0x0008, Foo::GCT_SEPARATOR)
   end

   def test_method_constants
      assert_not_nil(Foo::PathAddBackslash)
      assert_not_nil(Foo::PathAddExtension)
      assert_not_nil(Foo::PathAppend)
      assert_not_nil(Foo::PathBuildRoot)
      assert_not_nil(Foo::PathCanonicalize)
      assert_not_nil(Foo::PathCombine)
      assert_not_nil(Foo::PathCommonPrefix)
      assert_not_nil(Foo::PathCompactPath)
      assert_not_nil(Foo::PathCompactPathEx)
      assert_not_nil(Foo::PathCreateFromUrl)
      assert_not_nil(Foo::PathFileExists)
      assert_not_nil(Foo::PathFindExtension)
      assert_not_nil(Foo::PathFindFileName)
      assert_not_nil(Foo::PathFindNextComponent)
      assert_not_nil(Foo::PathFindOnPath)
      assert_not_nil(Foo::PathFindSuffixArray)
      assert_not_nil(Foo::PathGetArgs)
      assert_not_nil(Foo::PathGetCharType)
      assert_not_nil(Foo::PathGetDriveNumber)
      assert_not_nil(Foo::PathIsContentType)
      assert_not_nil(Foo::PathIsDirectory)
      assert_not_nil(Foo::PathIsDirectoryEmpty)
      assert_not_nil(Foo::PathIsFileSpec)
      #assert_not_nil(Foo::PathIsHTMLFile)
      assert_not_nil(Foo::PathIsLFNFileSpec)
      assert_not_nil(Foo::PathIsNetworkPath)
      assert_not_nil(Foo::PathIsPrefix)
      assert_not_nil(Foo::PathIsRelative)
      assert_not_nil(Foo::PathIsRoot)
      assert_not_nil(Foo::PathIsSameRoot)
      assert_not_nil(Foo::PathIsSystemFolder)
      assert_not_nil(Foo::PathIsUNC)
      assert_not_nil(Foo::PathIsUNCServer)
      assert_not_nil(Foo::PathIsUNCServerShare)
      assert_not_nil(Foo::PathIsURL)
      assert_not_nil(Foo::PathMakePretty)
      assert_not_nil(Foo::PathMakeSystemFolder)
      assert_not_nil(Foo::PathMatchSpec)
      assert_not_nil(Foo::PathParseIconLocation)
      assert_not_nil(Foo::PathQuoteSpaces)
      assert_not_nil(Foo::PathRelativePathTo)
      assert_not_nil(Foo::PathRemoveArgs)
      assert_not_nil(Foo::PathRemoveBackslash)
      assert_not_nil(Foo::PathRemoveBlanks)
      assert_not_nil(Foo::PathRemoveExtension)
      assert_not_nil(Foo::PathRemoveFileSpec)
      assert_not_nil(Foo::PathRenameExtension)
      assert_not_nil(Foo::PathSearchAndQualify)
      assert_not_nil(Foo::PathSetDlgItemPath)
      assert_not_nil(Foo::PathSkipRoot)
      assert_not_nil(Foo::PathStripPath)
      assert_not_nil(Foo::PathStripToRoot)
      assert_not_nil(Foo::PathUndecorate)
      assert_not_nil(Foo::PathUnExpandEnvStrings)
      assert_not_nil(Foo::PathUnmakeSystemFolder)
      assert_not_nil(Foo::PathUnquoteSpaces)
   end

   def teardown
      @foo  = nil
      @path = nil
   end
end
