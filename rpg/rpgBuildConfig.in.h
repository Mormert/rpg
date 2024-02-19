/*********************************************************************************************
*               Copyright (c) 2023-2024 Johan Lind. All rights reserved.                    *
*********************************************************************************************/

#pragma once

#include <jleBuildConfig.h>

#cmakedefine01 RPG_BUILD_SERVER
#cmakedefine01 RPG_BUILD_IN_PROC_SERVER

#if JLE_BUILD_RUNTIME_CONFIGURABLE
inline bool RPG_BUILD_SERVER_RUNTIME = RPG_BUILD_SERVER;
inline bool RPG_BUILD_IN_PROC_SERVER_RUNTIME = RPG_BUILD_IN_PROC_SERVER;
#endif