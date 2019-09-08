# For those who don't like gems...
require 'rbconfig'
require 'ftools'
include Config

sitelibdir = CONFIG['sitelibdir']
installdir = sitelibdir + '/win32'
file1 = 'lib\win32\eventlog.rb'
file2 = 'lib\win32\mc.rb'

Dir.mkdir(installdir) unless File.exists?(installdir)
File.copy(file1, installdir, true)
File.copy(file2, installdir, true)