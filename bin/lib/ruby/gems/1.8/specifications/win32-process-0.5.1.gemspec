Gem::Specification.new do |s|
  s.name = %q{win32-process}
  s.version = "0.5.1"
  s.date = %q{2006-08-24}
  s.summary = %q{Adds fork, wait, wait2, waitpid, waitpid2 and a special kill method}
  s.email = %q{djberg96@gmail.com}
  s.homepage = %q{http://www.rubyforge.org/projects/win32utils}
  s.description = %q{Adds fork, wait, wait2, waitpid, waitpid2 and a special kill method}
  s.has_rdoc = true
  s.authors = ["Daniel J. Berger"]
  s.files = ["lib/win32/process.rb", "test/CVS", "test/tc_process.rb", "CHANGES", "CVS", "MANIFEST", "README"]
  s.test_files = ["test/tc_process.rb"]
  s.extra_rdoc_files = ["README", "CHANGES"]
  s.add_dependency(%q<windows-pr>, [">= 0.5.2"])
end
