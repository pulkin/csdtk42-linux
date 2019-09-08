require 'windows/error'
require 'windows/eventlog'
require 'windows/security'
require 'windows/registry'
require 'windows/system_info'
require 'windows/library'
require 'windows/synchronize'
require 'windows/handle'

class String
   # Return the portion of the string up to the first NULL character.  This
   # was added for both speed and convenience.
   def nstrip
      self[ /^[^\0]*/ ]
   end
end

module Win32
   class EventLogError < StandardError; end
   class EventLog
      include Windows::Error
      include Windows::EventLog
      include Windows::Security
      include Windows::Registry
      include Windows::SystemInfo
      include Windows::Library
      include Windows::Synchronize
      include Windows::Handle
      extend Windows::Error
      extend Windows::Registry
      
      VERSION = '0.4.3'
      
      # Aliased read flags
      FORWARDS_READ   = EVENTLOG_FORWARDS_READ
      BACKWARDS_READ  = EVENTLOG_BACKWARDS_READ
      SEEK_READ       = EVENTLOG_SEEK_READ
      SEQUENTIAL_READ = EVENTLOG_SEQUENTIAL_READ
      
      # Aliased event types
      SUCCESS         = EVENTLOG_SUCCESS
      ERROR           = EVENTLOG_ERROR_TYPE
      WARN            = EVENTLOG_WARNING_TYPE
      INFO            = EVENTLOG_INFORMATION_TYPE
      AUDIT_SUCCESS   = EVENTLOG_AUDIT_SUCCESS
      AUDIT_FAILURE   = EVENTLOG_AUDIT_FAILURE
      
      # These are not meant for public use
      BUFFER_SIZE = 1024 * 64
      MAX_SIZE    = 256
      MAX_STRINGS = 16
      BASE_KEY    = "SYSTEM\\CurrentControlSet\\Services\\EventLog\\"
      
      EventLogStruct = Struct.new('EventLogStruct', :record_number,
         :time_generated, :time_written, :event_id, :event_type, :category,
         :source, :computer, :user, :string_inserts, :description
      )
      
      # The name of the event log source.  This will typically be
      # 'Application', 'System' or 'Security', but could also refer to
      # a custom event log source.
      # 
      attr_reader :source
      
      # The name of the server which the event log is reading from.
      # 
      attr_reader :server
      
      # The name of the file used in the EventLog.open_backup method.  This is
      # set to nil if the file was not opened using the EventLog.open_backup
      # method.
      # 
      attr_reader :file
      
      # Opens a handle to the new EventLog +source+ on +server+, or the local
      # machine if no host is specified.  Typically, your source will be
      # 'Application, 'Security' or 'System', although you can specify a
      # custom log file as well.
      # 
      # If a custom, registered log file name cannot be found, the event
      # logging service opens the 'Application' log file.  This is the
      # behavior of the underlying Windows function, not my own doing.
      # 
      def initialize(source = 'Application', server = nil, file = nil)
         @source = source || 'Application' # In case of explicit nil
         @server = server
         @file   = file
         
         # Avoid Win32API segfaults
         raise TypeError unless @source.is_a?(String)
         raise TypeError unless @server.is_a?(String) if @server
         
         function = 'OpenEventLog()'
         
         if @file.nil?
            @handle = OpenEventLog(@server, @source)
         else
            @handle = OpenBackupEventLog(@server, @file)
            function = 'OpenBackupEventLog()'
         end
         
         if @handle == 0
            error = "#{function} failed: " + get_last_error
            raise EventLogError, error
         end
         
         # Ensure the handle is closed at the end of a block
         if block_given?
            begin
               yield self
            ensure
               close
            end
         end
      end
      
      # Class method aliases
      class << self
         alias :open :new
      end
      
      # Nearly identical to EventLog.open, except that the source is a backup
      # file and not an event source (and there is no default).
      # 
      def self.open_backup(file, source = 'Application', server = nil)
         @file   = file
         @source = source
         @server = server
         
         # Avoid Win32API segfaults
         raise TypeError unless @file.is_a?(String)
         raise TypeError unless @source.is_a?(String)
         raise TypeError unless @server.is_a?(String) if @server        
				
         self.new(source, server, file)         
      end
      
      # Adds an event source to the registry.  The following are valid keys:
      #   
      # * source                - Source name.  Set to "Application" by default
      # * key_name              - Name stored as the registry key
      # * category_count        - Number of supported (custom) categories
      # * event_message_file    - File (dll) that defines events
      # * category_message_file - File (dll) that defines categories
      # * supported_types       - See the 'event types' constants
      #
      # Of these keys, only +key_name+ is mandatory.  An ArgumentError is
      # raised if you attempt to use an invalid key.  If +supported_types+
      # is not specified then the following value is used:
      #
      # EventLog::ERROR | EventLog::WARN | EventLog::INFO
      #
      # The +event_message_file+ and +category_message_file+ are typically,
      # though not necessarily, the same file.  See the documentation on .mc files
      # for more details.
      #     
      def self.add_event_source(args)
         raise TypeError unless args.is_a?(Hash)
         
         hkey = [0].pack('L')
         
         valid_keys = %w/source key_name category_count event_message_file
            category_message_file supported_types/

         key_base = "SYSTEM\\CurrentControlSet\\Services\\EventLog\\"

         # Default values
         hash = {
            'source'          => 'Application',
            'supported_types' => ERROR | WARN | INFO
         }
         
         # Validate the keys, and convert symbols and case to lowercase strings.     
         args.each{ |key, val|
            key = key.to_s.downcase
            unless valid_keys.include?(key)
               raise ArgumentError, "invalid key '#{key}'"
            end
            hash[key] = val
         }
         
         # The key_name must be specified
         unless hash['key_name']
            raise EventLogError, 'no event_type specified'
         end
         
         key = key_base << hash['source'] << "\\" << hash['key_name']
         
         if RegCreateKey(HKEY_LOCAL_MACHINE, key, hkey) != ERROR_SUCCESS
            error = 'RegCreateKey() failed: ' + get_last_error
            raise EventLogError, error
         end
         
         hkey = hkey.unpack('L').first
         
         if hash['category_count']
            data = [hash['category_count']].pack('L')
            
            rv = RegSetValueEx(
               hkey,
               'CategoryCount',
               0,
               REG_DWORD,
               data,
               data.size
            )
            
            if rv != ERROR_SUCCESS
               error = 'RegSetValueEx() failed: ', get_last_error
               RegCloseKey(hkey)
               raise EventLogError, error
            end
         end
         
         if hash['category_message_file']
            data = File.expand_path(hash['category_message_file'])
            
            rv = RegSetValueEx(
               hkey,
               'CategoryMessageFile',
               0,
               REG_EXPAND_SZ,
               data,
               data.size
            )
            
            if rv != ERROR_SUCCESS
               error = 'RegSetValueEx() failed: ', get_last_error
               RegCloseKey(hkey)
               raise EventLogError, error
            end
         end
         
         if hash['event_message_file']
            data = File.expand_path(hash['event_message_file'])
            
            rv = RegSetValueEx(
               hkey,
               'EventMessageFile',
               0,
               REG_EXPAND_SZ,
               data,
               data.size
            )
            
            if rv != ERROR_SUCCESS
               error = 'RegSetValueEx() failed: ', get_last_error
               RegCloseKey(hkey)
               raise EventLogError, error
            end
         end
         
         data = [hash['supported_types']].pack('L')
         rv = RegSetValueEx(
            hkey,
            'TypesSupported',
            0,
            REG_DWORD,
            data,
            data.size
         )
            
         if rv != ERROR_SUCCESS
            error = 'RegSetValueEx() failed: ', get_last_error
            RegCloseKey(hkey)
            raise EventLogError, error
         end
         
         RegCloseKey(hkey)
         self
      end
      
      # Backs up the event log to +file+.  Note that you cannot backup to
      # a file that already exists or a EventLogError will be raised.
      # 
      def backup(file)
         raise TypeError unless file.is_a?(String)
         unless BackupEventLog(@handle, file)
            error = 'BackupEventLog() failed: ' + get_last_error
            raise EventLogError, error
         end
         self
      end
      
      # Clears the EventLog.  If +backup_file+ is provided, it backs up the
      # event log to that file first.
      # 
      def clear(backup_file = nil)
         raise TypeError unless backup_file.is_a?(String) if backup_file
         backup_file = 0 unless backup_file
          
         unless ClearEventLog(@handle, backup_file)
            error = 'ClearEventLog() failed: ' + get_last_error
            raise EventLogError
         end
         
         self
      end
      
      # Closes the EventLog handle.  Automatically closed for you if you use
      # the block form of EventLog.new.
      # 
      def close
         CloseEventLog(@handle)
      end
      
      # Indicates whether or not the event log is full.
      # 
      def full?
         buf    = [0].pack('L') # dwFull
         needed = [0].pack('L')
         
         unless GetEventLogInformation(@handle, 0, buf, buf.size, needed)
            raise 'GetEventLogInformation() failed: ' + get_last_error
         end

         buf[0,4].unpack('L').first != 0
      end
      
      # Returns the absolute record number of the oldest record.  Note that
      # this is not guaranteed to be 1 because event log records can be
      # overwritten.
      # 
      def oldest_record_number
         rec = [0].pack('L')
         
         unless GetOldestEventLogRecord(@handle, rec)
            error = 'GetOldestEventLogRecord() failed: ' + get_last_error
            raise EventLogError, error
         end
         
         rec.unpack('L').first
      end
      
      # Returns the total number of records for the given event log.
      # 
      def total_records
         total = [0].pack('L')
         
         unless GetNumberOfEventLogRecords(@handle, total)
            error = 'GetNumberOfEventLogRecords() failed: '
            error += get_last_error
            raise EventLogError, error
         end
         
         total.unpack('L').first
      end
      
      # Yields an EventLogStruct every time a record is written to the event
      # log. Unlike EventLog#tail, this method breaks out of the block after
      # the event.
      # 
      # Raises an EventLogError if no block is provided.
      # 
      def notify_change(&block)
         unless block_given?
            raise EventLogError, 'block missing for notify_change()'
         end
         
         # Reopen the handle because the NotifyChangeEventLog() function will
         # choke after five or six reads otherwise.
         @handle = OpenEventLog(@server, @source)
         
         if @handle == 0
            error = "OpenEventLog() failed: " + get_last_error
            raise EventLogError, error
         end
         
         event = CreateEvent(0, 0, 0, 0)
         
         unless NotifyChangeEventLog(@handle, event)
            error = 'NotifyChangeEventLog() failed: ' + get_last_error
            raise EventLogError, error
         end
         
         wait_result = WaitForSingleObject(event, INFINITE)
         
         if wait_result == WAIT_FAILED
            error = 'WaitForSingleObject() failed: ' + get_last_error
            CloseHandle(event)
            raise EventLogError, error
         else
            last = read_last_event
            block.call(last)
         end
   
         CloseHandle(event)
         self
      end
      
      # Yields an EventLogStruct every time a record is written to the event
      # log, once every +frequency+ seconds. Unlike EventLog#notify_change,
      # this method does not break out of the block after the event.  The read
      # +frequency+ is set to 5 seconds by default.
      # 
      # Raises an EventLogError if no block is provided.
      # 
      # The delay between reads is due to the nature of the Windows event log.
      # It is not really designed to be tailed in the manner of a Unix syslog,
      # for example, in that not nearly as many events are typically recorded. 
      # It's just not designed to be polled that heavily.
      #
      def tail(frequency = 5)
         unless block_given?
            raise EventLogError, 'block missing for tail()'
         end
         
         old_total = total_records()       
         flags     = FORWARDS_READ | SEEK_READ
         rec_num   = read_last_event.record_number
         
         while true          
            new_total = total_records()    
            if new_total != old_total
               rec_num = oldest_record_number() if full?
               read(flags, rec_num).each{ |log| yield log }
               old_total = new_total
               rec_num   = read_last_event.record_number + 1
            end
            sleep frequency
         end
      end
      
      # EventLog#read(flags=nil, offset=0)
      # EventLog#read(flags=nil, offset=0){ |log| ... }
      # 
      # Iterates over each record in the event log, yielding a EventLogStruct
      # for each record.  The offset value is only used when used in
      # conjunction with the EventLog::SEEK_READ flag.  Otherwise, it is
      # ignored.  If no flags are specified, then the default flags are:
      #   
      # EventLog::SEQUENTIAL_READ | EventLog::FORWARDS_READ
      #
      # Note that, if you're performing a SEEK_READ, then the offset must
      # refer to a record number that actually exists.  The default of 0
      # may or may not work for your particular event log.
      #   
      # The EventLogStruct struct contains the following members:
      #   
      # record_number  # Fixnum
      # time_generated # Time
      # time_written   # Time
      # event_id       # Fixnum
      # event_type     # String
      # category       # String
      # source         # String
      # computer       # String
      # user           # String or nil
      # description    # String or nil
      # string_inserts # An array of Strings or nil
      #   
      # If no block is given the method returns an array of EventLogStruct's.
      # 
      def read(flags = nil, offset = 0)
         buf    = 0.chr * BUFFER_SIZE # 64k buffer 
         size   = buf.size      
         read   = [0].pack('L')
         needed = [0].pack('L')
         array  = []
         
         unless flags
            flags = FORWARDS_READ | SEQUENTIAL_READ
         end
         
         while ReadEventLog(@handle, flags, offset, buf, size, read, needed) ||
            GetLastError() == ERROR_INSUFFICIENT_BUFFER
            
            if GetLastError() == ERROR_INSUFFICIENT_BUFFER
               buf += 0.chr * needed.unpack('L').first
               ReadEventLog(@handle, flags, offset, buf, size, read, needed)
            end
                       
            dwread = read.unpack('L').first
               
            while dwread > 0
               struct       = EventLogStruct.new
               event_source = buf[56..-1].nstrip
               computer     = buf[56 + event_source.length + 1..-1].nstrip
               
               user = get_user(buf)
               strings, desc = get_description(buf, event_source)
      
               struct.source         = event_source
               struct.computer       = computer
               struct.record_number  = buf[8,4].unpack('L').first
               struct.time_generated = Time.at(buf[12,4].unpack('L').first)
               struct.time_written   = Time.at(buf[16,4].unpack('L').first)
               struct.event_id       = buf[20,4].unpack('L').first & 0x0000FFFF
               struct.event_type     = get_event_type(buf[24,2].unpack('S').first)
               struct.user           = user
               struct.category       = buf[28,2].unpack('S').first
               struct.string_inserts = strings
               struct.description 	 = desc

               if block_given?
                  yield struct
               else
                  array.push(struct)
               end
               
               if flags & EVENTLOG_BACKWARDS_READ > 0
                  offset = buf[8,4].unpack('L').first - 1
               else
                  offset = buf[8,4].unpack('L').first + 1
               end
               
               length = buf[0,4].unpack('L').first # Length

               dwread -= length
               buf = buf[length..-1]
            end
            
            buf = 0.chr * BUFFER_SIZE
            read = [0].pack('L')
         end
         
         block_given? ? nil : array
      end
      
      # This class method is nearly identical to the EventLog#read instance
      # method, except that it takes a +source+ and +server+ as the first two
      # arguments.
      # 
      def self.read(source='Application', server=nil, flags=nil, offset=0)
         self.new(source, server){ |log|
            if block_given?
               log.read(flags, offset){ |els| yield els }
            else
               return log.read(flags, offset)
            end
         }
      end
     
      # EventLog#report_event(key => value, ...)
      #
      # Writes an event to the event log.  The following are valid keys:
      #  
      # source         # Event log source name. Defaults to "Application"
      # event_id       # Event ID (defined in event message file)
      # category       # Event category (defined in category message file)
      # data           # String that is written to the log
      # event_type     # Type of event, e.g. EventLog::ERROR, etc.
      #   
      # The +event_type+ keyword is the only mandatory keyword.  The others are
      # optional.  Although the +source+ defaults to "Application", I
      # recommend that you create an application specific event source and use
      # that instead.  See the 'EventLog.add_event_source' method for more
      # details.
      #  
      # The +event_id+ and +category+ values are defined in the message
      # file(s) that you created for your application.  See the tutorial.txt
      # file for more details on how to create a message file.
      #   
      # An ArgumentError is raised if you attempt to use an invalid key.
      # 
      def report_event(args)
         raise TypeError unless args.is_a?(Hash)
         
         valid_keys  = %w/source event_id category data event_type/
         num_strings = 0

         # Default values
         hash = {
            'source'   => @source,
            'event_id' => 0,
            'category' => 0,
            'data'     => 0
         }
         
         # Validate the keys, and convert symbols and case to lowercase strings.     
         args.each{ |key, val|
            key = key.to_s.downcase
            unless valid_keys.include?(key)
               raise ArgumentError, "invalid key '#{key}'"
            end
            hash[key] = val
         }
         
         # The event_type must be specified
         unless hash['event_type']
            raise EventLogError, 'no event_type specified'
         end
         
         handle = RegisterEventSource(@server, hash['source'])
         
         if handle == 0
            error = 'RegisterEventSource() failed: ' + get_last_error
            raise EventLogError, error
         end
         
         if hash['data'].is_a?(String)
            data = hash['data'] << 0.chr
            data = [data].pack('p*')
            num_strings = 1
         else
            data = 0
            num_strings = 0
         end
         
         bool = ReportEvent(
            handle,
            hash['event_type'],
            hash['category'],
            hash['event_id'],
            0,
            num_strings,
            0,
            data,
            0
         )
         
         unless bool
            error = 'ReportEvent() failed: ' + get_last_error
            raise EventLogError, error
         end
         
         self
      end
      
      alias :write :report_event
      
      private
      
      # A private method that reads the last event log record.
      # 
      def read_last_event(handle=@handle, source=@source, server=@server)
         buf    = 0.chr * BUFFER_SIZE # 64k buffer
         read   = [0].pack('L')
         needed = [0].pack('L')
    
         flags = EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ
         ReadEventLog(@handle, flags, 0, buf, buf.size, read, needed)
      
         event_source = buf[56..-1].nstrip
         computer     = buf[56 + event_source.length + 1..-1].nstrip
         event_type   = get_event_type(buf[24,2].unpack('S').first)
         user         = get_user(buf)
         desc         = get_description(buf, event_source)
      
         struct = EventLogStruct.new
         struct.source         = event_source
         struct.computer       = computer
         struct.record_number  = buf[8,4].unpack('L').first
         struct.time_generated = Time.at(buf[12,4].unpack('L').first)
         struct.time_written   = Time.at(buf[16,4].unpack('L').first)
         struct.event_id       = buf[20,4].unpack('L').first & 0x0000FFFF
         struct.event_type     = event_type
         struct.user           = user
         struct.category       = buf[28,2].unpack('S').first
         struct.description 	 = desc
         
         struct
      end
      
      # Private method that gets the string inserts (Array) and the full
      # event description (String) based on data from the EVENTLOGRECORD
      # buffer.
      # 
      def get_description(rec, event_source)    
         str     = rec[rec[36,4].unpack('L').first .. -1]         
         num     = rec[26,2].unpack('S').first # NumStrings
         hkey    = [0].pack('L')
         key     = BASE_KEY + "#{@source}\\#{event_source}"
         buf     = 0.chr * 1024
         va_list = nil
         
         if num == 0
            va_list_ptr = 0.chr * 4
         else
	         va_list = str.split(0.chr)[0...num]
	         va_list_ptr = va_list.map{ |x|
	            [x + 0.chr].pack('P').unpack('L').first
	         }.pack('L*')
         end
         	         	
         if RegOpenKeyEx(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, hkey) == 0
            value = 'EventMessageFile'
            file  = 0.chr * MAX_SIZE
            hkey  = hkey.unpack('L').first
            size  = [file.length].pack('L')
            
            if RegQueryValueEx(hkey, value, 0, 0, file, size) == 0
               file = file.nstrip
               exe  = 0.chr * MAX_SIZE
               
               ExpandEnvironmentStrings(file, exe, exe.size)
               exe = exe.nstrip
               
               exe.split(';').each{ |file|
                  hmodule  = LoadLibraryEx(file, 0, LOAD_LIBRARY_AS_DATAFILE)
                  event_id = rec[20,4].unpack('L').first
                  if hmodule != 0
                     FormatMessage(
                        FORMAT_MESSAGE_FROM_HMODULE |
                        FORMAT_MESSAGE_ARGUMENT_ARRAY,
                        hmodule,
                        event_id,
                        0,
                        buf,
                        buf.size,
                        va_list_ptr 
                     )
                     
                     FreeLibrary(hmodule)
                  end
               }
            end
            
            RegCloseKey(hkey)
         end
         [va_list, buf.strip]
      end
      
      # Private method that retrieves the user name based on data in the
      # EVENTLOGRECORD buffer.
      # 
      def get_user(buf)      
         return nil if buf[40,4].unpack('L').first <= 0 # UserSidLength

         name        = 0.chr * MAX_SIZE
         name_size   = [name.size].pack('L')
         domain      = 0.chr * MAX_SIZE
         domain_size = [domain.size].pack('L')
         snu         = 0.chr * 4
         
         offset = buf[44,4].unpack('L').first # UserSidOffset
      
         val = LookupAccountSid(
            @server,
            [buf].pack('P').unpack('L').first + offset,
            name,
            name_size,
            domain,
            domain_size,
            snu
         )
         
         # Return nil if the lookup failed
         return val ? name.nstrip : nil
      end
      
      # Private method that converts a numeric event type into a human
      # readable string.
      # 
      def get_event_type(event)
         case event
            when EVENTLOG_ERROR_TYPE
               'error'
            when EVENTLOG_WARNING_TYPE
               'warning'
            when EVENTLOG_INFORMATION_TYPE
               'information'
            when EVENTLOG_AUDIT_SUCCESS
               'audit_success'
            when EVENTLOG_AUDIT_FAILURE
               'audit_failure'
            else
               nil
         end
      end
   end
end
