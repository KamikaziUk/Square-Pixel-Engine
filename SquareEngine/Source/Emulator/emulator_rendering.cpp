#include "emulator_rendering.h"

#include <string.h>

static char textMGGames[] = "MG GAMES";

EmulatorRendering EmulatorRenderingSetup(CameraRect* screenCamera)
{
    // Emulator rendering setup
    EmulatorRendering emuRe = {};

    // Image loading
    emuRe.imagesSize = (int)EmulatorImages::MAX;
    emuRe.newImages = new Image[emuRe.imagesSize];
    emuRe.newImages[(int)EmulatorImages::Emulator] = LoadImageFromFile("Assets/Sprites/Emulator.png");
    emuRe.newImages[(int)EmulatorImages::Font16] = LoadImageFromFile("Assets/Fonts/Font16.png");
    emuRe.newImages[(int)EmulatorImages::Font32] = LoadImageFromFile("Assets/Fonts/Font32.png");
    emuRe.newImages[(int)EmulatorImages::ButtonA] = LoadImageFromFile("Assets/Sprites/ButtonA.png");
    emuRe.newImages[(int)EmulatorImages::ButtonB] = LoadImageFromFile("Assets/Sprites/ButtonB.png");
    emuRe.newImages[(int)EmulatorImages::ButtonJ] = LoadImageFromFile("Assets/Sprites/ButtonJ.png");
    emuRe.newImages[(int)EmulatorImages::ButtonK] = LoadImageFromFile("Assets/Sprites/ButtonK.png");
    emuRe.newImages[(int)EmulatorImages::ButtonUp] = LoadImageFromFile("Assets/Sprites/ButtonUp.png");
    emuRe.newImages[(int)EmulatorImages::ButtonDown] = LoadImageFromFile("Assets/Sprites/ButtonDown.png");
    emuRe.newImages[(int)EmulatorImages::ButtonMenuUp] = LoadImageFromFile("Assets/Sprites/ButtonMenuUp.png");
    emuRe.newImages[(int)EmulatorImages::ButtonMenuDown] = LoadImageFromFile("Assets/Sprites/ButtonMenuDown.png");
    emuRe.newImages[(int)EmulatorImages::DpadButtonArrowUp] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowUp.png");
    emuRe.newImages[(int)EmulatorImages::DpadButtonArrowDown] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowDown.png");
    emuRe.newImages[(int)EmulatorImages::DpadButtonArrowLeft] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowLeft.png");
    emuRe.newImages[(int)EmulatorImages::DpadButtonArrowRight] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowRight.png");
    emuRe.newImages[(int)EmulatorImages::DpadButtonUp] = LoadImageFromFile("Assets/Sprites/DpadButtonUp.png");
    emuRe.newImages[(int)EmulatorImages::DpadButtonDown] = LoadImageFromFile("Assets/Sprites/DpadButtonDown.png");

    // Sprites
    emuRe.spriteSize = 14;
    emuRe.sprites = new Sprite[emuRe.spriteSize];
    emuRe.sprites[0] = Sprite(0, 0, 0, &emuRe.newImages[(int)EmulatorImages::Emulator], screenCamera);

    // A/B buttons
    emuRe.sprites[1] = Sprite(134, 204, 0, &emuRe.newImages[(int)EmulatorImages::ButtonUp], screenCamera);
    emuRe.sprites[2] = Sprite(134, 206, 1, &emuRe.newImages[(int)EmulatorImages::ButtonJ], screenCamera);
    emuRe.sprites[3] = Sprite(161, 182, 0, &emuRe.newImages[(int)EmulatorImages::ButtonUp], screenCamera);
    emuRe.sprites[4] = Sprite(161, 184, 1, &emuRe.newImages[(int)EmulatorImages::ButtonK], screenCamera);

    // Dpad
    emuRe.sprites[5] = Sprite(55, 182, 0, &emuRe.newImages[(int)EmulatorImages::DpadButtonUp], screenCamera);
    emuRe.sprites[6] = Sprite(55, 182, 1, &emuRe.newImages[(int)EmulatorImages::DpadButtonArrowUp], screenCamera);

    emuRe.sprites[7] = Sprite(55, 214, 0, &emuRe.newImages[(int)EmulatorImages::DpadButtonUp], screenCamera);
    emuRe.sprites[8] = Sprite(55, 214, 1, &emuRe.newImages[(int)EmulatorImages::DpadButtonArrowDown], screenCamera);

    emuRe.sprites[9] = Sprite(30, 198, 0, &emuRe.newImages[(int)EmulatorImages::DpadButtonUp], screenCamera);
    emuRe.sprites[10] = Sprite(30, 198, 1, &emuRe.newImages[(int)EmulatorImages::DpadButtonArrowLeft], screenCamera);

    emuRe.sprites[11] = Sprite(30 + 32 + 18, 198, 0, &emuRe.newImages[(int)EmulatorImages::DpadButtonUp], screenCamera);
    emuRe.sprites[12] = Sprite(30 + 32 + 18, 198, 1, &emuRe.newImages[(int)EmulatorImages::DpadButtonArrowRight], screenCamera);

    // Menu button
    emuRe.sprites[13] = Sprite(125, 7, 0, &emuRe.newImages[(int)EmulatorImages::ButtonMenuUp], screenCamera);

    // Intro
    emuRe.introImage = new Image();
    *emuRe.introImage = LoadImageFromFile("Assets/Sprites/LogoStartup.png");

    emuRe.introAnimation = new SpriteAnimated();
    *emuRe.introAnimation = SpriteAnimated(32, 32, 160, 144, 1, emuRe.introImage, screenCamera);
    LoadAnimation(emuRe.introAnimation, "Assets/AnimFiles/Intro.anim");

    // Text
    Color greyColor = Color(88, 88, 88);
    Color darkGreyColor = Color(50, 50, 50);

    emuRe.textSize = 1;
    emuRe.texts = new Text[emuRe.textSize];
    emuRe.texts[0] = Text(146, 233, 6, 6, (unsigned)strlen(textMGGames), textMGGames, 16, 3, darkGreyColor, &emuRe.newImages[(int)EmulatorImages::Font16], screenCamera);

    return emuRe;
}

void EmulatorRenderIntro(ScreenData* screenData, int screenSize, EmulatorRendering* emuRe, float dt)
{   
    UpdateSpriteAnimated(emuRe->introAnimation, dt);
    RenderSpriteAnimated(screenData, *emuRe->introAnimation, screenSize);
}

void EmulatorRender(ScreenData* screenData, int screenSize, EmulatorRendering* emuRe, float dt)
{    
    for(int i = 0; i < emuRe->spriteSize; i++)
    {
        RenderSprite(screenData, emuRe->sprites[i], screenSize);
    }

    for(int i = 0; i < emuRe->textSize; i++)
    {
        RenderText(screenData, emuRe->texts[i], screenSize);
    }
}