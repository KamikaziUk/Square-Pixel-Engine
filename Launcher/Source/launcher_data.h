#pragma once
#ifndef LAUNCHER_DATA
#define LAUNCHER_DATA

#include "launcher_rendering.h"

enum class LauncherState
{
    Intro,
    GameStart,
    GameUpdate
};

struct LauncherData
{
    LauncherData()
    {
        state = LauncherState::Intro;
        rendering = {};
    }

    LauncherState state;
    LauncherRendering rendering;
};

#endif