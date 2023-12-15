// Copyright (c) 2023. Johan Lind

#pragma once

#include "jleMaterial.h"

struct rpgMaterialFlatNoiseColorThreshold
{
    jleRGB color = jleRGB{1.f};
    float threshold{};

    template <class Archive>
    void serialize(Archive &ar);
};

JLE_EXTERN_TEMPLATE_CEREAL_H(rpgMaterialFlatNoiseColorThreshold)


class rpgMaterialFlatNoise : public jleMaterial
{
public:
    JLE_REGISTER_RESOURCE_TYPE(rpgMaterialFlatNoise, "mat")

    rpgMaterialFlatNoise();

    void useMaterial(const jleCamera &camera,
                     const std::vector<jle3DRendererLight> &lights,
                     const jle3DSettings &settings) override;

    template <class Archive>
    void serialize(Archive &ar);

    SAVE_SHARED_THIS_SERIALIZED_JSON(jleMaterial);

private:
    std::vector<rpgMaterialFlatNoiseColorThreshold> _colors;
};

JLE_EXTERN_TEMPLATE_CEREAL_H(rpgMaterialFlatNoise)


CEREAL_REGISTER_TYPE(rpgMaterialFlatNoise)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleMaterial, rpgMaterialFlatNoise)
