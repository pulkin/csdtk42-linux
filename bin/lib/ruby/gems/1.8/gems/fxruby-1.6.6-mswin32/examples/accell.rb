require 'fox16'

include Fox

class MyWindow < FXMainWindow

  include Responder

  ID_SAVE = FXMainWindow::ID_LAST

  def initialize(app)

    super(app, "Window", nil, nil, DECOR_ALL, 0, 0, 200, 100)

    FXMAPFUNC(SEL_COMMAND, MyWindow::ID_SAVE, 'onSave')
    accelTable.addAccel(fxparseAccel("Ctrl-S"), self, FXSEL(SEL_COMMAND, MyWindow::ID_SAVE))

  end

  def onSave(sender, selector, data)
    puts "save"
  end

  def create
    super
    show(PLACEMENT_SCREEN)
  end

end

if __FILE__ == $0
  application = FXApp.new("Fox", "FXRuby Test")
  MyWindow.new(application)
  application.create
  application.run
end
