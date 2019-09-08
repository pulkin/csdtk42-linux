Gem::Specification.new do |s|
  s.name = %q{win32-file}
  s.version = "0.5.3"
  s.date = %q{2006-11-02}
  s.summary = %q{Extra or redefined methods for the File class on Windows.}
  s.email = %q{djberg96@gmail.com}
  s.homepage = %q{http://www.rubyforge.org/projects/win32utils}
  s.description = %q{Extra or redefined methods for the File class on Windows.}
  s.has_rdoc = true
  s.authors = ["Daniel J. Berger"]
  s.files = ["lib/win32/file.rb", "test/sometestfile.txt", "test/tc_file_attributes.rb", "test/tc_file_constants.rb", "test/tc_file_encryption.rb", "test/tc_file_path.rb", "test/tc_file_security.rb", "test/tc_file_stat.rb", "test/ts_all.rb", "CHANGES", "install.rb", "lib", "MANIFEST", "README", "test", "win32-file.gemspec"]
  s.test_files = ["test/ts_all.rb"]
  s.extra_rdoc_files = ["README", "CHANGES"]
  s.add_dependency(%q<win32-file-stat>, [">= 1.2.0"])
end
