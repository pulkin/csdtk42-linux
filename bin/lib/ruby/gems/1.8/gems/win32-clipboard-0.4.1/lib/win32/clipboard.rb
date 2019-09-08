require 'windows/clipboard'
require 'windows/memory'
require 'windows/error'

module Win32
   class ClipboardError < StandardError; end
   class Clipboard
      include Windows::Clipboard
      include Windows::Memory
      include Windows::Error
      extend Windows::Clipboard
      extend Windows::Memory
      extend Windows::Error
      
      VERSION = '0.4.1'
      
      # Clipboard data types
      TEXT        = CF_TEXT
      OEMTEXT     = CF_OEMTEXT
      UNICODETEXT = CF_UNICODETEXT

      # We'll use a custom definition of memcpy since this is different than
      # what's in the Windows::MSVCRT::Buffer module.
      @@Memcpy = Win32API.new('msvcrt',  'memcpy', 'IPI', 'I')

      # Sets the clipboard contents to the data that you specify.  You may
      # optionally specify a clipboard format.  The default is Clipboard::TEXT.
      # 
      def self.set_data(clip_data, format = TEXT)
         self.open
         EmptyClipboard()

         # NULL terminate text
         case format
            when TEXT, OEMTEXT, UNICODETEXT				
               clip_data << "\0"
         end

         # Global Allocate a movable piece of memory.
         hmem = GlobalAlloc(GHND, clip_data.length + 4)
         mem  = GlobalLock(hmem)
         @@Memcpy.call(mem, clip_data, clip_data.length)

         # Set the new data
         if SetClipboardData(format, hmem) == 0
            error = get_last_error
            GlobalFree(hmem)
            self.close
            raise ClipboardError, "SetClipboardData() failed: #{error}"
         end

         GlobalFree(hmem)   
         self.close
         self
      end 		

      # Returns the data currently in the clipboard.  If +format+ is
      # specified, it will attempt to retrieve the data in that format. The
      # default is Clipboard::TEXT.
      # 
      def self.data(format = TEXT)
         clipdata = ""
         self.open
         begin
            clipdata = GetClipboardData(format)
         rescue ArgumentError
            # Assume failure is caused by no data in clipboard
         end
         self.close
         clipdata
      end
      
      # Singleton aliases
      # 
      class << self
         alias :get_data :data
      end

      # Empties the contents of the clipboard.
      # 
      def self.empty
         self.open
         EmptyClipboard()
         self.close
         self
      end

      # Returns the number of different data formats currently on the
      # clipboard.
      # 
      def self.num_formats
         count = 0
         self.open
         count = CountClipboardFormats()
         self.close
         count
      end
      
      # Returns whether or not +format+ (an int) is currently available.
      # 
      def self.format_available?(format)
         IsClipboardFormatAvailable(format)
      end
      
      # Returns the corresponding name for the given +format_num+, or nil
      # if it does not exist.  You cannot specify any of the predefined
      # clipboard formats (or nil is returned), only registered formats.
      # 
      def self.format_name(format_num)
         val = nil
         buf = 0.chr * 80
         self.open
         if GetClipboardFormatName(format_num, buf, buf.length) != 0
            val = buf
         end
         self.close
         val.split(0.chr).first rescue nil
      end
      
      # Returns a hash of all the current formats, with the format number as
      # the key and the format name as the value for that key.
      # 
      def self.formats
         formats = {}
         format = 0
         
         self.open
         while 0 != (format = EnumClipboardFormats(format))
            buf = 0.chr * 80
            GetClipboardFormatName(format, buf, buf.length)
            formats[format] = buf.split(0.chr).first
         end
         self.close
         formats
      end
      
      # Registers the given +format+ (a String) as a clipboard format, which
      # can then be used as a valid clipboard format.
      #
      # If a registered format with the specified name already exists, a new
      # format is not registered and the return value identifies the existing
      # format. This enables more than one application to copy and paste data
      # using the same registered clipboard format. Note that the format name
      # comparison is case-insensitive.
      #
      # Registered clipboard formats are identified by values in the range 0xC000
      # through 0xFFFF.
      # 
      def self.register_format(format)
         if RegisterClipboardFormat(format) == 0
            error = "RegisterClipboardFormat() call failed: " + get_last_error
            raise ClipboardError,  error
         end
      end

      private

      def self.open
         if 0 == OpenClipboard(0)
            raise ClipboardError, "OpenClipboard() failed: " + get_last_error
         end
      end

      def self.close
         CloseClipboard()
      end
   end
end
