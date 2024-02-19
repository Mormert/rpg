/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgBuildConfig.h"
#include "rpgGame.h"

#include <jleKickStarter.h>

class rpgKickStarter : public jleKickStarter
{
public:
#if JLE_BUILD_RUNTIME_CONFIGURABLE
    void
    configureRuntime(const jleCommandArguments &commandLineArguments) override
    {
        jleKickStarter::configureRuntime(commandLineArguments);

        if (commandLineArguments.hasArgument("-server")) {
            JLE_BUILD_EDITOR_RUNTIME = false;
            JLE_BUILD_HEADLESS_RUNTIME = true;
            RPG_BUILD_SERVER_RUNTIME = true;
        }

        if (commandLineArguments.hasArgument("-server-inproc")) {
            RPG_BUILD_IN_PROC_SERVER_RUNTIME = true;
        }
    }
#endif
};

int
main(int argc, char *argv[])
{
    auto kickstarter = rpgKickStarter{};
    kickstarter.kickStart<rpgGame>(argc, argv);
    return 0;
}