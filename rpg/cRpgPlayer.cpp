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

struct rpgPlayerNetworkEvent : public jleNetworkEvent {

    template <class Archive>
    void
    serialize(Archive &archive)
    {
        archive(CEREAL_NVP(someThing), CEREAL_NVP(entityId));
    }

    glm::vec3 someThing{};
    int64_t entityId{};

    void
    onReceiveFromClient(int32_t senderId) override
    {
        LOGI << "Received event with entity id: " << entityId << " from sender: " << senderId;
    }
};

CEREAL_REGISTER_TYPE(rpgPlayerNetworkEvent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleNetworkEvent, rpgPlayerNetworkEvent)

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
}

void
cRpgPlayer::serverUpdate(float dt)
{
    int a = 3;
}
