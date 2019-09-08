require "rubygems"

spec = Gem::Specification.new do |gem|
   gem.name        = "win32-eventlog"
   gem.version     = "0.4.3"
   gem.author      = "Daniel J. Berger"
   gem.email       = "djberg96@gmail.com"
   gem.homepage    = "http://www.rubyforge.org/projects/win32utils"
   gem.platform    = Gem::Platform::RUBY
   gem.summary     = "Interface for the MS Windows Event Log."
   gem.description = "Interface for the MS Windows Event Log."
   gem.test_file   = "test/ts_all.rb"
   gem.has_rdoc    = true
   gem.files       = Dir["lib/win32/*.rb"] + Dir["test/*"] + Dir["[A-Z]*"]
   gem.files.reject! { |fn| fn.include? "CVS" }
   gem.require_path = "lib"
   gem.extra_rdoc_files = ["README", "CHANGES", "doc/tutorial.txt"]
   gem.add_dependency("windows-pr", ">= 0.5.0")
end

if $0 == __FILE__
   Gem.manage_gems
   Gem::Builder.new(spec).build
end
