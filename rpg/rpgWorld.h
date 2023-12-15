// Copyright (c) 2023. Johan Lind

#pragma once

#include "rpgWorldSquare.h"

#include <array>

using rpgWorldGrid = std::array<std::array<rpgWorldSquare, 100>, 100>;

class rpgWorld
{
public:
    rpgWorld();

    rpgWorldGrid &getGrid();

private:
    rpgWorldGrid _grid;
};
