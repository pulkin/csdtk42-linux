Gem::Specification.new do |s|
  s.name = %q{win32-eventlog}
  s.version = "0.4.3"
  s.date = %q{2006-12-18}
  s.summary = %q{Interface for the MS Windows Event Log.}
  s.email = %q{djberg96@gmail.com}
  s.homepage = %q{http://www.rubyforge.org/projects/win32utils}
  s.description = %q{Interface for the MS Windows Event Log.}
  s.has_rdoc = true
  s.authors = ["Daniel J. Berger"]
  s.files = ["lib/win32/eventlog.rb", "lib/win32/mc.rb", "test/CVS", "test/foo.mc", "test/tc_eventlog.rb", "test/tc_mc.rb", "test/ts_all.rb", "CHANGES", "CVS", "doc", "examples", "install.rb", "lib", "MANIFEST", "misc", "README", "test", "win32-eventlog.gemspec", "doc/tutorial.txt"]
  s.test_files = ["test/ts_all.rb"]
  s.extra_rdoc_files = ["README", "CHANGES", "doc/tutorial.txt"]
  s.add_dependency(%q<windows-pr>, [">= 0.5.0"])
end
