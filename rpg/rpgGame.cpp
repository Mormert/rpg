// Copyright (c) 2023. Johan Lind

#include "rpgGame.h"

rpgGame::rpgGame() { gRpgGame = this; }

void
rpgGame::start()
{
    loadScene(jlePath{"GR:/scenes/main.scn"});
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
