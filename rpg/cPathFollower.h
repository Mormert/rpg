/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include <jleComponent.h>
#include <jleNetworkEvent.h>
#include <jleSceneClient.h>

class cPathFollower : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cPathFollower)
public:
    template <class Archive>
    void
    serialize(Archive &archive)
    {
    }

    void start() override;

    void update(float dt) override;

    void serverUpdate(float dt) override;

    glm::vec3 getCurrentPosition() const;

    glm::vec3 getMoveToPosition() const;

private:
    friend struct rpgPathFollowEventServerToClient;
    friend struct rpgPlayerMovementEventClientToServer;

    void moveTowardsPosition(float dt);
    glm::vec3 _moveToPosition{};
    glm::vec3 _moveFromPosition{};
    float _interpolationAlpha{};

};

struct rpgPathFollowEventServerToClient : public jleServerToClientEvent {
    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(objectNetId), CEREAL_NVP(moveFromPosition), CEREAL_NVP(moveToPosition));
    }

    int32_t objectNetId{};
    glm::vec3 moveFromPosition{};
    glm::vec3 moveToPosition{};

    void execute() override;
};
JLE_REGISTER_NET_EVENT(rpgPathFollowEventServerToClient)


CEREAL_REGISTER_TYPE(cPathFollower)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cPathFollower)