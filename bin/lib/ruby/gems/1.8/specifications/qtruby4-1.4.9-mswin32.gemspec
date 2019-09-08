Gem::Specification.new do |s|
  s.name = %q{qtruby4}
  s.version = "1.4.9"
  s.date = %q{2007-06-20}
  s.summary = %q{Provides Qt bindings for Ruby programs.}
  s.homepage = %q{http://rubyforge.org/projects/korundum/}
  s.rubyforge_project = %q{qtruby}
  s.autorequire = %q{qt/qtruby4}
  s.required_ruby_version = Gem::Version::Requirement.new(">= 1.8.1")
  s.platform = %q{mswin32}
  s.files = ["lib/Makefile", "lib/qt", "lib/Qt.rb", "lib/Qt3.rb", "lib/Qt4.rb", "lib/qtruby4.so", "bin/assistant.exe", "bin/designer.exe", "bin/libdbus-1.dll", "bin/linguist.exe", "bin/qdbus.exe", "bin/qdbuscpp2xml.exe", "bin/qdbusviewer.exe", "bin/qdbusxml2cpp.exe", "bin/Qt3Support4.dll", "bin/QtAssistantClient4.dll", "bin/QtCore4.dll", "bin/QtDBus4.dll", "bin/QtDesigner4.dll", "bin/QtDesignerComponents4.dll", "bin/QtGui4.dll", "bin/QtNetwork4.dll", "bin/QtOpenGL4.dll", "bin/QtScript4.dll", "bin/QtSql4.dll", "bin/QtSvg4.dll", "bin/QtTest4.dll", "bin/QtXml4.dll", "bin/rbqtapi", "bin/rbqtsh", "bin/rbrcc.exe", "bin/rbuic4.exe", "bin/smokeqt.dll", "lib/qt/qtruby4.rb", "extconf.rb"]
  s.extensions = ["extconf.rb"]
end
