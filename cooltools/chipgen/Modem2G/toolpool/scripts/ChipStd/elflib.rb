#############################################################
#															#
#    Elf Utilies for reading and writing elf files          #
#															#
#############################################################

class Elf_Header
    attr_accessor :ident, :type, :machine, :version, :entry, :phoff, :shoff, :flags, :ehsize, :phentsize, :phnum, :shentsize, :shnum, :shstrndx
    
    def initialize
        @ident = 0x0.chr * 16
        @type = 0x0.chr * 2
        @machine = 0x0.chr * 2
        @version = 0x0.chr * 4
        @entry = 0x0.chr * 4
        @phoff = 0x0.chr * 4
        @shoff = 0x0.chr * 4
        @flags = 0x0.chr * 4
        @ehsize = 0x0.chr * 2
        @phentsize = 0x0.chr * 2
        @phnum = 0x0.chr * 2
        @shentsize = 0x0.chr * 2
        @shnum = 0x0.chr * 2
        @shstrndx = 0x0.chr * 2
    end

    def write_ident(elf_class, elf_data, elf_version)
    	hh = ""
    	hh << 0x7f << 'E' << 'L' << 'F' << elf_class << elf_data << elf_version << 0.chr * 8 << 0
    	ident[0..15] = hh
    end

    def write_type(type)
        valid_types = [0, 1, 2, 3, 4, 0xff00, 0xffff]
        if !valid_types.member?(type)
            raise 'Invalid ELF type specified'
        else
            @type = [type].pack('S')
        end
    end

    def write_machine(machine)
        valid_machines = [0,1,2,3,4,5,6,7,8,10]
        if !valid_machines.member?(machine)
            raise 'Invalid machine specified'
        else
            @machine = [machine].pack('S')
        end
    end

    def write_version(version)
        if ![0,1].member?(version)
            raise 'Invalid version specified'
        else
            @version = [version].pack('I')
        end
    end

    def write_entry(entry)
        @entry = [entry].pack('I')
    end

    def write_offsets(phoff, shoff)
        @phoff = [phoff].pack('I')
        @shoff = [shoff].pack('I')
    end

    def write_flags(flags)
        @flags = [flags].pack('I')
    end

    def write_sizes(ehsize, phentsize, shentsize)
        @ehsize = [ehsize].pack('S')
        @phentsize = [phentsize].pack('S')
        @shentsize = [shentsize].pack('S')
    end

    def write_nums(phnum, shnum)
        @phnum = [phnum].pack('S')
        @shnum = [shnum].pack('S')
    end

    def write_strndx(strndx)
        @shstrndx = [strndx].pack('S')
    end

    def get_header
        header = ident << type << machine << version << entry << phoff << shoff << flags << ehsize << phentsize << phnum << shentsize << shnum << shstrndx
        return header
    end

end

  ## ELF Program Header
 class Program_Header
      attr_accessor :type, :offset, :vaddr, :vaddr, :filesz, :memsz, :flags, :align
      def initialize
          @type = 0x0.chr * 4
          @offset = 0x0.chr * 4
          @vaddr  = 0x0.chr * 4
          @paddr = 0x0.chr * 4
          @filesz = 0x0.chr * 4
          @memsz = 0x0.chr * 4
          @flags = 0x0.chr * 4
          @align = 0x0.chr * 4
      end
      
      def write_type(type)
          @type = [type].pack('I')
      end
      
      def write_offset(offset)
          @offset = [offset].pack('I')
      end
      
      def write_addr(vaddr,paddr)
          @vaddr = [vaddr].pack('I') 
          @paddr = [paddr].pack('I') 
      end
      
      def write_fsize(filesz)
          @filesz = [filesz].pack('I')
      end
      
      def write_msize(memsz)
          @memsz = [memsz].pack('I')
      end
      
      def write_flags(flags)
          @flags = [flags].pack('I')
      end
      
      def write_align(align)
          @align = [align].pack('I')
      end
      
      def get_header
          header = @type << @offset << @vaddr << @paddr << @filesz << @memsz << @flags << @align
          return header
      end
      
  end

  
class String_Table
    attr_accessor :contents
    def initialize(contents)
        @contents = contents
    end
end

class Section_Header_Table
    attr_accessor :reserved
    def initialize
        @sections = Array.new #index
        @reserved_indexes = [0].concat((0xff00..0xffff).to_a)
        @section_count = 0
        #Define some special sections
        add_section #Undef section
    end
    
    def add_section(name=0, type=0, flags=0, addr=0, offset=0, size=0, link=0, info=0, addr_align=0, entsize=0)
        #TODO:Check these values are correct tomorrow
        new_section = Section_Header.new
        new_section.write_name name
        new_section.write_type type
        new_section.write_flags flags
        new_section.write_addr addr
        new_section.write_offset offset
        new_section.write_size size
        new_section.write_link link
        new_section.write_info info
        new_section.write_addr_align addr_align
        new_section.write_entsize entsize
        @sections[@section_count] = new_section
        @section_count += 1
    end

    def get_table
        table = ""
        @sections.each do |section|
            table << section.get_header
        end
        puts "Size of one header entry: #{table.size/@sections.size}"
        return table
    end

end

class Section_Header
    def initialize
        @name = 0x0.chr * 4
        @type = 0x0.chr * 4
        @flags = 0x0.chr * 4
        @addr = 0x0.chr * 4
        @offset = 0x0.chr * 4
        @size = 0x0.chr * 4
        @link = 0x0.chr * 4
        @info = 0x0.chr * 4
        @addr_align = 0x0.chr * 4
        @entsize = 0x0.chr * 4
    end


    ###TODO: Find a better way to do this###
    def write_name(name)
        @name = [name].pack('I')
    end

    def write_type(type)
            @type = [type].pack('I')
    end

    def write_flags(flags)
        @flags = [flags].pack('I')
    end

    def write_addr(addr)
        @addr = [addr].pack('I') 
    end

    def write_offset(offset)
        @offset = [offset].pack('I')
    end

    def write_size(size)
        @size = [size].pack('I')
    end

    def write_link(link)
        @link = [link].pack('I')
    end

    def write_info(info)
        @info = [info].pack('I')
    end
    
    def write_addr_align(addr_align)
        @addr_align = [addr_align].pack('I')
    end

    def write_entsize(entsize)
        @entsize = [entsize].pack('I')
    end

    def get_header
        header = @name << @type << @flags << @addr << @offset << @size << @link << @info << @addr_align << @entsize
        return header
    end
end

  
