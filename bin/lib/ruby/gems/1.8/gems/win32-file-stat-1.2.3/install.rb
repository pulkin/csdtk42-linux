require 'rbconfig'
require 'ftools'
include Config

sitelibdir = CONFIG['sitelibdir']
installdir = sitelibdir + '/win32/file'
basedir = File.dirname(installdir)
file = 'lib\win32\file\stat.rb'

Dir.mkdir(basedir) unless File.exists?(basedir)
Dir.mkdir(installdir) unless File.exists?(installdir)
File.copy(file, installdir, true)