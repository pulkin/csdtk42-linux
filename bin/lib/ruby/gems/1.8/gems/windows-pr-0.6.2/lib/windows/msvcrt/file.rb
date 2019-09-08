require 'Win32API'

module Windows
   module MSVCRT
      module File
         S_IFMT   = 0170000 # file type mask
         S_IFDIR  = 0040000 # directory
         S_IFCHR  = 0020000 # character special
         S_IFIFO  = 0010000 # pipe
         S_IFREG  = 0100000 # regular
         S_IREAD  = 0000400 # read permission, owner
         S_IWRITE = 0000200 # write permission, owner
         S_IEXEC  = 0000100 # execute/search permission, owner

         Stat   = Win32API.new('msvcrt', '_stat', 'PP', 'I')
         Stat64 = Win32API.new('msvcrt', '_stat64', 'PP', 'I')
         
         def stat(path, buffer)
            Stat.call(path, buffer)
         end
      
         def stat64(path, buffer)
            Stat64.call(path, buffer)
         end
      end
   end
end