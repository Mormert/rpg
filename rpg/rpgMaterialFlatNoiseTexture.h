/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include <jleMaterial.h>
#include <jleRGB.h>

#include <cereal/types/vector.hpp>

class rpgMaterialFlatNoiseTexture : public jleMaterial
{
public:
    JLE_REGISTER_RESOURCE_TYPE(rpgMaterialFlatNoiseTexture, "mat")

    rpgMaterialFlatNoiseTexture();

    void useMaterial(const jleCamera &camera,
                     const std::vector<jle3DRendererLight> &lights,
                     const jle3DSettings &settings) override;

    template <class Archive>
    void serialize(Archive &ar);

    SAVE_SHARED_THIS_SERIALIZED_JSON(jleMaterial);

private:
    jleTextureRefOrRGB _albedo{glm::vec3{1.f}}; // White by default
};

JLE_EXTERN_TEMPLATE_CEREAL_H(rpgMaterialFlatNoiseTexture)

CEREAL_REGISTER_TYPE(rpgMaterialFlatNoiseTexture)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleMaterial, rpgMaterialFlatNoiseTexture)
