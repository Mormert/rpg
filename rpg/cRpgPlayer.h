/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include "jleComponent.h"

class cRpgPlayer : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cRpgPlayer)
public:

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(_color));
    }

    NET_SYNC(_color, _someInt)

    void start() override;

    void startLocalPlayer();

    void update(float dt) override;

    void playerInput();

    void serverUpdate(float dt) override;

    void playerMoveTo(const glm::vec3& position);

protected:
    friend struct rpgPlayerMovementEventServerToClient;
    friend struct rpgPlayerMovementEventClientToServer;

    void moveTowardsPosition(float dt);
    glm::vec3 _moveToPosition{};

    glm::vec3 _color{1.f};
    int _someInt{3};

    bool _localPlayer = false;
};

CEREAL_REGISTER_TYPE(cRpgPlayer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cRpgPlayer)