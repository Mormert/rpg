/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cRpgPlayer.h"

#include "cRpgPlayerCamera.h"
#include "editor/jleImGuiCerealArchive.h"

#include "jleGameEngine.h"
#include "jleInput.h"
#include "jleKeyboardInput.h"
#include "jleNetworkEvent.h"
#include "jleSceneClient.h"

#include <cCamera.h>
#include <jleSceneServer.h>

struct rpgPlayerMovementEvent : public jleClientToServerEvent {

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(objectNetId), CEREAL_NVP(movementCode));
    }

    int32_t objectNetId{};
    uint8_t movementCode{};

    void
    execute() override
    {
        const auto dt = gEngine->deltaFrameTime();

        if (auto object = getSceneServer().getObjectFromNetId(objectNetId)) {
            if (movementCode == static_cast<char>(jleKey::W)) {
                object->getTransform().addLocalTranslation(object->getTransform().getForward() * dt);
            }
            if (movementCode == static_cast<char>(jleKey::S)) {
                object->getTransform().addLocalTranslation(-object->getTransform().getForward() * dt);
            }
            if (movementCode == static_cast<char>(jleKey::A)) {
                object->getTransform().addLocalTranslation(object->getTransform().getRight() * dt);
            }
            if (movementCode == static_cast<char>(jleKey::D)) {
                object->getTransform().addLocalTranslation(-object->getTransform().getRight() * dt);
            }
        }
    }
};

JLE_REGISTER_NET_EVENT(rpgPlayerMovementEvent)

cRpgPlayer::
cRpgPlayer(jleObject *owner, jleScene *scene)
    : jleComponent(owner, scene)
{
}

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
}

void
cRpgPlayer::playerInput()
{
    if (gEngine->input().keyboard->keyDown(jleKey::W)) {
        auto event = jleMakeNetEvent<rpgPlayerMovementEvent>();
        event->movementCode = static_cast<uint8_t>(jleKey::W);
        event->objectNetId = object()->netID();
        sceneClient()->sendNetworkEvent(std::move(event));
    }
    if (gEngine->input().keyboard->keyDown(jleKey::S)) {
        auto event = jleMakeNetEvent<rpgPlayerMovementEvent>();
        event->movementCode = static_cast<uint8_t>(jleKey::S);
        event->objectNetId = object()->netID();
        sceneClient()->sendNetworkEvent(std::move(event));
    }
    if (gEngine->input().keyboard->keyDown(jleKey::A)) {
        auto event = jleMakeNetEvent<rpgPlayerMovementEvent>();
        event->movementCode = static_cast<uint8_t>(jleKey::A);
        event->objectNetId = object()->netID();
        sceneClient()->sendNetworkEvent(std::move(event));
    }
    if (gEngine->input().keyboard->keyDown(jleKey::D)) {
        auto event = jleMakeNetEvent<rpgPlayerMovementEvent>();
        event->movementCode = static_cast<uint8_t>(jleKey::D);
        event->objectNetId = object()->netID();
        sceneClient()->sendNetworkEvent(std::move(event));
    }
}

void
cRpgPlayer::serverUpdate(float dt)
{
    // syncServerToClient();
}
