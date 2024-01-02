/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include "jleSceneServer.h"

class rpgSceneServer : public jleSceneServer
{
    JLE_REGISTER_RESOURCE_TYPE(rpgSceneServer, "scn")
public:
    void onClientConnect(int32_t clientId) override;

    template <class Archive>
    void serialize(Archive &archive);
};

JLE_EXTERN_TEMPLATE_CEREAL_H(rpgSceneServer)

CEREAL_REGISTER_TYPE(rpgSceneServer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleSerializedOnlyResource, rpgSceneServer)