#include "launcher_rendering.h"

#include <string.h>

namespace MainLauncher
{
    static char textMGGames[] = "MG GAMES";

    LauncherRendering LauncherRenderingSetup(CameraRect* screenCamera)
    {
        LauncherRendering launcherRendering = {};

        // Image loading
        launcherRendering.imagesSize = (int)LauncherImages::MAX;
        launcherRendering.newImages = new Image[launcherRendering.imagesSize];
        launcherRendering.newImages[(int)LauncherImages::Launcher] = LoadImageFromFile("Assets/Sprites/Launcher.png");
        launcherRendering.newImages[(int)LauncherImages::Font16] = LoadImageFromFile("Assets/Fonts/Font16.png");
        launcherRendering.newImages[(int)LauncherImages::Font32] = LoadImageFromFile("Assets/Fonts/Font32.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonA] = LoadImageFromFile("Assets/Sprites/ButtonA.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonB] = LoadImageFromFile("Assets/Sprites/ButtonB.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonJ] = LoadImageFromFile("Assets/Sprites/ButtonJ.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonK] = LoadImageFromFile("Assets/Sprites/ButtonK.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonUp] = LoadImageFromFile("Assets/Sprites/ButtonUp.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonDown] = LoadImageFromFile("Assets/Sprites/ButtonDown.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonMenuUp] = LoadImageFromFile("Assets/Sprites/ButtonMenuUp.png");
        launcherRendering.newImages[(int)LauncherImages::ButtonMenuDown] = LoadImageFromFile("Assets/Sprites/ButtonMenuDown.png");
        launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowUp] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowUp.png");
        launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowDown] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowDown.png");
        launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowLeft] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowLeft.png");
        launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowRight] = LoadImageFromFile("Assets/Sprites/DpadButtonArrowRight.png");
        launcherRendering.newImages[(int)LauncherImages::DpadButtonUp] = LoadImageFromFile("Assets/Sprites/DpadButtonUp.png");
        launcherRendering.newImages[(int)LauncherImages::DpadButtonDown] = LoadImageFromFile("Assets/Sprites/DpadButtonDown.png");

        // Sprites
        launcherRendering.spriteSize = 14;
        launcherRendering.sprites = new Sprite[launcherRendering.spriteSize];
        launcherRendering.sprites[0] = 
            Sprite(0, 0, 0, &launcherRendering.newImages[(int)LauncherImages::Launcher], screenCamera);

        // A/B buttons
        launcherRendering.sprites[1] = 
            Sprite(134, 204, 0, &launcherRendering.newImages[(int)LauncherImages::ButtonUp], screenCamera);
        launcherRendering.sprites[2] = 
            Sprite(134, 206, 1, &launcherRendering.newImages[(int)LauncherImages::ButtonJ], screenCamera);
        launcherRendering.sprites[3] =
            Sprite(161, 182, 0, &launcherRendering.newImages[(int)LauncherImages::ButtonUp], screenCamera);
        launcherRendering.sprites[4] = 
            Sprite(161, 184, 1, &launcherRendering.newImages[(int)LauncherImages::ButtonK], screenCamera);

        // Dpad
        launcherRendering.sprites[5] = 
            Sprite(55, 182, 0, &launcherRendering.newImages[(int)LauncherImages::DpadButtonUp], screenCamera);
        launcherRendering.sprites[6] =
            Sprite(55, 182, 1, &launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowUp], screenCamera);

        launcherRendering.sprites[7] = 
            Sprite(55, 214, 0, &launcherRendering.newImages[(int)LauncherImages::DpadButtonUp], screenCamera);
        launcherRendering.sprites[8] = 
            Sprite(55, 214, 1, &launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowDown], screenCamera);

        launcherRendering.sprites[9] = 
            Sprite(30, 198, 0, &launcherRendering.newImages[(int)LauncherImages::DpadButtonUp], screenCamera);
        launcherRendering.sprites[10] = 
            Sprite(30, 198, 1, &launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowLeft], screenCamera);

        launcherRendering.sprites[11] = 
            Sprite(30 + 32 + 18, 198, 0, &launcherRendering.newImages[(int)LauncherImages::DpadButtonUp], screenCamera);
        launcherRendering.sprites[12] = 
            Sprite(30 + 32 + 18, 198, 1, &launcherRendering.newImages[(int)LauncherImages::DpadButtonArrowRight], screenCamera);

        // Menu button
        launcherRendering.sprites[13] = 
            Sprite(125, 7, 0, &launcherRendering.newImages[(int)LauncherImages::ButtonMenuUp], screenCamera);

        // Intro
        launcherRendering.introImage = new Image();
        *launcherRendering.introImage = LoadImageFromFile("Assets/Sprites/LogoStartup.png");

        launcherRendering.introAnimation = new SpriteAnimated();
        *launcherRendering.introAnimation = 
            SpriteAnimated(32, 32, 160, 144, 1, launcherRendering.introImage, screenCamera);
        LoadAnimation(launcherRendering.introAnimation, "Assets/AnimFiles/Intro.anim");

        // Text
        Color greyColor = Color(88, 88, 88);
        Color darkGreyColor = Color(100, 100, 100);

        launcherRendering.textSize = 1;
        launcherRendering.texts = new Text[launcherRendering.textSize];
        launcherRendering.texts[0] = Text(142, 233, 6, 6, (unsigned)strlen(textMGGames), textMGGames, 16, 3,
            darkGreyColor, &launcherRendering.newImages[(int)LauncherImages::Font16], screenCamera);

        return launcherRendering;
    }

    void LauncherRenderIntro(ScreenData* screenData, int screenSize, LauncherRendering* launcherRendering, float dt)
    {
        UpdateSpriteAnimated(launcherRendering->introAnimation, dt);
        RenderSpriteAnimated(screenData, *launcherRendering->introAnimation, screenSize);
    }

    void LauncherRender(ScreenData* screenData, int screenSize, LauncherRendering* launcherRendering, float dt)
    {
        for(int i = 0; i < launcherRendering->spriteSize; i++)
        {
            RenderSprite(screenData, launcherRendering->sprites[i], screenSize);
        }

        for(int i = 0; i < launcherRendering->textSize; i++)
        {
            RenderText(screenData, launcherRendering->texts[i], screenSize);
        }
    }

    void LauncherUpdateUI(InputData* inputData, LauncherRendering* launcherRendering)
    {
        // Update buttons if input type changes
        if(inputData->inputType == InputType::Controller)
        {
            launcherRendering->sprites[2].image = &launcherRendering->newImages[(int)LauncherImages::ButtonA];
            launcherRendering->sprites[4].image = &launcherRendering->newImages[(int)LauncherImages::ButtonB];
        }
        else
        {
            launcherRendering->sprites[2].image = &launcherRendering->newImages[(int)LauncherImages::ButtonJ];
            launcherRendering->sprites[4].image = &launcherRendering->newImages[(int)LauncherImages::ButtonK];
        }

        // Left
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::AKey] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::DPAD_LEFT] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::L_THUMB_LEFT] == ButtonState::Held)
        {
            launcherRendering->sprites[9].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonDown];
            launcherRendering->sprites[10].y = 200;
        }
        else
        {
            launcherRendering->sprites[9].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonUp];
            launcherRendering->sprites[10].y = 198;
        }

        // Right
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::DKey] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::DPAD_RIGHT] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::L_THUMB_RIGHT] == ButtonState::Held)
        {
            launcherRendering->sprites[11].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonDown];
            launcherRendering->sprites[12].y = 200;
        }
        else
        {
            launcherRendering->sprites[11].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonUp];
            launcherRendering->sprites[12].y = 198;
        }

        // Up
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::WKey] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::DPAD_UP] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::L_THUMB_UP] == ButtonState::Held)
        {
            launcherRendering->sprites[5].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonDown];
            launcherRendering->sprites[6].y = 184;
        }
        else
        {
            launcherRendering->sprites[5].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonUp];
            launcherRendering->sprites[6].y = 182;
        }

        // Down
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::SKey] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::DPAD_DOWN] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::L_THUMB_DOWN] == ButtonState::Held)
        {
            launcherRendering->sprites[7].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonDown];
            launcherRendering->sprites[8].y = 216;
        }
        else
        {
            launcherRendering->sprites[7].image = &launcherRendering->newImages[(int)LauncherImages::DpadButtonUp];
            launcherRendering->sprites[8].y = 214;
        }

        // Action
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::JKey] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::A] == ButtonState::Held)
        {
            launcherRendering->sprites[1].image = &launcherRendering->newImages[(int)LauncherImages::ButtonDown];
            launcherRendering->sprites[2].y = 204;
        }
        else
        {
            launcherRendering->sprites[1].image = &launcherRendering->newImages[(int)LauncherImages::ButtonUp];
            launcherRendering->sprites[2].y = 202;
        }

        // Back
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::KKey] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::B] == ButtonState::Held)
        {
            launcherRendering->sprites[3].image = &launcherRendering->newImages[(int)LauncherImages::ButtonDown];
            launcherRendering->sprites[4].y = 182;
        }
        else
        {
            launcherRendering->sprites[3].image = &launcherRendering->newImages[(int)LauncherImages::ButtonUp];
            launcherRendering->sprites[4].y = 180;
        }

        // Menu
        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::Escape] == ButtonState::Held ||
            inputData->controller.gamepadStates[(int)ControllerButtonCodes::START] == ButtonState::Held)
        {
            launcherRendering->sprites[13].image = &launcherRendering->newImages[(int)LauncherImages::ButtonMenuDown];
        }
        else
        {
            launcherRendering->sprites[13].image = &launcherRendering->newImages[(int)LauncherImages::ButtonMenuUp];
        }
    }
}