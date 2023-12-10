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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include <iostream>

#include "window_data.h"
#include "sound_data.h"
#include "input_data.h"
#include "launcher_data.h"

#include "Drawing/cameras.cpp"

using namespace MainLauncher;

// Global engine data
WindowData windowData = {};
SoundData soundData = {};
InputData inputData = {};
LauncherData launcherData = {};

// GameDLL
typedef void(__stdcall* OnGameStart)(SoundData* soundData, CameraRect* mainCamera);
typedef void(__stdcall* OnGameUpdate)(InputData* inputData, SoundData* soundData, CameraRect* mainCamera, float deltaTime);
typedef void(__stdcall* OnGameRender)(CameraRect* mainCamera, ScreenData* sD, int screenSize);
typedef void(__stdcall* OnGameEnd)(SoundData* soundData, CameraRect* mainCamera);

OnGameStart gameStart;
OnGameUpdate gameUpdate;
OnGameRender gameRender;
OnGameEnd gameEnd;

// Win32 messages for input, closing window etc
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_PAINT:
        break;

        case WM_DESTROY:
        windowData.isRunning = false;
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int LoadDLL(LPCWSTR dllName)
{
    HINSTANCE gameDLL = LoadLibrary(dllName);
    if(gameDLL == NULL)
    {
        printf("cannot load game .dll file\n");
        system("pause");
        return 1;
    }
    else
    {
        gameStart = (OnGameStart)GetProcAddress(gameDLL, "OnGameStart");
        if(!gameStart)
        {
            printf("could not locate the gameStart function\n");
            system("pause");
            return 1;
        }

        gameUpdate = (OnGameUpdate)GetProcAddress(gameDLL, "OnGameUpdate");
        if(!gameUpdate)
        {
            printf("could not locate the gameUpdate function\n");
            system("pause");
            return 1;
        }

        gameRender = (OnGameRender)GetProcAddress(gameDLL, "OnGameRender");
        if(!gameRender)
        {
            printf("could not locate the gameRender function\n");
            system("pause");
            return 1;
        }

        gameEnd = (OnGameEnd)GetProcAddress(gameDLL, "OnGameEnd");
        if(!gameEnd)
        {
            printf("could not locate the gameEnd function\n");
            system("pause");
            return 1;
        }
    }

    return 0;
}

// Main for launcher
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    printf("starting program \n");

#ifdef _DEBUG
    // Setup console
    {
        AllocConsole();
        auto consoleFile0 = freopen("CONIN$", "r", stdin);
        auto consoleFile1 = freopen("CONOUT$", "w", stdout);
        auto consoleFile2 = freopen("CONOUT$", "w", stderr);
    }
