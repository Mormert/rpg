// Copyright (c) 2023. Johan Lind

#pragma once

#include "jleComponent.h"

class cRpgPlayer : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cRpgPlayer)
public:
    explicit cRpgPlayer(jleObject *owner = nullptr, jleScene *scene = nullptr);

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(_color));
    }

    NET_SYNC(_color, _someInt)

    void start() override;

    void update(float dt) override;

    void serverUpdate(float dt) override;

protected:
    glm::vec3 _color{1.f};
    int _someInt{3};
};

CEREAL_REGISTER_TYPE(cRpgPlayer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cRpgPlayer)