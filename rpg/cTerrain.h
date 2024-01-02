/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#pragma once

#include "jleComponent.h"
#include "jleMaterial.h"
#include "jleMesh.h"

class cTerrain : public jleComponent
{
    JLE_REGISTER_COMPONENT_TYPE(cTerrain)
public:
    explicit cTerrain(jleObject *owner = nullptr, jleScene *scene = nullptr);

    template <class Archive>
    void
    serialize(Archive &ar)
    {
        ar(CEREAL_NVP(_terrainMaterialRef));
        try {
            ar(CEREAL_NVP(_treeMeshRef), CEREAL_NVP(_terrainTreeMaterialRef));
        } catch (std::exception &e) {
        }
    }

    void start() override;

    void update(float dt) override;

    void editorUpdate(float dt) override;

    void editorGizmosRender(bool selected) override;

    void registerLua(sol::state &lua, sol::table &table) override;

    void editorInspectorImGuiRender() override;

    void generateTerrain();

private:
    std::shared_ptr<jleMesh> _terrainMesh;
    jleResourceRef<jleMaterial> _terrainMaterialRef;
    jleResourceRef<jleMaterial> _terrainTreeMaterialRef;
    jleResourceRef<jleMesh> _treeMeshRef;

    void generateTerrainMesh(std::vector<glm::vec3> &positions,
                         std::vector<glm::vec3> &normals,
                         std::vector<glm::vec2> &texCoords,
                         std::vector<glm::vec3> &tangents,
                         std::vector<glm::vec3> &bitangents,
                         std::vector<unsigned int> &indices);

    void generateTerrainHeights();

    constexpr static size_t _gridSize = 100;
    std::array<std::array<int, _gridSize>, _gridSize> _terrainHeights{};
};

JLE_EXTERN_TEMPLATE_CEREAL_H(cTerrain)

CEREAL_REGISTER_TYPE(cTerrain)
CEREAL_REGISTER_POLYMORPHIC_RELATION(jleComponent, cTerrain)
