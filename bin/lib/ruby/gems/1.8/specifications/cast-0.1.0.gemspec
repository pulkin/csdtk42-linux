Gem::Specification.new do |s|
  s.name = %q{cast}
  s.version = "0.1.0"
  s.date = %q{2006-04-25}
  s.summary = %q{C parser and AST constructor.}
  s.email = %q{george.ogata@gmail.com}
  s.homepage = %q{http://cast.rubyforge.org}
  s.rubyforge_project = %q{cast}
  s.autorequire = %q{cast}
  s.authors = ["George Ogata"]
  s.files = ["README", "lib/cast", "lib/cast.rb", "lib/cast/node.rb", "lib/cast/c.y", "lib/cast/c_nodes.rb", "lib/cast/inspect.rb", "lib/cast/node_list.rb", "lib/cast/parse.rb", "lib/cast/to_s.rb", "ext/cast_ext.c", "ext/cast.h", "ext/extconf.rb", "ext/parser.c", "ext/yylex.re", "doc/index.html", "test/test_node.rb", "test/run.rb", "test/test_c_nodes.rb", "test/test_node_list.rb", "test/test_parse.rb", "test/test_parser.rb", "ext/yylex.c", "lib/cast/c.tab.rb"]
  s.test_files = ["test/run.rb"]
  s.extensions = ["ext/extconf.rb"]
end
