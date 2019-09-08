Gem::Specification.new do |s|
  s.name = %q{win32-sapi}
  s.version = "0.1.3"
  s.date = %q{2005-11-09}
  s.summary = %q{An interface to the MS SAPI (Sound API) library.}
  s.email = %q{djberg96@gmail.com}
  s.homepage = %q{http://www.rubyforge.org/projects/win32utils}
  s.description = %q{An interface to the MS SAPI (Sound API) library.}
  s.has_rdoc = true
  s.platform = %q{mswin32}
  s.authors = ["Daniel J. Berger"]
  s.files = ["lib/win32/sapi5.rb", "CHANGES", "MANIFEST", "README", "test/tc_sapi5.rb"]
  s.test_files = ["test/tc_sapi5.rb"]
  s.extra_rdoc_files = ["README", "CHANGES"]
end
