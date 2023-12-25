// Copyright (c) 2023. Johan Lind

#include "cRpgPlayer.h"

#include "editor/jleImGuiCerealArchive.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include "jleGameEngine.h"
#include "jleInput.h"
#include "jleKeyboardInput.h"
#include "jleNetworkEvent.h"
#include "jleSceneClient.h"

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
        LOGI << "Received event with entity id: " << entityId;
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
        LOGI << "Received event2 with entity id: " << entityId;
    }
};

JLE_REGISTER_NET_EVENT(rpgPlayerNetworkEvent2)

cRpgPlayer::cRpgPlayer(jleObject *owner, jleScene *scene) : jleComponent(owner, scene) {}

void
cRpgPlayer::start()
{
}

void
cRpgPlayer::update(float dt)
{
    if (gEngine->input().keyboard->keyPressed(jleKey::T)) {
        auto event = jleMakeNetEvent<rpgPlayerNetworkEvent>();
        event->someThing = glm::vec3{5.f};
        event->entityId = object()->netEntityID();
        auto *scn = dynamic_cast<jleSceneClient *>(scene());
        if (scn) {
            scn->sendNetworkEvent(std::move(event));
        }
    }
    if (gEngine->input().keyboard->keyPressed(jleKey::R)) {
        auto event = jleMakeNetEvent<rpgPlayerNetworkEvent2>();
        event->someThing = glm::vec3{5.f};
        event->entityId = object()->netEntityID();
        auto *scn = dynamic_cast<jleSceneClient *>(scene());
        if (scn) {
            scn->sendNetworkEvent(std::move(event));
        }
    }
}

void
cRpgPlayer::serverUpdate(float dt)
{
    int a = 3;
}
