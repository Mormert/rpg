/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgGame.h"

rpgGame::rpgGame() { gRpgGame = this; }

void
rpgGame::start()
{
}

void
rpgGame::update(float dt)
{
}

rpgGame::~rpgGame() {}

rpgWorld &
rpgGame::getWorld()
{
    return _world;
}
