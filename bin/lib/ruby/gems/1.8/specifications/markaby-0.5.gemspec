Gem::Specification.new do |s|
  s.name = %q{markaby}
  s.version = "0.5"
  s.date = %q{2006-10-03}
  s.summary = %q{Markup as Ruby, write HTML in your native Ruby tongue}
  s.has_rdoc = true
  s.authors = ["Tim Fletcher and _why"]
  s.files = ["README", "Rakefile", "setup.rb", "test/test_markaby.rb", "lib/markaby", "lib/markaby.rb", "lib/markaby/metaid.rb", "lib/markaby/tags.rb", "lib/markaby/builder.rb", "lib/markaby/cssproxy.rb", "lib/markaby/rails.rb", "lib/markaby/template.rb", "tools/rakehelp.rb"]
  s.test_files = ["test/test_markaby.rb"]
  s.extra_rdoc_files = ["README"]
  s.add_dependency(%q<builder>, [">= 2.0.0"])
end
