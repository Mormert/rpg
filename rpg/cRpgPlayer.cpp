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

struct rpgPlayerNetworkEvent : public jleClientToServerEvent {

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(someThing), CEREAL_NVP(entityId));
    }

    glm::vec3 someThing{};
    int64_t entityId{};

    void
    execute() override
    {
        // LOGI << "Received event with entity id: " << entityId;
    }
};

JLE_REGISTER_NET_EVENT(rpgPlayerNetworkEvent)

struct rpgPlayerNetworkEvent2 : public jleClientToServerEvent {

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(someThing), CEREAL_NVP(entityId));
    }

    glm::vec3 someThing{};
    int64_t entityId{};

    void
    execute() override
    {
        // LOGI << "Received event2 with entity id: " << entityId;
    }
};

JLE_REGISTER_NET_EVENT(rpgPlayerNetworkEvent2)

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
            startLocalPlayer();
        }
    }
}

void
cRpgPlayer::startLocalPlayer()
{
    auto cameraObject = scene()->spawnObjectWithName("playerCamera");
    cameraObject->addComponent<cCamera>();
    auto playerCamComp = cameraObject->addComponent<cRpgPlayerCamera>();
    playerCamComp->setTarget(getObjectSharedPtr());
}

void
cRpgPlayer::update(float dt)
{
    if (gEngine->input().keyboard->keyPressed(jleKey::T)) {
        for (int i = 0; i < 10; i++) {
            auto event = jleMakeNetEvent<rpgPlayerNetworkEvent>();
            event->someThing = glm::vec3{5.f};
            event->entityId = object()->netID();
            if (auto *scn = sceneClient()) {
                scn->sendNetworkEvent(std::move(event));
            }
        }
    }
    if (gEngine->input().keyboard->keyPressed(jleKey::R)) {
        auto event = jleMakeNetEvent<rpgPlayerNetworkEvent2>();
        event->someThing = glm::vec3{5.f};
        event->entityId = object()->netID();
        auto *scn = dynamic_cast<jleSceneClient *>(scene());
        if (scn) {
            scn->sendNetworkEvent(std::move(event));
        }
    }
}

void
cRpgPlayer::serverUpdate(float dt)
{
    // syncServerToClient();
}
