require 'windows/msvcrt/buffer'
require 'windows/msvcrt/file'
require 'windows/filesystem'
require 'windows/device_io'
require 'windows/path'
require 'windows/file'
require 'windows/error'
require 'windows/handle'
require 'pp'

class File::Stat
   include Windows::MSVCRT::Buffer
   include Windows::MSVCRT::File
   include Windows::DeviceIO
   include Windows::FileSystem
   include Windows::Path
   include Windows::File
   include Windows::Error
   include Windows::Handle
   include Comparable
   
   VERSION = '1.2.3'

   # Defined in Ruby's win32.h.  Not meant for public consumption.
   S_IWGRP = 0020
   S_IWOTH = 0002
   
   attr_reader :dev_major, :dev_minor, :rdev_major, :rdev_minor
   
   # Creates and returns a File::Stat object, which encapsulate common status
   # information for File objects on MS Windows sytems. The information is
   # recorded at the moment the File::Stat object is created; changes made to
   # the file after that point will not be reflected.
   #
   def initialize(file)
      @file = file
      
      @blockdev  = false
      @file_type = get_file_type(file) # May update @blockdev
      
      @chardev = @file_type == FILE_TYPE_CHAR

      stat_buf = [0,0,0,0,0,0,0,0,0,0,0,0,0].pack('ISSssssIIQQQQ')
      
      # The stat64 function doesn't seem to like character devices
      if stat64(file, stat_buf) != 0
          raise ArgumentError, get_last_error unless @chardev
      end

      # Some bytes skipped (padding for struct alignment)
      @dev   = stat_buf[0, 4].unpack('I').first  # Drive number
      @ino   = stat_buf[4, 2].unpack('S').first  # Meaningless
      @mode  = stat_buf[6, 2].unpack('S').first  # File mode bit mask
      @nlink = stat_buf[8, 2].unpack('s').first  # Always 1
      @uid   = stat_buf[10, 2].unpack('s').first # Always 0
      @gid   = stat_buf[12, 2].unpack('s').first # Always 0
      @rdev  = stat_buf[16, 4].unpack('I').first # Same as dev
      @size  = stat_buf[24, 8].unpack('Q').first # Size of file in bytes
      @atime = Time.at(stat_buf[32, 8].unpack('Q').first) # Access time
      @mtime = Time.at(stat_buf[40, 8].unpack('Q').first) # Modification time
      @ctime = Time.at(stat_buf[48, 8].unpack('Q').first) # Creation time
      
      @mode = 33188 if @chardev

      attr = GetFileAttributes(file)
      
      if attr == INVALID_FILE_ATTRIBUTES
         raise ArgumentError, get_last_error
      end
      
      @blksize = get_blksize(file)
      
      # This is a reasonable guess
      case @blksize
         when nil
            @blocks = nil
         when 0
            @blocks = 0
         else
            @blocks  = (@size.to_f / @blksize.to_f).ceil
      end
      
      @readonly      = attr & FILE_ATTRIBUTE_READONLY > 0
      @hidden        = attr & FILE_ATTRIBUTE_HIDDEN > 0
      @system        = attr & FILE_ATTRIBUTE_SYSTEM > 0
      @archive       = attr & FILE_ATTRIBUTE_ARCHIVE > 0
      @directory     = attr & FILE_ATTRIBUTE_DIRECTORY > 0
      @encrypted     = attr & FILE_ATTRIBUTE_ENCRYPTED > 0
      @normal        = attr & FILE_ATTRIBUTE_NORMAL > 0
      @temporary     = attr & FILE_ATTRIBUTE_TEMPORARY > 0
      @sparse        = attr & FILE_ATTRIBUTE_SPARSE_FILE > 0
      @reparse_point = attr & FILE_ATTRIBUTE_REPARSE_POINT > 0
      @compressed    = attr & FILE_ATTRIBUTE_COMPRESSED > 0
      @offline       = attr & FILE_ATTRIBUTE_OFFLINE > 0
      @indexed       = attr & ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED > 0
      
      @executable = GetBinaryType(file, '')
      @regular    = @file_type == FILE_TYPE_DISK
      @pipe       = @file_type == FILE_TYPE_PIPE
      
      # Not supported and/or meaningless
      @dev_major     = nil
      @dev_minor     = nil
      @grpowned      = true
      @owned         = true
      @readable      = true
      @readable_real = true
      @rdev_major    = nil
      @rdev_minor    = nil
      @setgid        = false
      @setuid        = false
      @sticky        = false
      @symlink       = false
      @writable      = true
      @writable_real = true
   end
   
   ## Comparable
   
   # Compares two File::Stat objects.  Comparsion is based on mtime only.
   #
   def <=>(other)
      @mtime.to_i <=> other.mtime.to_i
   end
   
   ## Miscellaneous
   
   def blockdev?
      @blockdev
   end
   
   # Returns whether or not the file is a character device.
   #
   def chardev?
      @chardev
   end
   
   # Returns whether or not the file is executable.  Generally speaking, this
   # means .bat, .cmd, .com, and .exe files.
   #
   def executable?
      @executable
   end
   
   alias :executable_real? :executable?
   
   # Returns whether or not the file is a regular file, as opposed to a pipe,
   # socket, etc.
   #
   def file?
      @regular
   end
   
   # Identifies the type of file. The return string is one of: file,
   # directory, characterSpecial, socket or unknown.
   #
   def ftype
      return 'directory' if directory?
      case @file_type
         when FILE_TYPE_CHAR
            'characterSpecial'
         when FILE_TYPE_DISK
            'file'
         when FILE_TYPE_PIPE
            'socket'
         else
            if blockdev?
               'blockSpecial'
            else
               'unknown'
            end
      end
   end
   
   # Meaningless on Windows.
   #
   def grpowned?
      @grpowned
   end
   
   # Always true on Windows
   def owned?
      @owned
   end
   
   # Returns whether or not the file is a pipe.
   #
   def pipe?
      @pipe
   end
   
   alias :socket? :pipe?
   
   # Meaningless on Windows
   #
   def readable?
      @readable
   end
   
   # Meaningless on Windows
   #
   def readable_real?
      @readable_real
   end
   
   # Meaningless on Windows
   #
   def setgid?
      @setgid
   end
   
   # Meaningless on Windows
   #
   def setuid?
      @setuid
   end
   
   # Returns nil if statfile is a zero-length file; otherwise, returns the
   # file size. Usable as a condition in tests.
   #
   def size?
      @size > 0 ? @size : nil
   end
   
   # Meaningless on Windows.
   #
   def sticky?
      @sticky
   end
   
   # Meaningless on Windows at the moment.  This may change in the future.
   #
   def symlink?
      @symlink
   end
   
   # Meaningless on Windows.
   #
   def writable?
      @writable
   end
   
   # Meaningless on Windows.
   #
   def writable_real?
      @writable_real
   end
   
   # Returns whether or not the file size is zero.
   #
   def zero?
      @size == 0
   end
   
   ## Attribute members
   
   # Returns whether or not the file is an archive file.
   #
   def archive?
      @archive
   end

   # Returns whether or not the file is compressed.
   #
   def compressed?
      @compressed
   end
   
   # Returns whether or not the file is a directory.
   #
   def directory?
      @directory
   end
   
   # Returns whether or not the file in encrypted.
   #
   def encrypted?
      @encrypted
   end
   
   # Returns whether or not the file is hidden.
   #
   def hidden?
      @hidden
   end
   
   # Returns whether or not the file is content indexed.
   #
   def indexed?
      @indexed
   end
   alias :content_indexed? :indexed?
   
   # Returns whether or not the file is 'normal'.  This is only true if
   # virtually all other attributes are false.
   #
   def normal?
      @normal
   end
   
   # Returns whether or not the file is offline.
   #
   def offline?
      @offline
   end
   
   # Returns whether or not the file is readonly.
   #
   def readonly?
      @readonly
   end 

   alias :read_only? :readonly?
   
   # Returns whether or not the file is a reparse point.
   #
   def reparse_point?
      @reparse_point
   end
   
   # Returns whether or not the file is a sparse file.  In most cases a sparse
   # file is an image file.
   #
   def sparse?
      @sparse
   end
   
   # Returns whether or not the file is a system file.
   #
   def system?
      @system
   end
   
   # Returns whether or not the file is being used for temporary storage.
   #
   def temporary?
      @temporary
   end
   
   ## Standard stat members
   
   # Returns a Time object containing the last access time.
   #
   def atime
      @atime
   end
   
   # Returns the file system's block size, or nil if it cannot be determined.
   #
   def blksize
      @blksize
   end
   
   # Returns the number of blocks used by the file, where a block is defined
   # as size divided by blksize, rounded up.
   #
   # :no-doc:
   # This is a fudge.  A search of the internet reveals different ways people
   # have defined st_blocks on MS Windows.
   #
   def blocks
      @blocks
   end
   
   # Returns a Time object containing the time that the file status associated
   # with the file was changed.
   #
   def ctime
      @ctime
   end
   
   # Drive letter (A-Z) of the disk containing the file.  If the path is a
   # UNC path then the drive number (probably -1) is returned instead.
   #
   def dev
      if PathIsUNC(@file)
         @dev
      else
         (@dev + ?A).chr + ':'
      end
   end
   
   # Group ID. Always 0.
   #
   def gid
      @gid
   end
   
   # Inode number. Meaningless on NTFS.
   #
   def ino
      @ino
   end
   
   # Bit mask for file-mode information.
   #
   # :no-doc:
   # This was taken from rb_win32_stat() in win32.c.  I'm not entirely
   # sure what the point is.
   #
   def mode
      @mode &= ~(S_IWGRP | S_IWOTH)
   end
   
   # Returns a Time object containing the modification time.
   #
   def mtime
      @mtime
   end

   # Drive number of the disk containing the file.
   #
   def rdev
      @rdev
   end
   
   # Always 1
   #
   def nlink
      @nlink
   end
   
   # Returns the size of the file, in bytes.
   #
   def size
      @size
   end
   
   # User ID. Always 0.
   #
   def uid
      @uid
   end
   
   # Returns a stringified version of a File::Stat object.
   #
   def inspect    
      members = %w/
         archive? atime blksize blocks compressed? ctime dev encrypted? gid
         hidden? indexed? ino mode mtime rdev nlink normal? offline? readonly?
         reparse_point? size sparse? system? temporary? uid
      /
      str = "#<#{self.class}"
      members.sort.each{ |mem|
         if mem == 'mode'
            str << " #{mem}=" << sprintf("0%o", send(mem.intern))
         elsif mem[-1].chr == '?'
            str << " #{mem.chop}=" << send(mem.intern).to_s
         else
            str << " #{mem}=" << send(mem.intern).to_s
         end
      }
      str
   end
   
   # A custom pretty print method.  This was necessary not only to handle
   # the additional attributes, but to work around an error caused by the
   # builtin method for the current File::Stat class (see pp.rb).
   #
   def pretty_print(q)
      members = %w/
         archive? atime blksize blocks compressed? ctime dev encrypted? gid
         hidden? indexed? ino mode mtime rdev nlink normal? offline? readonly?
         reparse_point? size sparse? system? temporary? uid
      /

      q.object_group(self){
         q.breakable
         members.each{ |mem|
            q.group{
               q.text("#{mem}".ljust(15) + "=> ")
               if mem == 'mode'
                  q.text(sprintf("0%o", send(mem.intern)))
               else
                  val = self.send(mem.intern)
                  if val.nil?
                     q.text('nil')
                  else
                     q.text(val.to_s)
                  end
               end
            }
            q.comma_breakable unless mem == members.last
         }
      }
   end
   
   private
   
   # Returns the file system's block size.
   #
   def get_blksize(file)
      size = nil
   
      sectors = [0].pack('L')
      bytes   = [0].pack('L')
      free    = [0].pack('L')
      total   = [0].pack('L')
      
      # If there's a drive letter it must contain a trailing backslash.
      # The dup is necessary here because, for some odd reason, the function
      # appears to modify the argument passed in.
      if PathStripToRoot(file.dup)
         file += "\\" unless file[-1].chr == "\\"
      else
         file = 0 # Default to root drive
      end
      
      # Don't check for an error here.  Just default to nil.
      if GetDiskFreeSpace(file, sectors, bytes, free, total)
         size = sectors.unpack('L').first * bytes.unpack('L').first
      end
      
      size
   end
   
   # Returns the file's type (as a numeric).
   #
   def get_file_type(file)
      handle = CreateFile(
         file,
         0,
         0,
         0,
         OPEN_EXISTING,
         FILE_FLAG_BACKUP_SEMANTICS, # Need this for directories
         0
      )
      
      if handle == INVALID_HANDLE_VALUE
         raise ArgumentError, get_last_error
      end
      
      file_type = GetFileType(handle)
      error = GetLastError()
           
      CloseHandle(handle)
      
      if error == 0
         if file_type == FILE_TYPE_DISK || file_type == FILE_TYPE_UNKNOWN
            @blockdev = true
         end
      end
      
      file_type
   end
   
   # Verifies that a value is either true or false
   def check_bool(val)
      raise TypeError unless val == true || val == false
   end
end
