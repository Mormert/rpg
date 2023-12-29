/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgWorld.h"

rpgWorld::rpgWorld() {}

rpgWorldGrid &
rpgWorld::getGrid()
{
    return _grid;
}
