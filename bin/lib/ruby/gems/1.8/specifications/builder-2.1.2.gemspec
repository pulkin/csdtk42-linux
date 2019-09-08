Gem::Specification.new do |s|
  s.name = %q{builder}
  s.version = "2.1.2"
  s.date = %q{2007-06-15}
  s.summary = %q{Builders for MarkUp.}
  s.email = %q{jim@weirichhouse.org}
  s.homepage = %q{http://onestepback.org}
  s.description = %q{Builder provides a number of builder objects that make creating structured data simple to do.  Currently the following builder objects are supported:  * XML Markup * XML Events}
  s.autorequire = %q{builder}
  s.has_rdoc = true
  s.authors = ["Jim Weirich"]
  s.files = ["lib/blankslate.rb", "lib/builder.rb", "lib/builder/blankslate.rb", "lib/builder/xchar.rb", "lib/builder/xmlbase.rb", "lib/builder/xmlevents.rb", "lib/builder/xmlmarkup.rb", "test/performance.rb", "test/preload.rb", "test/test_xchar.rb", "test/testblankslate.rb", "test/testeventbuilder.rb", "test/testmarkupbuilder.rb", "scripts/publish.rb", "CHANGES", "Rakefile", "README", "doc/releases/builder-1.2.4.rdoc", "doc/releases/builder-2.0.0.rdoc", "doc/releases/builder-2.1.1.rdoc"]
  s.test_files = ["test/test_xchar.rb", "test/testblankslate.rb", "test/testeventbuilder.rb", "test/testmarkupbuilder.rb"]
  s.rdoc_options = ["--title", "Builder -- Easy XML Building", "--main", "README", "--line-numbers"]
  s.extra_rdoc_files = ["CHANGES", "Rakefile", "README", "doc/releases/builder-1.2.4.rdoc", "doc/releases/builder-2.0.0.rdoc", "doc/releases/builder-2.1.1.rdoc"]
end
