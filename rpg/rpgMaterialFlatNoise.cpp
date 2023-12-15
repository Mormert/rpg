// Copyright (c) 2023. Johan Lind

#include "rpgMaterialFlatNoise.h"

JLE_EXTERN_TEMPLATE_CEREAL_CPP(rpgMaterialFlatNoiseColorThreshold)

template <class Archive>
void
rpgMaterialFlatNoiseColorThreshold::serialize(Archive &ar)
{
    try {
        ar(CEREAL_NVP(color), CEREAL_NVP(threshold));
    } catch (std::exception &e) {
        LOGE << "Failed loading color threshold:" << e.what();
    }
}

JLE_EXTERN_TEMPLATE_CEREAL_CPP(rpgMaterialFlatNoise)

rpgMaterialFlatNoise::rpgMaterialFlatNoise() { _shaderRef = jleResourceRef<jleShader>("GR:/shaders/flatNoise.glsl"); }

void
rpgMaterialFlatNoise::useMaterial(const jleCamera &camera,
                                  const std::vector<jle3DRendererLight> &lights,
                                  const jle3DSettings &settings)
{
    jleMaterial::useMaterial(camera, lights, settings);

    auto &shader = *_shaderRef.get();

    if(!_colors.empty())
    {
        std::vector<glm::vec3> c;
        c.reserve(_colors.size());

        std::vector<float> h;
        h.reserve(_colors.size());

        for (auto &i : _colors) {
            c.push_back(i.color);
            h.push_back(i.threshold);
        }
        shader.SetVec3("colors", c);
        shader.SetFloat("heights", h);
        shader.SetInt("heightLevels", _colors.size());
    }
}

template <class Archive>
void
rpgMaterialFlatNoise::serialize(Archive &ar)
{
    try {
        ar(cereal::base_class<jleMaterial>(this), CEREAL_NVP(_colors));
    } catch (std::exception &e) {
        LOGE << "Failed loading material:" << e.what();
    }
}
