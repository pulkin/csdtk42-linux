Gem::Specification.new do |s|
  s.name = %q{win32-clipboard}
  s.version = "0.4.1"
  s.date = %q{2006-07-30}
  s.summary = %q{A package for interacting with the Windows clipboard}
  s.email = %q{djberg96@gmail.com}
  s.homepage = %q{http://www.rubyforge.org/projects/win32utils}
  s.rubyforge_project = %q{win32utils}
  s.description = %q{A package for interacting with the Windows clipboard}
  s.has_rdoc = true
  s.authors = ["Daniel J. Berger"]
  s.files = ["examples/clipboard_test.rb", "lib/win32/clipboard.rb", "test/tc_clipboard.rb", "CHANGES", "MANIFEST", "README"]
  s.test_files = ["test/tc_clipboard.rb"]
  s.extra_rdoc_files = ["CHANGES", "README"]
  s.add_dependency(%q<windows-pr>, [">= 0.4.1"])
end
