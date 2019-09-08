#####################################################################
# tc_file.rb
#
# Test case for the Windows::File module.
#####################################################################
base = File.basename(Dir.pwd)
if base == 'test' || base =~ /windows-pr/
   Dir.chdir '..' if base == 'test'
   $LOAD_PATH.unshift Dir.pwd + '/lib'
   Dir.chdir 'test' rescue nil
end

require 'windows/file'
require 'test/unit'

class Foo
   include Windows::File
end

class TC_Windows_File < Test::Unit::TestCase
  
   def setup
      @foo = Foo.new
   end
   
   def test_numeric_constants
      assert_equal(0x00000001, Foo::FILE_ATTRIBUTE_READONLY)  
      assert_equal(0x00000002, Foo::FILE_ATTRIBUTE_HIDDEN)  
      assert_equal(0x00000004, Foo::FILE_ATTRIBUTE_SYSTEM)
      assert_equal(0x00000010, Foo::FILE_ATTRIBUTE_DIRECTORY) 
      assert_equal(0x00000020, Foo::FILE_ATTRIBUTE_ARCHIVE)  
      assert_equal(0x00000040, Foo::FILE_ATTRIBUTE_ENCRYPTED)
      assert_equal(0x00000080, Foo::FILE_ATTRIBUTE_NORMAL)  
      assert_equal(0x00000100, Foo::FILE_ATTRIBUTE_TEMPORARY)  
      assert_equal(0x00000200, Foo::FILE_ATTRIBUTE_SPARSE_FILE) 
      assert_equal(0x00000400, Foo::FILE_ATTRIBUTE_REPARSE_POINT)  
      assert_equal(0x00000800, Foo::FILE_ATTRIBUTE_COMPRESSED)  
      assert_equal(0x00001000, Foo::FILE_ATTRIBUTE_OFFLINE) 
      assert_equal(0x00002000, Foo::FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
   end
   
   def test_method_constants
      assert_not_nil(Foo::CopyFile)
      assert_not_nil(Foo::CopyFileEx)
      assert_not_nil(Foo::CreateFile)
      assert_not_nil(Foo::CreateFileW)
      assert_not_nil(Foo::CreateHardLink)
      assert_not_nil(Foo::DecryptFile)
      assert_not_nil(Foo::DeleteFile)
      assert_not_nil(Foo::EncryptFile)
      assert_not_nil(Foo::GetBinaryType)
      assert_not_nil(Foo::GetFileAttributes)
      assert_not_nil(Foo::GetFileAttributesEx)
      assert_not_nil(Foo::GetFileSize)
      assert_not_nil(Foo::GetFileSizeEx)
      assert_not_nil(Foo::GetFileType)
      assert_not_nil(Foo::GetFullPathName)
      assert_not_nil(Foo::GetFullPathNameW)
      assert_not_nil(Foo::GetLongPathName)
      assert_not_nil(Foo::GetShortPathName)
      assert_not_nil(Foo::LockFile)
      assert_not_nil(Foo::LockFileEx)
      assert_not_nil(Foo::ReadFile)
      assert_not_nil(Foo::ReadFileEx)
      assert_not_nil(Foo::SetFileAttributes)
      assert_not_nil(Foo::UnlockFile)
      assert_not_nil(Foo::UnlockFileEx)
      assert_not_nil(Foo::WriteFile)
      assert_not_nil(Foo::WriteFileEx)
   end
   
   def teardown
      @foo = nil
   end
end