#!/usr/bin/env ruby
include CoolHost
require 'help.rb'

addHelpEntry("chip", "sver", "", "", "Prints the version information of the software modules.")
def sver()
    
    puts "Please wait, reading info..."
    
    dark = true
    color = (dark)?("#d0d0d0"):("#f0f0f0")
    str = ("html><table><tr bgcolor=%s>" % color) + 
            "<td><font color=red><b>Name</b></font></td>" +
            "<td><font color=red><b>Revision</b></font></td>" +
            "<td><font color=red><b>Number</b></font></td>" +
            "<td><font color=red><b>Date</b></font></td>" +
            "<td><font color=red><b>String</b></font></td>" +
            "</tr>" 

    $map_table.>.members.each { |e| 
         
        begin
            e.derefAt(4).revision
        rescue NoMethodError
            next
        end

        dark = !dark
        color = (dark)?("#d0d0d0"):("#f0f0f0")
        
        str +=  ("<tr bgcolor=%s><td><b>" % color) + e.to_s + "</b></td>"
        
        begin
            content = e.>
        rescue NullPointer
            str += "<td>--</td><td>--</td><td>--</td><td>--</td></tr>"
            next
        end
        
        str += "<td>%x</td>" % content.revision.R    
        str += "<td>0x%X</td>" % content.number.R  
        datestr = "%d" % content.date.R  
        str += "<td><font color=blue>%s/%s/%s</font></td>" % [datestr[0..3],datestr[4..5],datestr[6..7]] 
        str += "<td><font color=darkgreen>%s</font></td>" % content.string.R().RS(128)
        str += "</tr>"
    }
    
    puts str
    
end
