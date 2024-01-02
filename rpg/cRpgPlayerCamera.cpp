/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cRpgPlayerCamera.h"

#include <utility>

cRpgPlayerCamera::
cRpgPlayerCamera(jleObject *owner, jleScene *scene)
{
}

void
cRpgPlayerCamera::update(float dt)
{
}

void
cRpgPlayerCamera::setTarget(std::weak_ptr<jleObject> target)
{
    _target = std::move(target);
}