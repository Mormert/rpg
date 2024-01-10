/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgSceneServer.h"

#include "cRpgPlayer.h"

JLE_EXTERN_TEMPLATE_CEREAL_CPP(rpgSceneServer)

void
rpgSceneServer::onClientConnect(int32_t clientId)
{
    const auto player = spawnObjectWithOwner("player_" + std::to_string(clientId), clientId);
    player->addComponent<cRpgPlayer>();
    player->getTransform().setLocalPosition({50.f, 0.f, 50.f});

    player->spawnChildObjectFromTemplate(jlePath{"GR:/otemps/player_gfx.jobj"});
}

template <class Archive>
void
rpgSceneServer::serialize(Archive &archive)
{
    archive(cereal::base_class<jleScene>(this));
}
