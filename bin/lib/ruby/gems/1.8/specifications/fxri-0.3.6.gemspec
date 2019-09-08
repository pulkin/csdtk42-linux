Gem::Specification.new do |s|
  s.name = %q{fxri}
  s.version = "0.3.6"
  s.date = %q{2006-12-17}
  s.summary = %q{Graphical interface to the RI documentation, with search engine.}
  s.email = %q{markus.prinz@qsig.org}
  s.homepage = %q{http://rubyforge.org/projects/fxri/}
  s.rubyforge_project = %q{fxri}
  s.description = %q{FxRi is an FXRuby interface to the RI documentation, with a search engine that allows for search-on-typing.}
  s.default_executable = %q{fxri}
  s.bindir = %q{.}
  s.files = ["lib", "fxri", "fxri.gemspec", "fxri-0.3.6.tar.gz", "fxri.rb", "lib/FoxDisplayer.rb", "lib/icons", "lib/Recursive_Open_Struct.rb", "lib/RiManager.rb", "lib/Globals.rb", "lib/fxirb.rb", "lib/Packet_Item.rb", "lib/Search_Engine.rb", "lib/Empty_Text_Field_Handler.rb", "lib/FoxTextFormatter.rb", "lib/Packet_List.rb", "lib/Icon_Loader.rb", "lib/icons/module.png", "lib/icons/method.png", "lib/icons/class.png", "./fxri"]
  s.executables = ["fxri"]
  s.add_dependency(%q<fxruby>, [">= 1.2.0"])
end
