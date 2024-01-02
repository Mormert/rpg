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
}

template <class Archive>
void
rpgSceneServer::serialize(Archive &archive)
{
    archive(cereal::base_class<jleScene>(this));
}
