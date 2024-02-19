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

option(RPG_BUILD_SERVER "Build the game server" OFF)
option(RPG_BUILD_IN_PROC_SERVER "Build the game server in-proc" OFF)

if(RPG_BUILD_SERVER)
    set(JLE_BUILD_HEADLESS ON)
    set(JLE_BUILD_EDITOR OFF)
endif()

configure_file(rpgBuildConfig.in.h buildConfig/rpgBuildConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/buildConfig)

project(${JLE_GAME_BUILD})

add_executable(${JLE_GAME_BUILD}
        "main.cpp"
        "cEmptyComponentTemplateClass.cpp"
        "rpgGame.cpp"
        "cTerrain.cpp"
        "rpgMaterialFlatNoise.cpp"
        "rpgMaterialFlatNoiseTexture.cpp"
        "cRpgPlayer.cpp"
        "cRpgPlayerCamera.cpp"
        "rpgSceneServer.cpp"
        "cPathFollower.cpp")

include(${JLE_ENGINE_PATH}/cmake/game2.cmake)