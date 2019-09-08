def bist_test

    $CURRENTCONNECTION.enableEvents(true) 
    $CURRENTCONNECTION.flushEvents() 

	xfbp
	bfbp
	puts "BIST is in progress, please wait! ......"
	sleep(1)
	
	w(0xa1909008,0x01980000)	  
	w(0xa190900c,0x33333400)	
	w(0xa1909000,0x00000001)
	while(1)
		a = r(0xa1909004)   
		if((a&0x1) == 0x1)
		 	sleep(1)
		elsif(a==0)
			puts "BIST phase 1 passed!"
			break
		else
			raise "BIST phase 1 failed!"
		end
	end
	
	w(0xa195ffa0,0x00000001)
	while(1)
		a = r(0xa195ffa0)   
		if((a&0x1) == 0x1)
		 	sleep(1)
		elsif(a==0xc0010000)
			puts "BIST phase 2 passed!"
			break
		else
			raise "BIST phase 2 failed!"
		end
	end
	
	w(0xa1a04010,0x00000001)
	while(1)
		a = r(0xa1a04010)   
		if((a&0x1) == 0x1)
		 	sleep(1)
		elsif(a==0x0)
			puts "BIST phase 3 passed!"
			break
		else
			raise "BIST phase 3 failed!"
		end
	end
 
	w(0xa1a0400c,0x00000001)
	while(1)
		a = r(0xa1a0400c)   
		if((a&0x1) == 0x1)
		 	sleep(1)
		elsif(a==0xc0010000)
			puts "BIST phase 4 passed!"
			break
		else
			raise "BIST phase 4 failed!"
		end
	end
	
	puts "BIST PASSED!"

end