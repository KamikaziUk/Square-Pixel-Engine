#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include <iostream>

#include "window_data.h"
#include "sound_data.h"

#include "drawing.h"
#include "input_data.h"
#include "Emulator/emulator_rendering.h"
#include "Emulator/emulator_input.h"

// Global engine data
WindowData windowData = {};
SoundData soundData = {};
InputData inputData = {};

// GameDLL
typedef void(__stdcall* OnGameStart)(SoundData* soundData, CameraRect* mainCamera);
typedef void(__stdcall* OnGameUpdate)(InputData* inputData, SoundData* soundData, CameraRect* mainCamera, float deltaTime);
typedef void(__stdcall* OnGameRender)(CameraRect* mainCamera, ScreenData* sD, int screenSize);
typedef void(__stdcall* OnGameEnd)(SoundData* soundData, CameraRect* mainCamera);

OnGameStart gameStart;
OnGameUpdate gameUpdate;
OnGameRender gameRender;
OnGameEnd gameEnd;

// Emulator data
enum EmulatorState
{
    Intro,
    SantaGameStart,
    SantaGameUpdate
};

EmulatorState emuState = EmulatorState::Intro;
EmulatorRendering emuRe = {};

// Render cameras
int camerasSize = 0;
CameraRect* cameras = nullptr;
float gameCameraXSpeed = 0;
float gameCameraYSpeed = 0;

void CameraMovement()
{
    // Camera movement
    const float moveSpeed = 0.1f;

    if(inputData.keyboardMouse.keyStates[(int)KeyCodes::ArrowLeft] == ButtonState::Held)
    {
        gameCameraXSpeed = -moveSpeed;
    }
    else if(inputData.keyboardMouse.keyStates[(int)KeyCodes::ArrowRight] == ButtonState::Held)
    {
        gameCameraXSpeed = moveSpeed;
    }
    else
    {
        gameCameraXSpeed = 0;
    }

    if(inputData.keyboardMouse.keyStates[(int)KeyCodes::ArrowUp] == ButtonState::Held)
    {
        gameCameraYSpeed = moveSpeed;
    }
    else if(inputData.keyboardMouse.keyStates[(int)KeyCodes::ArrowDown] == ButtonState::Held)
    {
        gameCameraYSpeed = -moveSpeed;
    }
    else
    {
        gameCameraYSpeed = 0;
    }
}

void RenderToScreen(HWND hwnd, HDC hdc, float dt)
{
    auto* screenData = &windowData.screenData;

    memset(screenData->memory, 0, screenData->nativeWidth * screenData->nativeHeight * 4);
    memset(screenData->sortDepthBuffer, 0, (screenData->nativeWidth * screenData->nativeHeight) * sizeof(int));

    int screenSize = screenData->nativeWidth * screenData->nativeHeight * 4;

    EmulatorRender(screenData, screenSize, &emuRe, dt);

    if(emuState == EmulatorState::SantaGameUpdate)
    {
        gameRender(&cameras[1], screenData, screenSize);
    }
    else if(emuState == EmulatorState::Intro)
    {
        EmulatorRenderIntro(screenData, screenSize, &emuRe, dt);
    }

    StretchDIBits(hdc,
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

void MainLoop()
{
    windowData.dt = (clock() - windowData.currentTicks) / 1000.0f;
    windowData.currentTicks = clock();

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
  
    UpdateEmulatorUI(&inputData, &emuRe);

    CameraMovement();

    cameras[1].positionX += gameCameraXSpeed;
    cameras[1].positionY += gameCameraYSpeed;

    if(emuState == EmulatorState::SantaGameStart)
    {
        gameStart(&soundData, &cameras[1]);

        emuState = EmulatorState::SantaGameUpdate;
    }
    else if(emuState == EmulatorState::SantaGameUpdate)
    {
        gameUpdate(&inputData, &soundData, &cameras[1], windowData.dt);
    }
    else if(emuState == EmulatorState::Intro)
    {
        if(emuRe.introAnimation->currentTime == emuRe.introAnimation->animations[emuRe.introAnimation->currentAnimationID].animationTime)
        {
            emuState = EmulatorState::SantaGameStart;
        }
    }

    RenderToScreen(windowData.window, windowData.hdc, windowData.dt);

    cs_mix(soundData.ctx);

    windowData.deltaTicks = clock() - windowData.currentTicks; //the time, in ms, that took to render the scene

    if(windowData.deltaTicks > 0)
    {
        windowData.fps = CLOCKS_PER_SEC / windowData.deltaTicks;
    }
}

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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    printf("starting program \n");

#ifdef _DEBUG
    // Setup console
    {
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif

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

    WNDCLASSW wc = { 0 };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"Pixels";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

     windowData.screenData.nativeWidth = 224;
     windowData.screenData.nativeHeight = 248;

    int borderSizeWidth = 16;
    int borderSizeHeight = 39;

    int actualScreenWidth = (windowData.screenData.nativeWidth + borderSizeWidth) * windowData.scaleWindow;
    int actualScreenHeight = (windowData.screenData.nativeHeight + borderSizeHeight) * windowData.scaleWindow;

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    RegisterClassW(&wc);
    windowData.window = CreateWindowW(wc.lpszClassName, L"MG Games Collection",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        (width / 2) - (actualScreenWidth / 2), (height / 2) - (actualScreenHeight / 2), actualScreenWidth, actualScreenHeight, NULL, NULL, hInstance, NULL);

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

     windowData.screenData.sortDepthBuffer = new int[windowData.screenData.nativeWidth * windowData.screenData.nativeHeight];
     windowData.screenData.bitmap_info.bmiHeader.biSize = sizeof(windowData.screenData.bitmap_info.bmiHeader);
     windowData.screenData.bitmap_info.bmiHeader.biWidth = windowData.screenData.nativeWidth;
     windowData.screenData.bitmap_info.bmiHeader.biHeight = windowData.screenData.nativeHeight;
     windowData.screenData.bitmap_info.bmiHeader.biPlanes = 1;
     windowData.screenData.bitmap_info.bmiHeader.biBitCount = 32;
     windowData.screenData.bitmap_info.bmiHeader.biCompression = BI_RGB;

    // Cameras
    camerasSize = 2;
    cameras = new CameraRect[camerasSize];
    cameras[0] = CameraRect(0, 0, windowData.screenData.nativeWidth, windowData.screenData.nativeHeight, false);
    cameras[1] = CameraRect(32, 32, 160, 144, true);

    emuRe = EmulatorRenderingSetup(&cameras[0]);

    printf("emulator render setup \n");

    srand(time(NULL));

    windowData.hdc = GetDC(windowData.window);

    windowData.begin = clock();

    soundData.ctx = cs_make_context(windowData.window, 44100, 8192, 0, NULL);

    printf("finished window setup \n");

    while(windowData.isRunning)
    {
        MainLoop();
    }

    gameEnd(&soundData, &cameras[1]);

    cs_shutdown_context(soundData.ctx);

    for(int i = 0; i < emuRe.imagesSize; i++)
    {
        DestroyImageFromFile(&emuRe.newImages[i]);
    }

    delete[] emuRe.sprites;
    delete[] emuRe.newImages;

    delete[]  windowData.screenData.sortDepthBuffer;

    srand((unsigned int)time(NULL));

    return 0;
}