#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include <iostream>

#include "window_data.h"
#include "sound_data.h"
#include "input_data.h"
#include "launcher_data.h"

#include "Drawing/cameras.cpp"
#include "Input/launcher_input.h"
#include "launcher_rendering.h"

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

    // Load DLL for game
    HINSTANCE gameDLL = LoadLibrary(L"SantaGame.dll");
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
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

    launcherData.rendering = LauncherRenderingSetup(&windowData.cameras[CameraTypes::Launcher]);

    printf("emulator render setup \n");

    // Setup time
    srand((unsigned int)time(NULL));
    windowData.startTime = (float)clock();

    // Setup audio
    soundData.ctx = cs_make_context(windowData.window, 44100, 8192, 0, NULL);

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

        // Update audio
        cs_mix(soundData.ctx);

        if(windowData.deltaTime > 0)
        {
            windowData.fps = CLOCKS_PER_SEC / windowData.deltaTime;
        }
    }

    // Clean up any resources etc
    gameEnd(&soundData, &windowData.cameras[CameraTypes::Gameplay]);

    //cs_shutdown_context(soundData.ctx);

    for(int i = 0; i < launcherData.rendering.imagesSize; i++)
    {
        DestroyImageFromFile(&launcherData.rendering.newImages[i]);
    }

    delete[] launcherData.rendering.sprites;
    delete[] launcherData.rendering.newImages;

    delete[]  windowData.screenData.sortDepthBuffer;

    srand((unsigned int)time(NULL));
    return 0;
}