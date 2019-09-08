

def getLocalIp
    orig, Socket.do_not_reverse_lookup = Socket.do_not_reverse_lookup, true  # turn off reverse DNS resolution temporarily

    UDPSocket.open do |s|
        s.connect '64.233.187.99', 1
        s.addr.last
    end

ensure
    Socket.do_not_reverse_lookup = orig
end

def logAhbBug(stringToLog)
    begin
        Net::HTTP.post_form(URI.parse('http://10.50.0.5/greenstoneahbmasterlogger.php'), { 'stringToLog'=>stringToLog }) 
    rescue Exception
    end
end