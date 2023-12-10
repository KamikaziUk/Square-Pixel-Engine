// Copyright (c) Marty Green 2023
// https://github.com/KamikaziUk

// MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef LAUNCHER_DATA
#define LAUNCHER_DATA

#include "launcher_rendering.h"

using namespace SquarePixelEngine;

namespace MainLauncher
{
    enum class LauncherState
    {
        Intro,
        GameChooser,
        GameStart,
        GameUpdate
    };

    struct GameLaunchData
    {
        GameLaunchData()
        {
            dllName = nullptr;
            cartridgeImageName = nullptr;
        }

        GameLaunchData(LPCWSTR dllName, const char* cartridgeImageName)
        {
            this->dllName = dllName;
            this->cartridgeImageName = cartridgeImageName;
        }

        LPCWSTR dllName;
        const char* cartridgeImageName;
    };

    struct LauncherData
    {
        #define LaunchPoolSize 8

        LauncherData()
        {
            state = LauncherState::Intro;
            rendering = {};
            currentSelectedGame = -1;
            currentHighlightedGame = 0;
            memset(&gameLaunchData, 0, sizeof(GameLaunchData) * LaunchPoolSize);
            gameLaunchDataCount = 0;
        }

        int currentSelectedGame;
        int currentHighlightedGame;

        GameLaunchData gameLaunchData[LaunchPoolSize];
        int gameLaunchDataCount;

        LauncherState state;
        LauncherRendering rendering;
    };
}

#endif