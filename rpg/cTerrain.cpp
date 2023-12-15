// Copyright (c) 2023. Johan Lind

#include "cTerrain.h"
#include "cMesh.h"

#include "editor/jleEditorGizmos.h"
#include "jle3DRenderer.h"
#include "rpgGame.h"

#include <glm/gtc/noise.hpp>

JLE_EXTERN_TEMPLATE_CEREAL_CPP(cTerrain)

cTerrain::cTerrain(jleObject *owner, jleScene *scene) : jleComponent(owner, scene) {}

void
cTerrain::start()
{
    // generateTerrainFromWorld();
}

void
cTerrain::update(float dt)
{
    if (_terrainMesh) {
        std::shared_ptr<jleMaterial> material = _terrainMaterialRef.get();

        gEngine->renderGraph().sendMesh(
            _terrainMesh, material, getTransform().getWorldMatrix(), object()->instanceID(), false);
    }
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
cTerrain::generateTerrainFromWorld()
{
    if (gRpgGame) {
        auto &world = gRpgGame->getWorld();
        auto &grid = world.getGrid();

        _terrainMesh = std::make_shared<jleMesh>();

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        std::vector<unsigned int> indices;

        generateTerrain(positions, normals, texCoords, tangents, bitangents, indices);

        _terrainMesh->makeMesh(positions, normals, texCoords, tangents, bitangents, indices);
    }
}

void
cTerrain::editorInspectorImGuiRender()
{
#ifdef BUILD_EDITOR
    if (ImGui::Button("Generate Terrain from World")) {
        generateTerrainFromWorld();
    }
#endif
}

float
get_height(int x, int z)
{

    float scale = 0.075f;
    float heightScale = 4.0f;

    // Generate the Perlin noise for the given x/z
    float noise = glm::perlin(glm::vec2(x * scale, z * scale));

    // Scale and shift the noise from range [-1, 1] to [0, 1]
    noise = noise * 0.5f + 0.5f;

    // Scale the noise by our height scale
    noise *= heightScale;

    return (int)noise;
}

void
cTerrain::generateTerrain(std::vector<glm::vec3> &positions,
                          std::vector<glm::vec3> &normals,
                          std::vector<glm::vec2> &texCoords,
                          std::vector<glm::vec3> &tangents,
                          std::vector<glm::vec3> &bitangents,
                          std::vector<unsigned int> &indices)
{

    int gridSize = 100;

    // We'll have gridSize * gridSize vertices, and (gridSize - 1)^2 square faces.
    positions.resize(gridSize * gridSize);
    normals.resize(gridSize * gridSize);
    texCoords.resize(gridSize * gridSize);
    tangents.resize(gridSize * gridSize);
    bitangents.resize(gridSize * gridSize);

    // Generate vertices for the grid
    for (int x = 0; x < gridSize; ++x) {
        for (int z = 0; z < gridSize; ++z) {
            float height = get_height(x, z);

            if (std::rand() % 100 < 1) {
                auto scn = scene();
                auto tree = scn->spawnObjectWithName(std::string{"Tree_"} + std::to_string(x + z));
                tree->getTransform().setLocalPosition(glm::vec3{x, height, z});
                auto mat = tree->getTransform().getLocalMatrix();
                mat = glm::scale(mat, glm::vec3{4.5f});
                tree->getTransform().setLocalMatrix(mat);
                auto meshComponent = tree->addComponent<cMesh>();
                meshComponent->getMeshRef() = _treeMeshRef;
                meshComponent->getMaterialRef() = _terrainTreeMaterialRef;
            }

            positions[x * gridSize + z] = {x, height, z};
            normals[x * gridSize + z] = {0.0f, 1.0f, 0.0f}; // simplified, assuming flat terrain
            texCoords[x * gridSize + z] = {x / (gridSize - 1.0f), z / (gridSize - 1.0f)};
            tangents[x * gridSize + z] = {1.0f, 0.0f, 0.0f};   // simplified, assuming flat terrain
            bitangents[x * gridSize + z] = {0.0f, 0.0f, 1.0f}; // simplified, assuming flat terrain
        }
    }

    // Generate indices for the faces
    for (int x = 0; x < gridSize - 1; ++x) {
        for (int z = 0; z < gridSize - 1; ++z) {
            int topLeft = x * gridSize + z;
            int topRight = topLeft + 1;
            int bottomLeft = (x + 1) * gridSize + z;
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
