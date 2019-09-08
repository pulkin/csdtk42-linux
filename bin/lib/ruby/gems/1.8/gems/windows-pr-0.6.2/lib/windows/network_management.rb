require 'Win32API'

module Windows
   module NetworkManagement
      NERR_Success         = 0
      MAX_PREFERRED_LENGTH = -1

      # Taken from LMServer.h
      SV_TYPE_WORKSTATION       = 0x00000001
      SV_TYPE_SERVER            = 0x00000002
      SV_TYPE_SQLSERVER         = 0x00000004
      SV_TYPE_DOMAIN_CTRL       = 0x00000008
      SV_TYPE_DOMAIN_BAKCTRL    = 0x00000010
      SV_TYPE_TIME_SOURCE       = 0x00000020
      SV_TYPE_AFP               = 0x00000040
      SV_TYPE_NOVELL            = 0x00000080
      SV_TYPE_DOMAIN_MEMBER     = 0x00000100
      SV_TYPE_PRINTQ_SERVER     = 0x00000200
      SV_TYPE_DIALIN_SERVER     = 0x00000400
      SV_TYPE_XENIX_SERVER      = 0x00000800
      SV_TYPE_SERVER_UNIX       = SV_TYPE_XENIX_SERVER
      SV_TYPE_NT                = 0x00001000
      SV_TYPE_WFW               = 0x00002000
      SV_TYPE_SERVER_MFPN       = 0x00004000
      SV_TYPE_SERVER_NT         = 0x00008000
      SV_TYPE_POTENTIAL_BROWSER = 0x00010000
      SV_TYPE_BACKUP_BROWSER    = 0x00020000
      SV_TYPE_MASTER_BROWSER    = 0x00040000
      SV_TYPE_DOMAIN_MASTER     = 0x00080000
      SV_TYPE_SERVER_OSF        = 0x00100000
      SV_TYPE_SERVER_VMS        = 0x00200000
      SV_TYPE_WINDOWS           = 0x00400000
      SV_TYPE_DFS               = 0x00800000
      SV_TYPE_CLUSTER_NT        = 0x01000000
      SV_TYPE_TERMINALSERVER    = 0x02000000
      SV_TYPE_CLUSTER_VS_NT     = 0x04000000
      SV_TYPE_DCE               = 0x10000000
      SV_TYPE_ALTERNATE_XPORT   = 0x20000000
      SV_TYPE_LOCAL_LIST_ONLY   = 0x40000000
      SV_TYPE_DOMAIN_ENUM       = 0x80000000
      SV_TYPE_ALL               = 0xFFFFFFFF

      NetAlertRaise          = Win32API.new('netapi32', 'NetAlertRaise', 'PPL', 'L')
      NetAlertRaiseEx        = Win32API.new('netapi32', 'NetAlertRaiseEx', 'PPLP', 'L')
      NetApiBufferAllocate   = Win32API.new('netapi32', 'NetApiBufferAllocate', 'LP', 'L')
      NetApiBufferFree       = Win32API.new('netapi32', 'NetApiBufferFree', 'P', 'L')
      NetApiBufferReallocate = Win32API.new('netapi32', 'NetApiBufferReallocate', 'PLP', 'L')
      NetApiBufferSize       = Win32API.new('netapi32', 'NetApiBufferSize', 'PP', 'L')
      NetGetAnyDCName        = Win32API.new('netapi32', 'NetGetAnyDCName', 'PPP', 'L')
      NetGetDCName           = Win32API.new('netapi32', 'NetGetDCName', 'PPP', 'L')

      NetGetDisplayInformationIndex = Win32API.new('netapi32', 'NetGetDisplayInformationIndex', 'PLPP', 'L')
      NetGetJoinableOUs             = Win32API.new('netapi32', 'NetGetJoinableOUs', 'PPPPPP', 'L')
      NetGetJoinInformation         = Win32API.new('netapi32', 'NetGetJoinInformation', 'PPP', 'L')

      NetGroupAdd      = Win32API.new('netapi32', 'NetGroupAdd', 'PLPP', 'L')
      NetGroupAddUser  = Win32API.new('netapi32', 'NetGroupAddUser', 'PPP', 'L')
      NetGroupDel      = Win32API.new('netapi32', 'NetGroupDel', 'PP', 'L')
      NetGroupDelUser  = Win32API.new('netapi32', 'NetGroupDelUser', 'PPP', 'L')
      NetGroupEnum     = Win32API.new('netapi32', 'NetGroupEnum', 'PLPLPPP', 'L')
      NetGroupGetInfo  = Win32API.new('netapi32', 'NetGroupGetInfo', 'PPLP', 'L')
      NetGroupGetUsers = Win32API.new('netapi32', 'NetGroupGetUsers', 'PPLPLPPP', 'L')
      NetGroupSetInfo  = Win32API.new('netapi32', 'NetGroupSetInfo', 'PPLPP', 'L')
      NetGroupSetUsers = Win32API.new('netapi32', 'NetGroupSetUsers', 'PPLPL', 'L')
      NetJoinDomain    = Win32API.new('netapi32', 'NetJoinDomain', 'PPPPPL', 'L')

      NetLocalGroupAdd        = Win32API.new('netapi32', 'NetLocalGroupAdd', 'PLPP', 'L')
      NetLocalGroupAddMembers = Win32API.new('netapi32', 'NetLocalGroupAddMembers', 'PPLPL', 'L')
      NetLocalGroupDel        = Win32API.new('netapi32', 'NetLocalGroupDel', 'PP', 'L')
      NetLocalGroupDelMembers = Win32API.new('netapi32', 'NetLocalGroupDelMembers', 'PPLPL', 'L')
      NetLocalGroupEnum       = Win32API.new('netapi32', 'NetLocalGroupEnum', 'PLPLPPP', 'L')
      NetLocalGroupGetInfo    = Win32API.new('netapi32', 'NetLocalGroupGetInfo', 'PPLP', 'L')
      NetLocalGroupGetMembers = Win32API.new('netapi32', 'NetLocalGroupGetMembers', 'PPLPLPPP', 'L')
      NetLocalGroupSetInfo    = Win32API.new('netapi32', 'NetLocalGroupSetInfo', 'PPLPP', 'L')
      NetLocalGroupSetMembers = Win32API.new('netapi32', 'NetLocalGroupSetMembers', 'PPLPP', 'L')
      NetMessageBufferSend    = Win32API.new('netapi32', 'NetMessageBufferSend', 'PPPPL', 'L')
      NetMessageNameAdd       = Win32API.new('netapi32', 'NetMessageNameAdd', 'PP', 'L')
      NetMessageNameDel       = Win32API.new('netapi32', 'NetMessageNameDel', 'PP', 'L')
      NetMessageNameEnum      = Win32API.new('netapi32', 'NetMessageNameEnum', 'PLPLPPP', 'L')
      NetMessageNameGetInfo   = Win32API.new('netapi32', 'NetMessageNameGetInfo', 'PPLP', 'L')

      NetQueryDisplayInformation = Win32API.new('netapi32', 'NetQueryDisplayInformation', 'PLLLLPP', 'L')
      NetRemoteComputerSupports  = Win32API.new('netapi32', 'NetRemoteComputerSupports', 'PLP', 'L')
      NetRemoteTOD               = Win32API.new('netapi32', 'NetRemoteTOD', 'PP', 'L')
      NetRenameMachineInDomain   = Win32API.new('netapi32', 'NetRenameMachineInDomain', 'PPPPL', 'L')
      NetScheduleJobAdd          = Win32API.new('netapi32', 'NetScheduleJobAdd', 'PPP', 'L')
      NetScheduleJobDel          = Win32API.new('netapi32', 'NetScheduleJobDel', 'PLL', 'L')
      NetScheduleJobEnum         = Win32API.new('netapi32', 'NetScheduleJobEnum', 'PPLPPP', 'L')
      NetScheduleJobGetInfo      = Win32API.new('netapi32', 'NetScheduleJobGetInfo', 'PLP', 'L')

      NetServerComputerNameAdd = Win32API.new('netapi32', 'NetServerComputerNameAdd', 'PPP', 'L')
      NetServerComputerNameDel = Win32API.new('netapi32', 'NetServerComputerNameDel', 'PP', 'L')
      NetServerDiskEnum        = Win32API.new('netapi32', 'NetServerDiskEnum', 'PLPLPPP', 'L')
      NetServerEnum            = Win32API.new('netapi32', 'NetServerEnum', 'PLPLPPLPP', 'L')
      NetServerGetInfo         = Win32API.new('netapi32', 'NetServerGetInfo', 'PLP', 'L')
      NetServerSetInfo         = Win32API.new('netapi32', 'NetServerSetInfo', 'PLPP', 'L')
      NetServerTransportAdd    = Win32API.new('netapi32', 'NetServerTransportAdd', 'PLP', 'L')
      NetServerTransportAddEx  = Win32API.new('netapi32', 'NetServerTransportAddEx', 'PLP', 'L')
      NetServerTransportDel    = Win32API.new('netapi32', 'NetServerTransportDel', 'PLP', 'L')
      NetServerTransportEnum   = Win32API.new('netapi32', 'NetServerTransportEnum', 'PLPLPPP', 'L')
      NetUnjoinDomain          = Win32API.new('netapi32', 'NetUnjoinDomain', 'PPPL', 'L')

      NetUseAdd     = Win32API.new('netapi32', 'NetUseAdd', 'PLPP', 'L')
      NetUseDel     = Win32API.new('netapi32', 'NetUseDel', 'PPL', 'L')
      NetUseEnum    = Win32API.new('netapi32', 'NetUseEnum', 'PLPLPPP', 'L')
      NetUseGetInfo = Win32API.new('netapi32', 'NetUseGetInfo', 'PPLP', 'L')

      NetUserAdd            = Win32API.new('netapi32', 'NetUserAdd', 'PLPP', 'L')
      NetUserChangePassword = Win32API.new('netapi32', 'NetUserChangePassword', 'PPPP', 'L')
      NetUserDel            = Win32API.new('netapi32', 'NetUserDel', 'PP', 'L')
      NetUserEnum           = Win32API.new('netapi32', 'NetUserEnum', 'PLLPLPPP', 'L')
      NetUserGetGroups      = Win32API.new('netapi32', 'NetUserGetGroups', 'PPLPLPP', 'L')
      NetUserGetInfo        = Win32API.new('netapi32', 'NetUserGetInfo', 'PPLP', 'L')
      NetUserGetLocalGroups = Win32API.new('netapi32', 'NetUserGetLocalGroups', 'PPLLPLPP', 'L')

      NetUserModalsGet = Win32API.new('netapi32', 'NetUserModalsGet', 'PLP', 'L')
      NetUserModalsSet = Win32API.new('netapi32', 'NetUserModalsSet', 'PLPP', 'L')
      NetUserSetGroups = Win32API.new('netapi32', 'NetUserSetGroups', 'PPLPL', 'L')
      NetUserSetInfo   = Win32API.new('netapi32', 'NetUserSetInfo', 'PPLPP', 'L')
      NetValidateName  = Win32API.new('netapi32', 'NetValidateName', 'PPPPP', 'L')

      NetWkstaGetInfo       = Win32API.new('netapi32', 'NetWkstaGetInfo', 'PLP', 'L')
      NetWkstaSetInfo       = Win32API.new('netapi32', 'NetWkstaSetInfo', 'PLPP', 'L')
      NetWkstaTransportAdd  = Win32API.new('netapi32', 'NetWkstaTransportAdd', 'PLPP', 'L')
      NetWkstaTransportDel  = Win32API.new('netapi32', 'NetWkstaTransportDel', 'PPL', 'L')
      NetWkstaTransportEnum = Win32API.new('netapi32', 'NetWkstaTransportEnum', 'PLPLPPP', 'L')
      NetWkstaUserEnum      = Win32API.new('netapi32', 'NetWkstaUserEnum', 'PLPLPPP', 'L')
      NetWkstaUserGetInfo   = Win32API.new('netapi32', 'NetWkstaUserGetInfo', 'PLP', 'L')
      NetWkstaUserSetInfo   = Win32API.new('netapi32', 'NetWkstaUserSetInfo', 'PPLP', 'L')

      begin 
         GetNetScheduleAccountInformation = Win32API.new('mstask', 'GetNetScheduleAccountInformation', 'PLP', 'L')
         SetNetScheduleAccountInformation = Win32API.new('netapi32', 'SetNetScheduleAccountInformation', 'PPP', 'L')
      rescue Exception
         # Do nothing.  Not supported on current platform
      end

      def NetAlertRaise(name, buf, bufsize)
         NetAlertRaise.call(name, buf, bufsize) == NERR_Success
      end

      def NetAlertRaiseEx(name, data, size, service)
         NetAlertRaiseEx.call(name, data, size, service) == NERR_Success
      end

      def NetApiBufferAllocate(num_bytes, buf)
         NetApiBufferAllocate.call(num_bytes, buf) == NERR_Success
      end

      def NetApiBufferFree(buf)
         NetApiBufferFree.call(buf) == NERR_Success
      end

      def NetApiBufferReallocate(old_buf, count, new_buf)
         NetApiBufferReallocate.call(old_buf, count, new_buf) == NERR_Success
      end

      def NetApiBufferSize(buf, count)
         NetApiBufferSize.call(buf, count) == NERR_Success
      end

      def NetGetAnyDCName(server, domain, buf)
         NetGetAnyDCName.call(server, domain, buf) == NERR_Success
      end

      def NetGetDCName(server, domain, buf)
         NetGetDCName.call(server, domain, buf) == NERR_Success
      end

      def NetGetDisplayInformationIndex(server, level, prefix, index)
         NetGetDisplayInformationIndex.call(server, level, prefix, index) == NERR_Success
      end

      def NetGetJoinableOUs(server, domain, account, password, count, ous)
         NetGetJoinableOUs.call(server, domain, account, password, count, ous) == NERR_Success
      end

      def NetGetJoinInformation(server, buf, buf_type)
         NetGetJoinInformation.call(server, buf, buf_type) == NERR_Success
      end

      def NetGroupAdd(server, level, buf, err)
         NetGroupAdd.call(server, level, buf, err).call == NERR_Success
      end

      def NetGroupAddUser(server, group, user)
         NetGroupAddUser.call(server, group, user) == NERR_Success
      end

      def NetGroupDel(server, group)
         NetGroupDel.call(server, group) == NERR_Success
      end

      def NetGroupDelUser(server, group, user)
         NetGroupDelUser.call(server, group, user) == NERR_Success
      end

      def NetGroupEnum(server, level, buf, max, entries, total_entries, resume)
         NetGroupEnum.call(server, level, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetGroupGetInfo(server, group, level, buf)
         NetGroupGetInfo.call(server, group, level, buf) == NERR_Success
      end

      def NetGroupGetUsers(server, group, level, buf, max, entries, total_entries, resume)
         NetGroupGetUsers.call(server, group, level, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetGroupSetInfo(server, group, level, buf, err)
         NetGroupSetInfo.call(server, group, level, buf, err) == NERR_Success
      end

      def NetGroupSetUsers(server, group, level, buf, total)
         NetGroupSetUsers.call(server, group, level, buf, total) == NERR_Success
      end

      def NetJoinDomain(server, domain, account_ou, account, password, opts)
         NetJoinDomain.call(server, domain, account_ou, account, password, opts) == NERR_Success
      end

      def NetLocalGroupAdd(server, level, buf, err)
         NetLocalGroupAdd.call(server, level, buf, err) == NERR_Success
      end

      def NetLocalGroupAddMembers(server, group, level, buf, total)
         NetLocalGroupAddMembers.call(server, group, level, buf, total) == NERR_Success
      end

      def NetLocalGroupDel(server, group)
         NetLocalGroupDel.call(server, group) == NERR_Success
      end

      def NetLocalGroupDelMembers(server, group, level, buf, total)
         NetLocalGroupDelMembers.call(server, group, level, buf, total) == NERR_Success
      end

      def NetLocalGroupEnum(server, level, buf, max, entries, total_entries, resume)
         NetLocalGroupEnum.call(server, level, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetLocalGroupGetInfo(server, group, level, buf)
         NetLocalGroupGetInfo.call(server, group, level, buf) == NERR_Success
      end

      def NetLocalGroupGetMembers(server, group, level, buf, max, entries, total_entries, resume)
         NetLocalGroupGetMembers.call(server, group, level, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetLocalGroupSetInfo(server, group, level, buf, err)
         NetLocalGroupSetInfo.call(server, group, level, buf, err) == NERR_Success
      end

      def NetLocalGroupSetMembers(server, group, level, buf, total)
         NetLocalGroupSetMembers.call(server, group, level, buf, total) == NERR_Success
      end

      def NetMessageBufferSend(server, msg, from, buf, bufsize)
         NetMessageBufferSend.call(server, msg, from, buf, bufsize) == NERR_Success
      end

      def NetMessageNameAdd(server, msg)
         NetMessageNameAdd.call(server, msg) == NERR_Success
      end

      def NetMessageNameDel(server, msg)
         NetMessageNameDel.call(server, msg) == NERR_Success
      end

      def NetMessageNameEnum(server, level, buf, max, entries, total_entries, resume)
         NetMessageNameEnum.call(server, level, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetMessageNameGetInfo(server, msg, level, buf)
         NetMessageNameGetInfo.call(server, msg, level, buf) == NERR_Success
      end

      def NetQueryDisplayInformation(server, level, index, entries, max, count, buf)
         NetQueryDisplayInformation.call(server, level, index, entries, max, count, buf) == NERR_Success
      end

      def NetRemoteComputerSupports(server, level, index, entries, max, count, buf)
         NetRemoteComputerSupports.call(server, level, index, entries, max, count, buf) == NERR_Success
      end

      def NetRemoteTOD(server, buf)
         NetRemoteTOD.call(server, buf) == NERR_Success
      end

      def NetRenameMachineInDomain(server, machine, account, password, options)
         NetRenameMachineInDomain.call(server, machine, account, password, options) == NERR_Success
      end

      def NetScheduleJobAdd(server, buf, job)
         NetScheduleJobAdd.call(server, buf, job) == NERR_Success
      end
      
      def NetScheduleJobDel(server, min, max)
         NetScheduleJobDel.call(server, min, max) == NERR_Success
      end

      def NetScheduleJobEnum(server, buf, max, entries, total_entries, resume)
         NetScheduleJobEnum.call(server, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetScheduleJobGetInfo(server, job, buf)
         NetScheduleJobGetInfo.call(server, job, buf) == NERR_Success
      end

      def NetServerComputerNameAdd(server, em_domain, em_server)
         NetServerComputerNameAdd.call(server, em_domain, em_server) == NERR_Success
      end

      def NetServerComputerNameDel(server, em_server)
         NetServerComputerNameDel.call(server, em_server) == NERR_Success
      end

      def NetServerDiskEnum(server, level, buf, maxlen, entries, total_entries, resume)
         NetServerDiskEnum.call(server, level, buf, maxlen, entries, total_entries, resume) == NERR_Success
      end

      def NetServerEnum(server, level, ptr, maxlen, num, total, stype, domain, handle)
         NetServerEnum.call(server, level, ptr, maxlen, num, total, stype, domain, handle) == NERR_Success
      end

      def NetServerGetInfo(server, level, buf)
         NetServerGetInfo.call(server, level, buf) == NERR_Success
      end

      def NetServerSetInfo(server, level, buf, error)
         NetServerSetInfo.call(server, level, buf, error) == NERR_Success
      end

      def NetServerTransportAdd(server, level, buf)
         NetServerTransportAdd.call(server, level, buf) == NERR_Success
      end

      def NetServerTransportAddEx(server, level, buf)
         NetServerTransportAddEx.call(server, level, buf) == NERR_Success
      end

      def NetServerTransportDel(server, level, buf)
         NetServerTransportDel.call(server, level, buf) == NERR_Success
      end

      def NetServerTransportEnum(server, level, buf, maxlen, entries, total_entries, resume)
         NetServerTransportEnum.call(server, level, buf, maxlen, entries, total_entries, resume) == NERR_Success
      end

      def NetUnjoinDomain(server, account, password, options)
         NetUnjoinDomain.call(server, account, password, options) == NERR_Success
      end

      def NetUseAdd(server, level, buf, error)
         NetUseAdd.call(server, level, buf, error) == NERR_Success
      end

      def NetUseDel(server, name, conn)
         NetUseDel.call(server, name, conn) == NERR_Success
      end

      def NetUseEnum(server, level, buf, max, entries, total_entries, resume)
         NetUseEnum.call(server, level, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetUseGetInfo(server, name, level, buf)
         NetUseGetInfo.call(server, name, level, buf) == NERR_Success
      end

      def NetUserAdd(server, level, buf, error)
         NetUserAdd.call(server, level, buf, error) == NERR_Success
      end

      def NetUserChangePassword(domain, user, old, new)
         NetUserChangePassword.call(domain, user, old, new) == NERR_Success
      end

      def NetUserDel(server, user)
         NetUserDel.call(server, user) == NERR_Success
      end

      def NetUserEnum(server, level, filter, buf, max, entries, total_entries, resume)
         NetUserEnum.call(server, level, filter, buf, max, entries, total_entries, resume) == NERR_Success
      end

      def NetUserGetGroups(server, user, level, buf, max, entries, total_entries)
         NetUserGetGroups.call(server, user, level, buf, max, entries, total_entries) == NERR_Success
      end

      def NetUserGetInfo(server, user, level, buf)
         NetUserGetInfo.call(server, user, level, buf) == NERR_Success
      end

      def NetUserGetLocalGroups(server, user, level, flags, buf, max, entries, total_entries)
         NetUserGetLocalGroups.call(server, user, level, flags, buf, max, entries, total_entries) == NERR_Success
      end

      def NetUserModalsGet(server, level, buf)
         NetUserModalsGet.call(server, level, buf) == NERR_Success
      end

      def NetUserModalsSet(server, level, buf, error)
         NetUserModalsSet.call(server, level, buf, error) == NERR_Success
      end

      def NetUserSetGroups(server, user, level, buf, num)
         NetUserSetGroups.call(server, user, level, buf, num) == NERR_Success
      end

      def NetUserSetInfo(server, user, level, buf, error)
         NetUserSetInfo.call(server, user, level, buf, error) == NERR_Success
      end

      def NetValidateName(server, name, account, password, name_type)
         NetValidateName.call(server, name, account, password, name_type) == NERR_Success
      end

      def NetWkstaGetInfo(server, level, buf)
         NetWkstaGetInfo.call(server, level, buf) == NERR_Success
      end

      def NetWkstaSetInfo.call(server, level, buf, error)
         NetWkstaSetInfo.call(server, level, buf, error) == NERR_Success
      end

      def NetWkstaTransportAdd(server, level, buf, error)
         NetWkstaTransportAdd.call(server, level, buf, error) == NERR_Success
      end

      def NetWkstaTransportDel(server, name, cond)
         NetWkstaTransportDel.call(server, name, cond) == NERR_Success
      end

      def NetWkstaTransportEnum(server, level, buf, maxlen, entries, total_entries, resume)
         NetWkstaTransportEnum.call(server, level, buf, maxlen, entries, total_entries, resume) == NERR_Success
      end

      def NetWkstaUserEnum(server, level, buf, maxlen, entries, total_entries, resume)
         NetWkstaUserEnum.call(server, level, buf, maxlen, entries, total_entries, resume) == NERR_Success
      end

      def NetWkstaUserGetInfo(res, level, buf)
         NetWkstaUserGetInfo.call(res, level, buf) == NERR_Success
      end

      def NetWkstaUserSetInfo(res, level, buf, error)
         NetWkstaUserSetInfo.call(res, level, buf, error) == NERR_Success
      end

      # Windows XP or later
      begin
         def GetNetScheduleAccountInformation(server, num_chars, chars)
            GetNetScheduleAccountInformation.call(server, num_chars, chars) == NERR_Success
         end

         def SetNetScheduleAccountInformation(server, account, password)
            SetNetScheduleAccountInformation.call(server, account, password) == NERR_Success
         end
      rescue Exception
      end
   end
end
