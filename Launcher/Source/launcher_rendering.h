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
