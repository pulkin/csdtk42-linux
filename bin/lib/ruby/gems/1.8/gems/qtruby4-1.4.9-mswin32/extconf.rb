require 'mkmf'

require 'fileutils'

FileUtils.mv Dir["bin/*"], "../../../../../../bin/"

FileUtils.mv "lib/Makefile", "Makefile"

FileUtils.rmdir "bin"
