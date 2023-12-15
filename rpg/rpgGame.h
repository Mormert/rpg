// Copyright (c) 2023. Johan Lind

#pragma once

#include "jleGame.h"
#include "jleMesh.h"
#include "rpgWorld.h"

class rpgGame;

inline rpgGame *gRpgGame;

class rpgGame final : public jleGame
{
public:
    rpgGame();

    ~rpgGame() override;

    void start() override;

    void update(float dt) override;

    rpgWorld &getWorld();

private:
    rpgWorld _world;
};