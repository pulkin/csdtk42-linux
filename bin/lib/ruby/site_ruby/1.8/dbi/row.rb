#
# $Id: row.rb,v 1.8 2003/09/07 12:36:43 mneumann Exp $
#

require "delegate"


# TODO: a method which returns the column-names,
#       #columns or #names ? 


module DBI

class Row < DelegateClass(Array)

  include Enumerable

  def initialize(col_names, size_or_arr=nil)
    size_or_arr ||= col_names.size 

    if size_or_arr.is_a? Integer
      @arr = Array.new(size_or_arr)
    elsif size_or_arr.is_a? Array
      @arr = size_or_arr
   else
      raise ArgumentError, "parameter must be either Integer or Array"   
    end

    @col_map = {}
    @col_names = col_names
    col_names.each_with_index {|c,i| @col_map[c] = i}
    super(@arr)
  end

  # Modifiers --------------------------------------------------------

  def set_values(new_values)
    # Accept an array of new values
    new_values.each_with_index do |v,i|
      @arr[i] = v
    end
  end

  def []=(*args)
     #if args.size == 1
     #  raise ArgumentError, "wrong # of arguments(#{args.size} for at least 2)"
     if args.size == 2
       @arr[conv_param(args[0])] = args[-1]
     elsif args.size == 3
       @arr[conv_param(args[0]), conv_param(args[1])] = args[-1]
     else
       raise ArgumentError, "wrong # of arguments(#{args.size} for 2)"
     end
  end

  # Cloning and Conversion -------------------------------------------
    
  def to_h
    hash = {}
    each_with_name {|v, n| hash[n] = v}
    hash
  end

 
  def clone_with(new_values)
    # Create a new row with 'new_values', reusing the field name hash. 
    Row.new(@col_names, new_values)
  end
 
  # Queries ----------------------------------------------------------

  def column_names
    @col_names
  end

  alias field_names column_names

  def by_index(index)
    # Value at 'index'.
    @arr[index]
  end
    
  def by_field(field_name)
    # Value of the field named 'field_name'.
    @arr[@col_map[field_name.to_s]]
  rescue TypeError
    nil
  end
 
  def [](*args)
     if args.size == 0
       raise ArgumentError, "wrong # of arguments(#{args.size} for at least 1)"
       # what todo if no param? => .to_a? .dup?
     elsif args.size == 1
       if args[0].is_a? Array
         args[0].collect {|i| self[i]}
       elsif args[0].is_a? Regexp
         cols = @col_names.grep args[0] 
         self[cols]
       else
         @arr[conv_param(args[0])]
       end
     elsif args.size == 2
       @arr[conv_param(args[0]), conv_param(args[1])]
     else
       args.collect {|i| self[i]}
       #raise ArgumentError, "wrong # of arguments(#{args.size} for 2)"
     end
  rescue TypeError
    nil
  end

  def each_with_name
    @arr.each_with_index do |v, i|
      yield v, @col_names[i]
    end 
  end

  def clone
    clone_with(@arr.dup)
  end

  alias dup clone

  private # ----------------------------------------------------------

  def conv_param(p)
    if p.is_a? String or p.is_a? Symbol then
      @col_map[p.to_s]
    elsif p.is_a? Range then

      if p.first.is_a? String or p.first.is_a? Symbol then
        first = @col_map[p.first.to_s]
      else
        first = p.first
      end

      if p.last.is_a? String or p.last.is_a? Symbol then
        last = @col_map[p.last.to_s]
      else
        last = p.last
      end

      if p.exclude_end?
        (first..last)
      else
        (first..last)
      end

    else
      p
    end
  end

end # class Row

end # module DBI
