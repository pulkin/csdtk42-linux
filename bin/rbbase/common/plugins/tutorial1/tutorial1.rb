# The real tutorial1 script.
# Here we define the functionnal features of the tutorial1 plugin.

module Tutorial1Module
    @@colors = ["FF3300","FF9900","FFCC00","00CC00","66FFCC","6666CC"]

    # Called when we press the button.        
    def tutorial1ButtonFunction()
        count = @@colors.size()
        puts "You pressed the button!"
        count.times { |i|
            str = "<font color='#%s'>This is print %d! (Check State is: %s)</font>" % [@@colors[i],i,tutorial1GetCheckState().to_s]
            tutorial1Print(str)
            tutorial1SetProgress( (i*1.0/(count-1))*100 )
            tutorial1Tic
            sleep 1.5
        }
    end
end

include Tutorial1Module
