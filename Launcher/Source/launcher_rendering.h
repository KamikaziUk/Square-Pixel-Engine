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
#ifndef LAUNCHER_RENDERING
#define LAUNCHER_RENDERING

#include "input_data.h"

#include "Drawing/drawing.h"

using namespace SquarePixelEngine;

namespace MainLauncher
{
    enum class LauncherImages : int
    {
        Launcher = 0,
        Font16,
        Font32,
        ButtonA,
        ButtonB,
        ButtonJ,
        ButtonK,
        ButtonUp,
        ButtonDown,
        ButtonMenuUp,
        ButtonMenuDown,
        DpadButtonArrowUp,
        DpadButtonArrowDown,
        DpadButtonArrowLeft,
        DpadButtonArrowRight,
        DpadButtonUp,
        DpadButtonDown,
        MAX
    };

    struct LauncherRendering
    {
        LauncherRendering()
        {
            imagesSize = 0;
            newImages = nullptr;
            spriteSize = 0;
            sprites = nullptr;
            textSize = 0;
            texts = nullptr;
            introImage = nullptr;
            introAnimation = nullptr;
            santaCartridgeImage = nullptr;
            santaCartridge = nullptr;
            isIntro = false;
        }

        int imagesSize;
        Image* newImages;

        int spriteSize;
        Sprite* sprites;

        int textSize;
        Text* texts;

        Image* introImage;
        SpriteAnimated* introAnimation;

        Image* santaCartridgeImage;
        Sprite* santaCartridge;

        bool isIntro;
    };

    LauncherRendering LauncherRenderingSetup(CameraRect* screenCamera);
    void LauncherUpdateUI(InputData* inputData, LauncherRendering* launcherRendering);
    void LauncherRender(ScreenData* screenData, int screenSize, LauncherRendering* launcherRendering, float deltaTime);
    void LauncherRenderIntro(ScreenData* screenData, int screenSize, LauncherRendering* launcherRendering, float deltaTime);
}

#endif
