/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cRpgPlayer.h"

#include "cRpgPlayerCamera.h"
#include "editor/jleImGuiCerealArchive.h"

#include "cCamera.h"
#include "jle3DGraph.h"
#include "jleGameEngine.h"
#include "jleInput.h"
#include "jleKeyboardInput.h"
#include "jleNetworkEvent.h"
#include "jlePhysics.h"
#include "jleSceneClient.h"
#include "jleSceneServer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btVector3.h>

struct rpgPlayerMovementEventServerToClient : public jleServerToClientEvent {

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(objectNetId), CEREAL_NVP(moveFromPosition), CEREAL_NVP(moveToPosition));
    }

    int32_t objectNetId{};
    glm::vec3 moveFromPosition{};
    glm::vec3 moveToPosition{};

    void
    execute() override
    {
        if (auto object = getSceneClient().getObjectFromNetId(objectNetId)) {
            auto player = object->getComponent<cRpgPlayer>();
            player->_moveFromPosition = moveFromPosition;
            player->_moveToPosition = moveToPosition;
            player->_interpolationAlpha = 0.f;
        }
    }
};

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
            auto player = object->getComponent<cRpgPlayer>();
            const auto moveFrom = player->getCurrentPosition();
            player->_moveFromPosition = moveFrom;
            player->_moveToPosition = moveToPosition;
            player->_interpolationAlpha = 0.f;

            auto event = jleMakeNetEvent<rpgPlayerMovementEventServerToClient>();
            event->moveToPosition = moveToPosition;
            event->moveFromPosition = moveFrom;
            event->objectNetId = object->netID();
            getSceneServer().sendNetworkEventBroadcast(std::move(event));
        }
    }
};

JLE_REGISTER_NET_EVENT(rpgPlayerMovementEventClientToServer)
JLE_REGISTER_NET_EVENT(rpgPlayerMovementEventServerToClient)

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
    if (_localPlayer) {
        playerInput();
    }

    moveTowardsPosition(dt);

    /*auto positionRotateTowards = _moveToPosition;
    positionRotateTowards.y = getTransform().getLocalPosition().y;
    getTransform().rotateTowardsPoint(positionRotateTowards);*/

    gEngine->renderGraph().sendLine(_moveToPosition + glm::vec3{0.f, 0.5f, 0.f},
                                    _moveToPosition - glm::vec3{0.f, 0.5f, 0.f});
}

void
cRpgPlayer::playerInput()
{
}

void
cRpgPlayer::serverUpdate(float dt)
{
    moveTowardsPosition(dt);
}

void
cRpgPlayer::playerMoveTo(const glm::vec3 &position)
{
    auto event = jleMakeNetEvent<rpgPlayerMovementEventClientToServer>();
    event->moveToPosition = position;
    event->objectNetId = object()->netID();
    sceneClient()->sendNetworkEvent(std::move(event));
}

glm::vec3
cRpgPlayer::getCurrentPosition()
{
    return glm::mix(_moveFromPosition, _moveToPosition, _interpolationAlpha);
}

void
cRpgPlayer::moveTowardsPosition(float dt)
{
    const auto interpLength = glm::length(_moveFromPosition - _moveToPosition);

    _interpolationAlpha += 1.f/interpLength * dt * 10.f;
    _interpolationAlpha = glm::clamp(_interpolationAlpha, 0.f, 1.f);

    const auto interpPos = getCurrentPosition();

    getTransform().setWorldPosition(interpPos);

    btVector3 rayFrom = {interpPos.x, interpPos.y + 10.f, interpPos.z};
    btVector3 rayTo = {interpPos.x, interpPos.y - 10.f, interpPos.z};

    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

    gEngine->physics().dynamicsWorld().rayTest(rayFrom, rayTo, rayCallback);

    if (rayCallback.hasHit()) {
        glm::vec3 hitPos{
            rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z()};
        hitPos.y -= 1.5f;
        getTransform().setWorldPosition(hitPos);
    }
}
