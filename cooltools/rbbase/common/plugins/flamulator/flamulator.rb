include CoolHost

begin
include Flamulator
rescue Exception
puts "*** FLAMULATOR NOT AVAILABLE."
end

begin
	include CoolWatcher
rescue Exception
end

def romulateProgress(burnProgress, verifyProgress)
	begin
		cwSetProgress(burnProgress,100,"%p% (burning)") if(burnProgress!=0)
		cwSetProgress(verifyProgress,100,"%p% (verifying)") if(verifyProgress!=0)		
	rescue Exception
	end
end

#Use 16MB buffer for flamulation.
def flamulate(files, verify=true, fullImage=true, flashSize=16)
    puts "html><b>Flamulator V1</b>"
    files.each { |file|
        puts "Downloading the file:"
        puts "html><i>" + File.basename(file) + "</i>"
    }
    
 	FLAMDownloadFiles(files, verify, fullImage, flashSize) { |burnProgress,verifyProgress|
 		romulateProgress(burnProgress,verifyProgress)
 	}
	cwSetProgress(-1,100)
	puts "Flamulation done."
end

def safulate(files)
	restart(true)
	flamulate(files)
	unfreeze()
end