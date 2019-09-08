require 'Win32API'

module Windows
   module FileMapping     
      FILE_MAP_COPY       = 0x0001
      FILE_MAP_WRITE      = 0x0002
      FILE_MAP_READ       = 0x0004
      FILE_MAP_ALL_ACCESS = 983071
      
      PAGE_NOACCESS          = 0x01     
      PAGE_READONLY          = 0x02     
      PAGE_READWRITE         = 0x04     
      PAGE_WRITECOPY         = 0x08     
      PAGE_EXECUTE           = 0x10     
      PAGE_EXECUTE_READ      = 0x20     
      PAGE_EXECUTE_READWRITE = 0x40     
      PAGE_EXECUTE_WRITECOPY = 0x80     
      PAGE_GUARD             = 0x100     
      PAGE_NOCACHE           = 0x200     
      PAGE_WRITECOMBINE      = 0x400
      SEC_FILE               = 0x800000     
      SEC_IMAGE              = 0x1000000     
      SEC_VLM                = 0x2000000     
      SEC_RESERVE            = 0x4000000     
      SEC_COMMIT             = 0x8000000     
      SEC_NOCACHE            = 0x10000000
      
      CreateFileMapping = Win32API.new('kernel32', 'CreateFileMapping', 'LPLLLP', 'L')
      FlushViewOfFile   = Win32API.new('kernel32', 'FlushViewOfFile', 'PL', 'I')
      MapViewOfFile     = Win32API.new('kernel32', 'MapViewOfFile', 'LLLLL', 'L')
      MapViewOfFileEx   = Win32API.new('kernel32', 'MapViewOfFileEx', 'LLLLLL', 'L')
      OpenFileMapping   = Win32API.new('kernel32', 'OpenFileMapping', 'LIP', 'L')
      UnmapViewOfFile   = Win32API.new('kernel32', 'UnmapViewOfFile', 'P', 'I')
      
      def CreateFileMapping(handle, security, protect, high, low, name)
         CreateFileMapping.call(handle, security, protect, high, low, name)
      end
      
      def FlushViewOfFile(address, bytes)
         FlushViewOfFile.call(address, bytes) != 0
      end
      
      def MapViewOfFile(handle, access, high, low, bytes)
         MapViewOfFile.call(handle, access, high, low, bytes)
      end
      
      def MapViewOfFileEx(handle, access, high, low, bytes, address)
         MapViewOfFileEx.call(handle, access, high, low, bytes, address)
      end

      def OpenFileMapping(access, inherit, name)
         OpenFileMapping.call(access, inherit, name)
      end
      
      def UnmapViewOfFile(address)
         UnmapViewOfFile.call(address) != 0
      end
   end
end