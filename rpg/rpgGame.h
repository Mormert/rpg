// Copyright (c) 2023. Johan Lind

#pragma once

#include "jleGame.h"
#include "jleMesh.h"

class rpgGame final : public jleGame
{
public:
    ~rpgGame() override;

    void start() override;

    void update(float dt) override;
};