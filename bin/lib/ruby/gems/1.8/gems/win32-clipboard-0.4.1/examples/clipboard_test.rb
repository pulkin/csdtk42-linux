##########################################################################
# clipboard_test.rb (win32-clipboard)
#
# Generic test script for those without TestUnit installed, or for
# general futzing.
##########################################################################
base = File.basename(Dir.pwd)
if base == "examples" || base =~ /win32-clipboard/
	require "ftools"
	Dir.chdir("..") if base == "examples"
	Dir.mkdir("win32") unless File.exists?("win32")
	File.copy("clipboard.so","win32")
	$LOAD_PATH.unshift Dir.pwd
	Dir.chdir("examples") if base =~ /win32-clipboard/
end

require "win32/clipboard"
require "pp"
include Win32

puts "VERSION: " + Clipboard::VERSION

pp Clipboard.formats
pp Clipboard.data(Clipboard::DIB)
pp Clipboard.format_available?(49161)
pp Clipboard.format_name(999999999)
pp Clipboard.format_available?(9999999)

puts "Data was: [" + Clipboard.data + "]"

Clipboard.set_data("foobar")

puts "Data is now: [" + Clipboard.data + "]"

puts "Number of available formats: " + Clipboard.num_formats.to_s

Clipboard.empty

puts "Clipboard emptied"