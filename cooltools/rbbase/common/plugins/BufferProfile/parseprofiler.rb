module ParseProfiler

def startParseProfiling(dumpfilename, outfilename, size, curPos, freq=16384)
    buffer = parseProfilingData(dumpfilename, size, curPos, freq)
    writeParsedProfiling(outfilename, buffer)
    return buffer
end

def parseProfilingData(profile1, size1, curPos1, freq=16384)
    file1 = File.open(profile1, "rb")
    file1_data  = file1.read()
    file1.close()
    file1_size = File.size(profile1)

    if (size1 == nil || curPos1 == nil)
        return Array.new(0)
    end

    sizeInBytes1 = size1*4
    curPosInBytes1 = curPos1*4

    puts "1 - File Size = %d" % file1_size
    puts "1 - Buffer Size = %d" % sizeInBytes1
    puts "1 - Buffer Position = %d" % curPosInBytes1
    puts "1 - Buffer freq = %d" % freq

    if (file1_size < sizeInBytes1 || sizeInBytes1 < 4)
        puts "File size or buffer size error: %s" % profile1
        return Array.new(0)
    end
    if (sizeInBytes1 <= curPosInBytes1)
        puts "Buffer position error: %s" % profile1
        return Array.new(0)
    end

    # Check if the profile buffer has some null data (it is not full yet)
    if (file1_data[curPosInBytes1] == 0 && file1_data[curPosInBytes1+1] == 0 &&
        file1_data[curPosInBytes1+2] == 0 && file1_data[curPosInBytes1+3] == 0)
        sizeInBytes1 = curPosInBytes1
        curPosInBytes1 = 0
    end
    puts "1 - Buffer Count = %d" % (sizeInBytes1/4)

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

    return buffer1
end

def combineProfileData(buffer1, buffer2, freq=16384)
    sizeInBytes1 = buffer1.length
    sizeInBytes2 = buffer2.length
    if (sizeInBytes1 == 0)
        return buffer2
    end
    if (sizeInBytes2 == 0)
        return buffer1
    end

    totalSize = sizeInBytes1 + sizeInBytes2
    g_pxts_buffer = Array.new(totalSize)

    i1 = sizeInBytes1 - 4
    i2 = sizeInBytes2 - 4
    count = totalSize - 4

    # determine the initial cycle count of 1 and 2
    pstamp1 = (buffer1[i1+3]<<8) | buffer1[i1+2]
    pstamp2 = (buffer2[i2+3]<<8) | buffer2[i2+2]
    delta0 = (pstamp1 - pstamp2).abs()
    deltap = (pstamp1 - (pstamp2 + 0x10000)).abs()
    deltan = (pstamp1 - (pstamp2 - 0x10000)).abs()
    cycCount1 = 0
    if delta0 < deltap && delta0 < deltan
        cycCount2 = 0
    elsif deltap < delta0 && deltap < deltan
        cycCount2 = 1
    else
        cycCount2 = -1
    end

    while (true)
        if (i1 >= 0 && i2 >= 0)
            # cal ts and update cycle count of each
            stamp1 = (buffer1[i1+3]<<8) | buffer1[i1+2]
            stamp2 = (buffer2[i2+3]<<8) | buffer2[i2+2]
            if stamp1 > pstamp1
                cycCount1 -= 1
            end
            if stamp2 > pstamp2
                cycCount2 -= 1
            end
            ts1 = stamp1 + (0x10000 * cycCount1)
            ts2 = stamp2 + (0x10000 * cycCount2)
            pstamp1 = stamp1
            pstamp2 = stamp2

            if (ts1 > ts2)
                copyFromBuffer1 = true
            else
                copyFromBuffer1 = false
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
    return g_pxts_buffer
end

def writeParsedProfiling(outfilename, buffer, freq=16384)
    myfile=File.open(outfilename, "wb")
    myfile << "DBGP"
    myfile << [0,1,0,1].pack("c*")

    firstTs = nil
    lastpfTsWrap = 0
    count = 0
    cycCount = 0 
    pre_count = 0
    totalSize = buffer.length

    begin
        data = Array.new(4)

        while (count < totalSize) do
            cur_count =  buffer[count+3]<< 8 | buffer[count+2]

            if(cur_count < pre_count) then
                cycCount+=1
            end

            ts = cur_count + 65536*cycCount
            if ( firstTs.nil? ) then
                firstTs = ts;
            end
            relTs=ts-firstTs;

            # for xcpu and bcpu trace timestamp is based on baseband uptime @16384Hz on 32bits
            # for wcput the timestamp maybe is @16384 or chip8(30720000Hz)
            # profiling format is defined with 1Mhz timespamp on 13 bits
            pfTs=relTs*1000000/freq

            while ((pfTs - lastpfTsWrap) >= (1<<13)) do
                myfile << [0xff,0xff,0xff,0x7f].pack("c*")
                lastpfTsWrap += (1<<13)
            end
            pfts = (pfTs - lastpfTsWrap)

            data[2] = pfts & 0xff
            data[3] = (pfts >> 8) & 0xff
            data[0] = buffer[count]
            data[1] = buffer[count+1]

            myfile << data[0..4].pack("c*")

            pre_count = cur_count;
            count = count + 4
        end
    ensure
        myfile << [0xff,0xff,0xfe,0x7f].pack("c*")
        myfile.close()
        puts "count = %d" % count
        puts "Parse done: %s\n" % outfilename
    end
end

end
