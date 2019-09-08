require "CoolTester/CoolTesterPluginActivation.rb"

include CoolTesterPluginActivation

def activateCoolTester
    
    testsPath=$PLUGINS_WORKDIR +"/CoolTester/tests"
    
    pathsToLoad=[$PLUGINS_WORKDIR +"/CoolTester",
                testsPath,
                $PLUGINS_WORKDIR +"/CoolTester/scenarii"
                ]
    
    puts "Adding paths to $LOAD_PATH : "
    begin
        CoolTesterPluginActivation.addLoadPaths( $LOAD_PATH, pathsToLoad )
        CoolTesterPluginActivation.loadUserTests(testsPath)
        default_help_path = "//Atlas/n/users/cooltester/public_html/"
        if (!File.exist?(default_help_path + "tests_list.php"))
            default_help_path = testsPath=$PLUGINS_WORKDIR + "/CoolTester/public_html/"
            if (!File.exist?(default_help_path))
                begin
                    Dir.mkdir(default_help_path)
                rescue SystemCallError => e
                    err_info = "Failed to create dir : "+default_help_path+"\n#{e}"
                    raise CT_Plugin_Activ_Err, err_info, caller
                end
            end
        end
        CoolTesterPluginActivation.createTestsHelp(default_help_path + "tests_list.php",
        default_help_path + "tests_content.php" )
        
        CoolTesterPluginActivation.setGUI
    
         puts "html><font color='darkgreen'>"+"CoolTester Plugin activation successed !" +"</font>"
         puts "Check you have load a .lod with GTES !"
         puts "Once, init CoolTester thanks to the 'blue initialization icon' and run your tests/scenarii!"
    rescue CT_Plugin_Activ_Err
        puts "html><font color='red'>"+"An error Occured while activating CoolTester plugin : " +"</font>"
        # Print error infos.
        puts "#{$!}"
    end
    
end

#CoolWatcher specific
begin
    include CoolWatcher
    cwAddMenuCommand("Plugins","Activate CoolTester","activateCoolTester()",0)
rescue Exception
end

