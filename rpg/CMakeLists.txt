# *********************************************************************************************
# *               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
# *********************************************************************************************

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

set(JLE_ENGINE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
cmake_path(GET JLE_ENGINE_PATH PARENT_PATH enginePath)
set(JLE_ENGINE_PATH "${enginePath}/jle/engine")

message("The absolute engine path is \"${JLE_ENGINE_PATH}\"")

include(${JLE_ENGINE_PATH}/cmake/game1.cmake)

project(${JLE_GAME_BUILD})

add_executable(${JLE_GAME_BUILD}
        "main.cpp"
        "rpgGame.cpp"
        "cTerrain.cpp"
        "rpgMaterialFlatNoise.cpp"
        "rpgMaterialFlatNoiseTexture.cpp"
        "cRpgPlayer.cpp"
        "cRpgPlayerCamera.cpp"
        "rpgSceneServer.cpp")

include(${JLE_ENGINE_PATH}/cmake/game2.cmake)