#pragma once
#ifndef EMULATOR_RENDERING_CODE
#define EMULATOR_RENDERING_CODE

#include "../drawing.h"

enum class EmulatorImages : int
{
    Emulator = 0,
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

struct EmulatorRendering
{
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

EmulatorRendering EmulatorRenderingSetup(CameraRect* screenCamera);
void EmulatorRender(ScreenData* screenData, int screenSize, EmulatorRendering* emuRe, float dt);
void EmulatorRenderIntro(ScreenData* screenData, int screenSize, EmulatorRendering* emuRe, float dt);

#endif