#endif

    // Win32 window settings
    WNDCLASSW wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"Pixels";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    windowData.screenData.nativeWidth = 224;
    windowData.screenData.nativeHeight = 248;

    // Get window size including border
    int borderSizeWidth = 16;
    int borderSizeHeight = 39;
    int actualScreenWidth = (windowData.screenData.nativeWidth + borderSizeWidth) * windowData.scaleWindow;
    int actualScreenHeight = (windowData.screenData.nativeHeight + borderSizeHeight) * windowData.scaleWindow;

    // Get monitor resolution
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    RegisterClassW(&wc);
    windowData.window = CreateWindowW(wc.lpszClassName, L"Square Engine by MGGames",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        (width / 2) - (actualScreenWidth / 2), (height / 2) - (actualScreenHeight / 2), 
        actualScreenWidth, actualScreenHeight, NULL, NULL, hInstance, NULL);

    // Get size of window for rendering
    RECT rect;
    GetClientRect(windowData.window, &rect);
    windowData.screenData.targetWidth = rect.right - rect.left;
    windowData.screenData.targetHeight = rect.bottom - rect.top;

    windowData.screenData.memory = VirtualAlloc(0,
         windowData.screenData.nativeWidth * windowData.screenData.nativeHeight * 4,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    printf("creating window %d, %d \n", windowData.screenData.nativeWidth, windowData.screenData.nativeHeight);

    windowData.isRunning = true;

    // Setup rendering settings
    windowData.screenData.sortDepthBuffer = 
        new int[windowData.screenData.nativeWidth * windowData.screenData.nativeHeight];
    windowData.screenData.bitmap_info.bmiHeader.biSize = sizeof(windowData.screenData.bitmap_info.bmiHeader);
    windowData.screenData.bitmap_info.bmiHeader.biWidth = windowData.screenData.nativeWidth;
    windowData.screenData.bitmap_info.bmiHeader.biHeight = windowData.screenData.nativeHeight;
    windowData.screenData.bitmap_info.bmiHeader.biPlanes = 1;
    windowData.screenData.bitmap_info.bmiHeader.biBitCount = 32;
    windowData.screenData.bitmap_info.bmiHeader.biCompression = BI_RGB;
    windowData.hdc = GetDC(windowData.window);

    // Cameras
    windowData.camerasSize = 2;
    windowData.cameras = new CameraRect[windowData.camerasSize];
    windowData.cameras[CameraTypes::Launcher] = 
        CameraRect(0, 0, windowData.screenData.nativeWidth, windowData.screenData.nativeHeight, false);
    windowData.cameras[CameraTypes::Gameplay] = 
        CameraRect(32, 32, 160, 144, true);

    // Setup a few games
    launcherData.gameLaunchData[launcherData.gameLaunchDataCount++] =
        GameLaunchData(L"SantaGame.dll", "Assets/Sprites/SantaGameCartridge.png");
    launcherData.gameLaunchData[launcherData.gameLaunchDataCount++] =
        GameLaunchData(L"SantaGame.dll", "Assets/Sprites/ComingSoonCartridge.png");

    launcherData.rendering = LauncherRenderingSetup(&windowData.cameras[CameraTypes::Launcher], &launcherData);

    printf("emulator render setup \n");

    // Setup time
    srand((unsigned int)time(NULL));
    windowData.startTime = (float)clock();

    // Setup audio
    soundData.result = ma_engine_init(NULL, &soundData.engine);
    if(soundData.result != MA_SUCCESS)
    {
        printf("failed to load mini audio\n");
        return -1;
    }

    printf("finished window setup \n");

    while(windowData.isRunning)
    {
        windowData.deltaTime = ((float)clock() - windowData.currentTicks) / 1000.0f;
        windowData.currentTicks = (float)clock();

        // Input updates
        {
            while(PeekMessage(&windowData.msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&windowData.msg);
                DispatchMessage(&windowData.msg);
            }

            if(UpdateControllerStates(&inputData.controller))
            {
                inputData.inputType = InputType::Controller;
            }

            if(UpdateKeyboardStates(&inputData.keyboardMouse))
            {
                inputData.inputType = InputType::KeyboardMouse;
            }

            // Input checks to change UI icons
            LauncherUpdateUI(&inputData, &launcherData.rendering);
        }

        DebugCameraMovement(&inputData, &windowData);

        // Update launcher or game (DLL)
        switch(launcherData.state)
        {
            case LauncherState::Intro:
            {
                int currentAnimId = launcherData.rendering.introAnimation->currentAnimationID;

                if(launcherData.rendering.introAnimation->currentTime ==
                    launcherData.rendering.introAnimation->animations[currentAnimId].animationTime)
                {
                    launcherData.state = LauncherState::GameChooser;
                }
            }
            break;

            case LauncherState::GameChooser:
            {
                LauncherUpdateGameChooser(&inputData, &launcherData, launcherData.gameLaunchDataCount, windowData.deltaTime);

                if(launcherData.currentSelectedGame != -1)
                {
                    if(LoadDLL(launcherData.gameLaunchData[launcherData.currentSelectedGame].dllName))
                    {
                        return -1;
                    }

                    launcherData.state = LauncherState::GameStart;
                }
            }
            break;

            case LauncherState::GameStart:
            {
                gameStart(&soundData, &windowData.cameras[CameraTypes::Gameplay]);
                launcherData.state = LauncherState::GameUpdate;
            }
            break;

            case LauncherState::GameUpdate:
            {
                gameUpdate(&inputData, &soundData, &windowData.cameras[CameraTypes::Gameplay], windowData.deltaTime);
            }
            break;
        }

        // Rendering
        {
            auto* screenData = &windowData.screenData;

            // Clear backbuffer/screen pixels
            memset(screenData->memory, 0, screenData->nativeWidth * screenData->nativeHeight * 4);
            memset(screenData->sortDepthBuffer, 0, (screenData->nativeWidth * screenData->nativeHeight) * sizeof(int));

            int screenSize = screenData->nativeWidth * screenData->nativeHeight * 4;

            // Draw launcher (retro console)
            LauncherRender(screenData, screenSize, &launcherData.rendering, windowData.deltaTime);

            // Draw intro animation or game
            switch(launcherData.state)
            {
                case LauncherState::Intro:
                {
                    LauncherRenderIntro(screenData, screenSize, &launcherData.rendering, windowData.deltaTime);
                }
                break;

                case LauncherState::GameChooser:
                {
                    LauncherRenderGameChooser(screenData, screenSize, &launcherData);
                }
                break;

                case LauncherState::GameUpdate:
                {
                    gameRender(&windowData.cameras[CameraTypes::Gameplay], screenData, screenSize);
                }
                break;
            }

            StretchDIBits(windowData.hdc,
                0,
                screenData->targetHeight,
                screenData->targetWidth,
                -screenData->targetHeight,
                0,
                0,
                screenData->nativeWidth,
                screenData->nativeHeight,
                screenData->memory,
                &screenData->bitmap_info,
                DIB_RGB_COLORS,
                SRCCOPY
            );
        }

        if(windowData.deltaTime > 0)
        {
            windowData.fps = CLOCKS_PER_SEC / windowData.deltaTime;
        }
    }

    // Clean up any resources etc
    gameEnd(&soundData, &windowData.cameras[CameraTypes::Gameplay]);

    //ma_engine_uninit(&soundData.engine);

    for(int i = 0; i < launcherData.rendering.imagesSize; i++)
    {
        DestroyImageFromFile(&launcherData.rendering.newImages[i]);
    }

    delete[] launcherData.rendering.sprites;
    delete[] launcherData.rendering.newImages;
    delete[] launcherData.rendering.launchGameImages;

    delete[]  windowData.screenData.sortDepthBuffer;

    srand((unsigned int)time(NULL));
    return 0;
}