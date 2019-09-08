module ParseProfiler

@@BufferProfileThread = nil;

def stopParseprofiling()
    @@BufferProfileThread.kill if (@@BufferProfileThread)
end

def startParseprofiling(dumpfilename, outfilename, size, curPos)
    CombineAndParseProfileData(outfilename, dumpfilename, size, curPos)
end

def CombineAndParseProfileData(outFile, profile1, size1, curPos1, profile2=nil, size2=nil, curPos2=nil)
#    @@TraceprofilerThread = Thread.new {
        sizeInBytes1 = size1*4
        curPosInBytes1 = curPos1*4

        file1 = File.open(profile1, "rb")
        file1_data  = file1.read
        file1.close()
        file1_size = File.size(profile1)

        puts "1 - File Size = %d" % file1_size
        puts "1 - Buffer Size = %d" % sizeInBytes1
        puts "1 - Buffer Position = %d" % curPosInBytes1

        if (file1_size < sizeInBytes1 || sizeInBytes1 < 4)
            puts "File size or buffer size error: %s" % profile1
            return
        end
        if (sizeInBytes1 <= curPosInBytes1)
            puts "Buffer position error: %s" % profile1
            return
        end

        # Check if the profile buffer has some null data (it is not full yet)
        if (file1_data[curPosInBytes1] == 0 && file1_data[curPosInBytes1+1] == 0 &&
            file1_data[curPosInBytes1+2] == 0 && file1_data[curPosInBytes1+3] == 0)
            sizeInBytes1 = curPosInBytes1
            curPosInBytes1 = 0
        end

        buffer1 = Array.new(sizeInBytes1)
        i = 0
        while (i < (sizeInBytes1 - curPosInBytes1))
        	buffer1[i] = file1_data[curPosInBytes1+i]
        	i += 1
        end
        i = 0
        while (i < curPosInBytes1)
        	buffer1[sizeInBytes1 - curPosInBytes1 + i] = file1_data[i]
        	i += 1
        end
        totalSize = sizeInBytes1

        if (profile2 != nil && size2 != nil && curPos2 != nil)
            sizeInBytes2 = size2*4
            curPosInBytes2 = curPos2*4

            file2 = File.open(profile2, "rb")
            file2_data  = file2.read
            file2.close()
            file2_size = File.size(profile2)

            puts "2 - File Size = %d" % file2_size
            puts "2 - Buffer Size = %d" % sizeInBytes2
            puts "2 - Buffer Position = %d" % curPosInBytes2

            if (file2_size < sizeInBytes2 || sizeInBytes2 < 4)
                puts "File size or buffer size error: %s" % profile2
                return
            end
            if (sizeInBytes2 <= curPosInBytes2)
                puts "Buffer position error: %s" % profile2
                return
            end

            # Check if the profile buffer has some null data (it is not full yet)
            if (file2_data[curPosInBytes2] == 0 && file2_data[curPosInBytes2+1] == 0 &&
                file2_data[curPosInBytes2+2] == 0 && file2_data[curPosInBytes2+3] == 0)
                sizeInBytes2 = curPosInBytes2
                curPosInBytes2 = 0
            end

            buffer2 = Array.new(sizeInBytes2)
            i = 0
            while (i < (sizeInBytes2 - curPosInBytes2))
            	buffer2[i] = file2_data[curPosInBytes2+i]
            	i += 1
            end
            i = 0
            while (i < curPosInBytes2)
            	buffer2[sizeInBytes2 - curPosInBytes2 + i] = file2_data[i]
            	i += 1
            end
            totalSize += sizeInBytes2
        end

        g_pxts_buffer = Array.new(totalSize)

        if (profile2 != nil && size2 != nil && curPos2 != nil)
            i1 = sizeInBytes1 - 4
            i2 = sizeInBytes2 - 4
            count = totalSize - 4
            while (true)
                copyFromBuffer1 = false
                if (i1 >= 0 && i2 >= 0)
                    stamp1 = (buffer1[i1+3]<<8) | buffer1[i1+2]
                    stamp2 = (buffer2[i2+3]<<8) | buffer2[i2+2]
                    # Stamp is a 16-bit value. Also check wrap issue.
                    if (stamp1 < stamp2 && (stamp2 - stamp1) < ((1<<16) - 1) / 2) ||
                       (stamp1 > stamp2 && (stamp1 - stamp2) > ((1<<16) - 1) / 2)
                        copyFromBuffer1 = false
                    else
                        copyFromBuffer1 = true
                    end
                elsif (i1 >= 0)
                    copyFromBuffer1 = true
                elsif (i2 >= 0)
                    copyFromBuffer1 = false
                else
                    break
                end

                if (copyFromBuffer1)
                    g_pxts_buffer[count] = buffer1[i1]
                    g_pxts_buffer[count+1] = buffer1[i1+1]
                    g_pxts_buffer[count+2] = buffer1[i1+2]
                    g_pxts_buffer[count+3] = buffer1[i1+3]
                    i1 -= 4
                else
                    g_pxts_buffer[count] = buffer2[i2]
                    g_pxts_buffer[count+1] = buffer2[i2+1]
                    g_pxts_buffer[count+2] = buffer2[i2+2]
                    g_pxts_buffer[count+3] = buffer2[i2+3]
                    i2 -= 4
                end
                count -= 4
            end
=begin
            combined_bin_filename = outFile[0, outFile.length-4]
            combined_bin_filename = combined_bin_filename + "_cmb.bin"
            combinedBinFile = File.open(combined_bin_filename, "wb")
            combinedBinFile << g_pxts_buffer.pack("c*")
            combinedBinFile.close()
=end
        else
            i1 = 0
            while (i1 < sizeInBytes1)
                g_pxts_buffer[i1] = buffer1[i1]
                i1 += 1
            end
        end

        myfile=File.open(outFile, "wb")
        myfile << "DBGP"
        myfile << [0,1,0,1].pack("c*")
        puts "Begin parsing memory profile ..."

        firstTs = nil
        lastpfTsWrap = 0
        count = 0
        cycCount = 0 
        pre_count = 0

        begin
            data = Array.new(4)

            while (count < totalSize) do
                cur_count =  g_pxts_buffer[count+3]<< 8 | g_pxts_buffer[count+2]

                # puts "@%12d: %04x" % [ts,tag]

                if(cur_count < pre_count) then
            	    cycCount+=1
                end

                ts = cur_count + 65536*cycCount
                if ( firstTs.nil? ) then
                    firstTs = ts;
                end
                relTs=ts-firstTs;

                # trace timestamp is based on baseband uptime @16384Hz on 32bits
                # profiling format is defined with 1Mhz timespamp on 13 bits
                pfTs=relTs*1000000/16384

                while ((pfTs - lastpfTsWrap) > (1<<13)) do
                    myfile << [0xff,0xff,0xff,0x7f].pack("c*")
                    lastpfTsWrap += (1<<13)
                end
                pfts = (pfTs - lastpfTsWrap)

                data[2] = pfts & 0xff
                data[3] = (pfts >> 8) & 0xff
                data[0] = g_pxts_buffer[count+0]
                data[1] = g_pxts_buffer[count+1]

                myfile << data[0..4].pack("c*")

                pre_count = cur_count;
                count = count + 4
            end
        ensure
            myfile << [0xff,0xff,0xfe,0x7f].pack("c*")
            myfile.close()

            puts "count = %d" % count
            puts "Parse done: %s\n" % outFile
        end
#    }
end

end
