require 'net/smtp'



def csmail(srcuser,dstuser,subject,content,smtp="10.50.0.3")
     
     if(!(srcuser =~ /@/))
        srcuserem = srcuser.downcase + "@coolsand-tech.fr"
     else
        srcuserem = srcuser
     end
      
     if(!(dstuser =~ /@/))
        dstuserem = dstuser.downcase + "@coolsand-tech.fr"
     else
        dstuserem = dstuser
      end
      
      sub = "[CoolRuby] " + subject
      srcuser = srcuser.capitalize
      dstuser = dstuser.capitalize
      
    	msg = <<END_OF_MESSAGE
From: #{srcuser} of CoolSand <#{srcuserem}>
To: #{dstuser} of CoolSand <#{dstuserem}>
Subject: #{sub}
	
#{content}
-----------------------------------
This message was sent to you through CoolRuby.
END_OF_MESSAGE
	
	Net::SMTP.start(smtp) do |smtp|
		smtp.send_message msg, srcuserem, dstuserem
  end
end
