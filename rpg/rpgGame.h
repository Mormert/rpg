/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include "rpgBuildConfig.h"

#include "jleGame.h"

class rpgGame;

inline rpgGame *gRpgGame;

class rpgGame final : public jleGame
{
public:
    rpgGame();

    ~rpgGame() override;

    void start() override;

    void update(float dt) override;
private:
};