# For those who don't like gems...
require 'rbconfig'
require 'ftools'
include Config

sitelibdir = CONFIG['sitelibdir']
installdir = sitelibdir + '/win32'
file = 'lib\win32\file.rb'

Dir.mkdir(installdir) unless File.exists?(installdir)
File.copy(file, installdir, true)