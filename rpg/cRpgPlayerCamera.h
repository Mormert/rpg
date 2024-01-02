/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include <jleComponent.h>

class cRpgPlayerCamera : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cRpgPlayerCamera)
public:

    explicit cRpgPlayerCamera(jleObject *owner = nullptr, jleScene *scene = nullptr);


    void update(float dt) override;

    void setTarget(std::weak_ptr<jleObject> target);

private:

    std::weak_ptr<jleObject> _target;
};


CEREAL_REGISTER_TYPE(cRpgPlayerCamera)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cRpgPlayerCamera)