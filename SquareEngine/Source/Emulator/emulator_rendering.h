#pragma once
#ifndef EMULATOR_RENDERING
#define EMULATOR_RENDERING

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

#include "input_data.h"
#include "drawing.h"

void UpdateEmulatorUI(InputData* inputData, EmulatorRendering* emuRe);
EmulatorRendering EmulatorRenderingSetup(CameraRect* screenCamera);
void EmulatorRender(ScreenData* screenData, int screenSize, EmulatorRendering* emuRe, float dt);
void EmulatorRenderIntro(ScreenData* screenData, int screenSize, EmulatorRendering* emuRe, float dt);

#endif
