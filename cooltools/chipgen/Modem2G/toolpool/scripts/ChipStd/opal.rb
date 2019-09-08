
require "base.rb"
require "help.rb"
require "target_executor.rb"


module Opal
   
public   

    addHelpEntry("Opal","opalDebug","write_flag,reg,value","readvalue","Call the debug function of PMD to access opal registers")

def opalDebug(write_flag,reg,value)
    funcAddr=$map_table.>.pmd_access.>.accessFunction.R
    if 0 == funcAddr
        raise "opalDebug no function pointer"
    end
    # build parameter
    paramStruct=CH__PMD_OPAL_DEBUG_FUNC_PARAM_T.new(0)
    param=0;
    param=paramStruct.Param.Write_Flag.wl(param, write_flag);
    param=paramStruct.Param.Reg.wl(param, reg);
    param=paramStruct.Param.Value.wl(param, value);
    paramStruct=nil
    #xp param

    tgxtor = TargetXtor.new
    begin
        ret=tgxtor.targetExecute(funcAddr,0,param);
    ensure
        tgxtor.closeConnection
    end
    tgxtor=nil
    ret[1]
end

    addHelpEntry("Opal","opalRead","reg","readvalue","Call the debug function of PMD to read opal registers")

def opalRead(reg)
    opalDebug(0,reg,0)
end

    addHelpEntry("Opal","opalWrite","reg,value","","Call the debug function of PMD to write opal registers")

def opalWrite(reg,value)
    opalDebug(1,reg,value)
end

end

include Opal
