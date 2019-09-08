# 
# $Id: columninfo.rb,v 1.1 2001/08/30 13:31:11 michael Exp $
#
# by Michael Neumann (neumann@s-direktnet.de)
#

class ColumnInfo

  # define attribute accessors for the following attributes:
  attrs = %w(name sql_type type_name precision scale default nullable indexed primary unique)
  attrs.each do | attr |
    eval %{
      def #{ attr }()         @hash['#{ attr }']         end
      def #{ attr }=( value ) @hash['#{ attr }'] = value end
    }
  end

  alias nullable? nullable
  alias is_nullable? nullable

  alias indexed? indexed
  alias is_indexed? indexed

  alias primary? primary
  alias is_primary? primary

  alias unique? unique
  alias is_unique unique

  alias size precision
  alias size= precision=
  alias length precision
  alias length= precision=


  alias decimal_digits scale
  alias decimal_digits= scale=

  # Constructor methods ------------------------------------------------------------------------

  def initialize( hash=nil )
    @hash = hash || Hash.new
  end

  # Attribute getter/setter --------------------------------------------------------------------
  
  def []( attr ) 
    @hash[attr]
  end

  def []=( attr, value ) 
    @hash[attr] = value
  end

  def keys
    @hash.keys
  end

  
  # to let a ColumnInfo behave like a Hash
  # (TODO: remove in later versions? only for compat. issues)
  def method_missing(id, *params, &b)
    @hash.send(id, *params, &b)
  end

end


