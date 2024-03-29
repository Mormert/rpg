/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cRpgPlayer.h"

#include "cPathFollower.h"
#include "cRpgPlayerCamera.h"

#include <cCamera.h>
#include <editor/jleImGuiCerealArchive.h>
#include <jle3DGraph.h>
#include <jleNetworkEvent.h>
#include <jleSceneClient.h>
#include <jleSceneServer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct rpgPlayerMovementEventClientToServer : public jleClientToServerEvent {

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(objectNetId), CEREAL_NVP(moveToPosition));
    }

    int32_t objectNetId{};
    glm::vec3 moveToPosition{};

    void
    execute() override
    {
        if (auto object = getSceneServer().getObjectFromNetId(objectNetId)) {
            if (auto pathFollower = object->getComponent<cPathFollower>()) {
                const auto moveFrom = pathFollower->getCurrentPosition();
                pathFollower->_moveFromPosition = moveFrom;
                pathFollower->_moveToPosition = moveToPosition;
                pathFollower->_interpolationAlpha = 0.f;

                auto event = jleMakeNetEvent<rpgPathFollowEventServerToClient>();
                event->moveToPosition = moveToPosition;
                event->moveFromPosition = moveFrom;
                event->objectNetId = object->netID();
                getSceneServer().sendNetworkEventBroadcast(std::move(event));
            }
        }
    }
};

JLE_REGISTER_NET_EVENT(rpgPlayerMovementEventClientToServer)

void
cRpgPlayer::start()
{
    if (const auto *client = sceneClient()) {
        const auto clientId = client->clientId();
        const auto ownerId = object()->netOwnerID();
        if (clientId == ownerId) {
            _localPlayer = true;
            startLocalPlayer();
        }
    }

    _graphicsChild = object()->spawnChildObjectFromTemplate(jlePath{"GR:/otemps/player_gfx.jobj"});

    object()->addComponent<cPathFollower>();
}

void
cRpgPlayer::serverStart()
{
    object()->addComponent<cPathFollower>();
}

void
cRpgPlayer::startLocalPlayer()
{
    auto cameraObject = scene()->spawnObjectWithName("playerCamera");
    auto playerCamComp = cameraObject->addComponent<cRpgPlayerCamera>();
    playerCamComp->setTarget(getObjectSharedPtr());
}

void
cRpgPlayer::update(float dt)
{
    if (auto child = _graphicsChild.lock()) {
        if (const auto pathFollower = object()->getComponent<cPathFollower>()) {
            const auto moveTo = pathFollower->getMoveToPosition();
            const auto currentPos = pathFollower->getCurrentPosition();
            const auto diff = moveTo - currentPos;
            if (glm::length(diff) > 0.f) {
                glm::vec3 direction = glm::normalize(diff);

                float angle = atan2(direction.x, direction.z) + glm::pi<float>();

                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
                rotationMatrix = glm::rotate(rotationMatrix, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));

                child->getTransform().setLocalMatrix(rotationMatrix);
            }
        }
    }
}

void
cRpgPlayer::serverUpdate(float dt)
{
}

void
cRpgPlayer::playerMoveTo(const glm::vec3 &position)
{
    auto event = jleMakeNetEvent<rpgPlayerMovementEventClientToServer>();
    event->moveToPosition = position;
    event->objectNetId = object()->netID();
    sceneClient()->sendNetworkEvent(std::move(event));
}
