/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgGame.h"

rpgGame::rpgGame() { gRpgGame = this; }

void
rpgGame::start()
{
    JLE_EXEC_IF(RPG_BUILD_SERVER) { loadScene(jlePath{"GR:/scenes/server.scn"}); }
    else JLE_EXEC_IF(RPG_BUILD_IN_PROC_SERVER)
    {
        loadScene(jlePath{"GR:/scenes/server.scn"});
    }

    JLE_EXEC_IF_NOT(RPG_BUILD_SERVER) { loadScene(jlePath{"GR:/scenes/client.scn"}); }
}

void
rpgGame::update(float dt)
{
}

rpgGame::~rpgGame() {}
