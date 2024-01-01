/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgGame.h"

rpgGame::rpgGame() { gRpgGame = this; }

void
rpgGame::start()
{
#ifdef JLE_BUILD_HEADLESS
    loadScene(jlePath{"GR:/scenes/server.scn"});
#else
#ifndef NDEBUG
    loadScene(jlePath{"GR:/scenes/server.scn"});
#endif
    loadScene(jlePath{"GR:/scenes/client.scn"});
#endif
}

void
rpgGame::update(float dt)
{
}

rpgGame::~rpgGame() {}
