require "rubygems"

spec = Gem::Specification.new do |gem|
   gem.name        = "windows-pr"
   gem.version     = "0.6.2"
   gem.author      = "Daniel J. Berger"
   gem.email       = "djberg96@gmail.com"
   gem.homepage    = "http://www.rubyforge.org/projects/win32utils"
   gem.platform    = Gem::Platform::RUBY
   gem.summary     = "Windows functions and constants predefined via Win32API"
   gem.description = "Windows functions and constants predefined via Win32API"
   gem.test_file   = "test/ts_all.rb"
   gem.has_rdoc    = true
   gem.files       = Dir["doc/*.txt"] + Dir["lib/windows/*.rb"]
   gem.files += Dir["lib/windows/msvcrt/*.rb"] 
   gem.files += Dir["test/*"] + Dir["[A-Z]*"]
   gem.files.reject! { |fn| fn.include? "CVS" }
   gem.require_path = "lib"
   gem.extra_rdoc_files = ["README", "CHANGES"]
end

if $0 == __FILE__
   Gem.manage_gems
   Gem::Builder.new(spec).build
end
