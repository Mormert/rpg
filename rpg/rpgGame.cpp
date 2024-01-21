/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgGame.h"

rpgGame::rpgGame() { gRpgGame = this; }

void
rpgGame::start()
{
    loadScene(jlePath{"GR:/scenes/pid_tests.scn"});

    /* // Temporarily removed to test PIDs instead ...
#ifdef JLE_BUILD_HEADLESS
    loadScene(jlePath{"GR:/scenes/server.scn"});
#else
#ifndef NDEBUG
    loadScene(jlePath{"GR:/scenes/server.scn"});
#endif
    loadScene(jlePath{"GR:/scenes/client.scn"});
#endif
     */
}

void
rpgGame::update(float dt)
{
}

rpgGame::~rpgGame() {}
