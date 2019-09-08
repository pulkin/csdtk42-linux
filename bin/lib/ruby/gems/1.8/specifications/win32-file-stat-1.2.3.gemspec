Gem::Specification.new do |s|
  s.name = %q{win32-file-stat}
  s.version = "1.2.3"
  s.date = %q{2006-11-04}
  s.summary = %q{A File::Stat class tailored to MS Windows}
  s.email = %q{djberg96@gmail.com}
  s.homepage = %q{http://www.rubyforge.org/projects/win32utils}
  s.description = %q{A File::Stat class tailored to MS Windows}
  s.has_rdoc = true
  s.authors = ["Daniel J. Berger"]
  s.files = ["lib/win32/file/stat.rb", "CHANGES", "install.rb", "lib", "MANIFEST", "README", "test", "win32-file-stat.gemspec", "test/sometestfile.exe", "test/sometestfile.txt", "test/tc_file_stat.rb"]
  s.test_files = ["test/tc_file_stat.rb"]
  s.extra_rdoc_files = ["README", "CHANGES"]
  s.add_dependency(%q<windows-pr>, [">= 0.4.0"])
end
