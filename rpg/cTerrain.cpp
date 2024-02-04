/*********************************************************************************************
 *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
 *********************************************************************************************/

#include "cTerrain.h"
#include "cMesh.h"
#include "cRigidbody.h"

#include "editor/jleEditorGizmos.h"
#include "jle3DRenderer.h"
#include "rpgGame.h"

#include <glm/gtc/noise.hpp>

JLE_EXTERN_TEMPLATE_CEREAL_CPP(cTerrain)

void
cTerrain::start()
{
    generateTerrain();
}

void
cTerrain::update(float dt)
{
}

void
cTerrain::editorUpdate(float dt)
{
}

void
cTerrain::editorGizmosRender(bool selected)
{
}

void
cTerrain::registerLua(sol::state &lua, sol::table &table)
{
}

void
cTerrain::generateTerrain()
{
    generateTerrainHeights();

    auto terrainMesh = std::make_shared<jleMesh>();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<unsigned int> indices;

    generateTerrainMesh(positions, normals, texCoords, tangents, bitangents, indices);

    terrainMesh->makeMesh(positions, normals, texCoords, tangents, bitangents, indices);

    const auto meshRef = gEngine->resources().storeResource<jleMesh>(terrainMesh, {"RM:/terrainMesh.mesh"});

    const auto meshComp = object()->addComponent<cMesh>();
    meshComp->getMeshRef() = meshRef;
    meshComp->getMaterialRef() = _terrainMaterialRef;

    const auto rigidBody = object()->addComponent<cRigidbody>();
}

void
cTerrain::editorInspectorImGuiRender()
{
#ifdef JLE_BUILD_EDITOR
    if (ImGui::Button("Generate Terrain from World")) {
        generateTerrain();
    }
#endif
}

void
cTerrain::generateTerrainHeights()
{
    const auto getHeight = [](int x, int z) {
        float scale = 0.075f;
        float heightScale = 4.0f;

        // Generate the Perlin noise for the given x/z
        float noise = glm::perlin(glm::vec2(x * scale, z * scale));

        // Scale and shift the noise from range [-1, 1] to [0, 1]
        noise = noise * 0.5f + 0.5f;

        // Scale the noise by our height scale
        noise *= heightScale;

        return (int)noise;
    };

    for (int x = 0; x < _gridSize; ++x) {
        for (int z = 0; z < _gridSize; ++z) {
            _terrainHeights[x][z] = getHeight(x, z);
        }
    }
}

void
cTerrain::generateTerrainMesh(std::vector<glm::vec3> &positions,
                              std::vector<glm::vec3> &normals,
                              std::vector<glm::vec2> &texCoords,
                              std::vector<glm::vec3> &tangents,
                              std::vector<glm::vec3> &bitangents,
                              std::vector<unsigned int> &indices)
{

    int _gridSize = 100;

    // We'll have gridSize * gridSize vertices, and (gridSize - 1)^2 square faces.
    positions.resize(_gridSize * _gridSize);
    normals.resize(_gridSize * _gridSize);
    texCoords.resize(_gridSize * _gridSize);
    tangents.resize(_gridSize * _gridSize);
    bitangents.resize(_gridSize * _gridSize);

    // Generate vertices for the grid
    for (int x = 0; x < _gridSize; ++x) {
        for (int z = 0; z < _gridSize; ++z) {
            auto height = _terrainHeights[x][z];

            if (std::rand() % 100 < 1) {
                auto tree = object()->spawnChildObject<jleObject>();
                tree->setInstanceName(std::string{"Tree_"} + std::to_string(x + z));
                tree->getTransform().setLocalPosition(glm::vec3{x, height, z});
                auto mat = tree->getTransform().getLocalMatrix();
                mat = glm::scale(mat, glm::vec3{4.5f});
                tree->getTransform().setLocalMatrix(mat);
                auto meshComponent = tree->addComponent<cMesh>();
                meshComponent->getMeshRef() = _treeMeshRef;
                meshComponent->getMaterialRef() = _terrainTreeMaterialRef;
            }

            positions[x * _gridSize + z] = {x, height, z};
            normals[x * _gridSize + z] = {0.0f, 1.0f, 0.0f}; // simplified, assuming flat terrain
            texCoords[x * _gridSize + z] = {x / (_gridSize - 1.0f), z / (_gridSize - 1.0f)};
            tangents[x * _gridSize + z] = {1.0f, 0.0f, 0.0f};   // simplified, assuming flat terrain
            bitangents[x * _gridSize + z] = {0.0f, 0.0f, 1.0f}; // simplified, assuming flat terrain
        }
    }

    // Generate indices for the faces
    for (int x = 0; x < _gridSize - 1; ++x) {
        for (int z = 0; z < _gridSize - 1; ++z) {
            int topLeft = x * _gridSize + z;
            int topRight = topLeft + 1;
            int bottomLeft = (x + 1) * _gridSize + z;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}
