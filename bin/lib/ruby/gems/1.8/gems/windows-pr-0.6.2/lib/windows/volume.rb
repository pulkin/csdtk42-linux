require 'Win32API'

module Windows
   module Volume
      DRIVE_UNKNOWN     = 0
      DRIVE_NO_ROOT_DIR = 1
      DRIVE_REMOVABLE   = 2
      DRIVE_FIXED       = 3
      DRIVE_REMOTE      = 4
      DRIVE_CDROM       = 5
      DRIVE_RAMDISK     = 6
      
      FindFirstVolume           = Win32API.new('kernel32', 'FindFirstVolume', 'PL', 'L')
      FindFirstVolumeMountPoint = Win32API.new('kernel32', 'FindFirstVolumeMountPoint', 'PPL', 'L')
      FindNextVolume            = Win32API.new('kernel32', 'FindNextVolume', 'LPL', 'I')
      FindNextVolumeMountPoint  = Win32API.new('kernel32', 'FindNextVolumeMountPoint', 'LPL', 'I')
      FindVolumeClose           = Win32API.new('kernel32', 'FindVolumeClose', 'L', 'I')
      FindVolumeMountPointClose = Win32API.new('kernel32', 'FindVolumeMountPointClose', 'L', 'I')
      
      GetDriveType           = Win32API.new('kernel32', 'GetDriveType', 'P', 'I')
      GetLogicalDrives       = Win32API.new('kernel32', 'GetLogicalDrives', 'V', 'L')
      GetLogicalDriveStrings = Win32API.new('kernel32', 'GetLogicalDrives', 'LP', 'L')
      GetVolumeInformation   = Win32API.new('kernel32', 'GetVolumeInformation', 'PPLPPPPL', 'I')      
      GetVolumeNameForVolumeMountPoint = Win32API.new('kernel32', 'GetVolumeNameForVolumeMountPoint', 'PPL', 'I')
      GetVolumePathName      = Win32API.new('kernel32', 'GetVolumePathName', 'PPL', 'I')
      GetVolumePathNamesForVolumeName = Win32API.new('kernel32', 'GetVolumePathNamesForVolumeName', 'PPLL', 'I')

      SetVolumeLabel = Win32API.new('kernel32', 'SetVolumeLabel', 'PP', 'I')
      SetVolumeMountPoint = Win32API.new('kernel32', 'SetVolumeMountPoint', 'PP', 'I')
      
      def FindFirstVolume(name, length=name.size)
         FindFirstVolume.call(name, length)
      end
      
      def FindFirstVolumeMountPoint(name, volume, length=volume.size)
         FindFirstVolumeMountPoint.call(name, volume, length)
      end
      
      def FindNextVolume(handle, name, length=name.size)
         FindNextVolume.call(handle, name, length) != 0
      end
      
      def FindNextVolumeMountPoint(handle, name, length=name.size)
         FindNextVolumeMountPoint.call(handle, name, length) != 0
      end
      
      def FindVolumeClose(handle)
         FindVolumeClose.call(handle) != 0
      end
      
      def FindVolumeMountPointClose(handle)
         FindVolumeMountPointClose.call(handle) != 0
      end
      
      def GetDriveType(path)
         GetDriveType.call(path)
      end
      
      def GetLogicalDrives()
         GetLogicalDrives.call()
      end
      
      def GetLogicalDriveStrings(length, buffer)
         GetLogicalDriveStrings.call(length, buffer)
      end
      
      def GetVolumeInformation(path, v_buf, v_size, serial, length, flags, fs_buf, fs_size)
         GetVolumeInformation.call(path, v_buf, v_size, serial, length, flags, fs_buf, fs_size) != 0
      end
      
      def GetVolumeNameForVolumeMountPoint(mount, name, length=name.size)
         GetVolumeNameForVolumeMountPoint.call(mount, name, length) != 0
      end
      
      def GetVolumePathName(file, name, length = name.size)
         GetVolumePathName.call(file, name, length) != 0
      end
      
      def GetVolumePathNamesForVolumeName(volume, paths, buf_len, return_len)
         GetVolumePathNamesForVolumeName.call(volume, paths, buf_len, return_len) != 0
      end
      
      def SetVolumeLabel(label, volume)
         SetVolumeLabel.call(label, volume) != 0
      end
      
      def SetVolumeMountPoint(mount, name)
         SetVolumeMountPoint.call(mount, name) != 0
      end
   end
end