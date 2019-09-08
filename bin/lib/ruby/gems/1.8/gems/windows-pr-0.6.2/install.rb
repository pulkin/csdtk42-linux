require "ftools"
require "rbconfig"
include Config

sitelibdir  = CONFIG["sitelibdir"]
installdir  = sitelibdir + "/windows"
installdir2 = sitelibdir + "/windows/msvcrt"

Dir.mkdir(installdir) unless File.exists?(installdir)
Dir.mkdir(installdir2) unless File.exists?(installdir2)

Dir["lib/windows/*.rb"].each{ |file|
   File.copy(file, installdir, true)
}

Dir["lib/windows/msvcrt/*.rb"].each{ |file|
   File.copy(file, installdir2, true)
}