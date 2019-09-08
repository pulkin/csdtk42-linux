require 'Win32API'

module Windows
   module Handle
      INVALID_HANDLE_VALUE = -1
      
      HANDLE_FLAG_INHERIT            = 0x00000001
      HANDLE_FLAG_PROTECT_FROM_CLOSE = 0x00000002
      
      CloseHandle          = Win32API.new('kernel32', 'CloseHandle', 'L', 'I')
      DuplicateHandle      = Win32API.new('kernel32', 'DuplicateHandle', 'LLLLLIL', 'I')
      GetHandleInformation = Win32API.new('kernel32', 'GetHandleInformation', 'LL', 'I')
      SetHandleInformation = Win32API.new('kernel32', 'SetHandleInformation', 'LLL', 'I')

      GetOSFHandle         = Win32API.new('msvcrt', '_get_osfhandle', 'I', 'L')
      OpenOSFHandle        = Win32API.new('msvcrt', '_open_osfhandle', 'LI', 'I')
            
      def CloseHandle(handle)
         CloseHandle.call(handle) != 0
      end
      
      def DuplicateHandle(sphandle, shandle, thandle, access, ihandle, opts)
         DuplicateHandle.call(sphandle, shandle, thandle, access, ihandle, opts) != 0
      end
      
      def GetHandleInformation(handle, flags)
         GetHandleInformation.call(handle, flags) != 0
      end
      
      def SetHandleInformation(handle, mask, flags)
         SetHandleInformation.call(handle, mask, flags) != 0
      end
      
      def get_osfhandle(fd)
         GetOSFHandle.call(fd)
      end
      
      def open_osfhandle(handle, flags)
         OpenOSFHandle.call(handle, flags)
      end
   end
end