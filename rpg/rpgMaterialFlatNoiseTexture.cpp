/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "rpgMaterialFlatNoiseTexture.h"

JLE_EXTERN_TEMPLATE_CEREAL_CPP(rpgMaterialFlatNoiseTexture)

rpgMaterialFlatNoiseTexture::rpgMaterialFlatNoiseTexture() { _shaderRef = jleResourceRef<jleShader>("GR:/shaders/flatNoiseTexture.glsl"); }

void
rpgMaterialFlatNoiseTexture::useMaterial(const jleCamera &camera,
                                  const std::vector<jle3DRendererLight> &lights,
                                  const jle3DSettings &settings)
{
    jleMaterial::useMaterial(camera, lights, settings);

    auto &shader = *_shaderRef.get();

    shader.SetTextureSlot("uAlbedoTexture", jleTextureSlot::Albedo);

    if (_albedo.textureRef()) {
        _albedo.textureRef()->setActive(jleTextureSlot::Albedo);
        shader.SetBool("uUseAlbedoTexture", true);
    } else {
        shader.SetBool("uUseAlbedoTexture", false);
        shader.SetVec3("uAlbedo", _albedo.color());
    }
}

template <class Archive>
void
rpgMaterialFlatNoiseTexture::serialize(Archive &ar)
{
    try {
        ar(cereal::base_class<jleMaterial>(this), CEREAL_NVP(_albedo));
    } catch (std::exception &e) {
        LOGE << "Failed loading material:" << e.what();
    }
}
