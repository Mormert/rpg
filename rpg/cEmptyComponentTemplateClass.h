/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include <jleComponent.h>

class cEmptyComponentTemplateClass : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cEmptyComponentTemplateClass)
public:
    template <class Archive>
    void
    serialize(Archive &archive)
    {
    }

    void start() override;

    void serverStart() override;

    void update(float dt) override;

    void serverUpdate(float dt) override;
};

CEREAL_REGISTER_TYPE(cEmptyComponentTemplateClass)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cEmptyComponentTemplateClass)