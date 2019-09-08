require 'Win32API'

module Windows
   module MSVCRT
      module Buffer
         Memcpy  = Win32API.new('msvcrt', 'memcpy', 'PLL', 'P')
         Memccpy = Win32API.new('msvcrt', '_memccpy', 'PPIL', 'P')
         Memchr  = Win32API.new('msvcrt', 'memchr', 'PIL', 'P')
         Memcmp  = Win32API.new('msvcrt', 'memcmp', 'PPL', 'I')
         Memicmp = Win32API.new('msvcrt', '_memicmp', 'PPL', 'I')
         Memmove = Win32API.new('msvcrt', 'memmove', 'PPL', 'P')
         Memset  = Win32API.new('msvcrt', 'memset', 'PLL', 'L')
         Swab    = Win32API.new('msvcrt', '_swab', 'PPI', 'V')
         
         MemcpyPLL  = Win32API.new('msvcrt', 'memcpy', 'PLL', 'P')
         MemcpyLPL  = Win32API.new('msvcrt', 'memcpy', 'LPL', 'P')
         MemcpyLLL  = Win32API.new('msvcrt', 'memcpy', 'LLL', 'P')
         MemcpyPPL  = Win32API.new('msvcrt', 'memcpy', 'PPL', 'P')
      
         # Wrapper for the memcpy() function.  Both the +dest+ and +src+ can
         # be either a string or a memory address.  If +size+ is omitted, it
         # defaults to the length of +src+.
         # 
         def memcpy(dest, src, size = src.length)
            if dest.is_a?(Integer)
               if src.is_a?(String)
                  MemcpyLPL.call(dest, src, size)
               else
                  MemcpyLLL.call(dest, src, size)
               end
            else
               if src.is_a?(String)
                  MemcpyPPL.call(dest, src, size)
               else
                  MemcpyPLL.call(dest, src, size)
               end
            end
         end
      
         def memccpy(dest, src, char, count)
            Memccpy.call(dest, src, char, count)
         end
         
         def memchr(buf, char, count)
            Memchr.call(buf, char, count)
         end
         
         def memcmp(buf1, buf2, count)
            Memcmp.call(buf1, buf2, count)
         end
         
         def memicmp(buf1, buf2, count)
            Memicmp.call(buf1, buf2, count)
         end
         
         def memmove(dest, src, count)
            Memmove.call(dest, src, count)
         end
      
         def memset(dest, char, count)
            Memset.call(dest, char, count)
         end
         
         def swab(src, dest, count)
            Swab.call(src, dest, count)
         end
      end
   end
end