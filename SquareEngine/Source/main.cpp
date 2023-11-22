#include <windows.h>
#pragma comment(lib, "winmm.lib")

#define CUTE_SOUND_IMPLEMENTATION
#include "External/cute_sound.h"

#include <iostream>

#include "drawing.h"
#include "input.h"
#include "Emulator/emulator_rendering.h"
#include "Emulator/emulator_input.h"
#include "game_santa.h"

ScreenData screenData = {};
GameSanta gameSanta = {};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void RenderToScreen(HWND hwnd, HDC hdc, float dt);

MSG msg;
cs_context_t* ctx;
HWND window;
HDC hdc;

enum EmulatorState
{
    Intro,
    SantaGameStart,
    SantaGameUpdate
};

EmulatorState emuState = EmulatorState::Intro;

EmulatorRendering emuRe = {};

int camerasSize = 0;
CameraRect* cameras = nullptr;

float gameCameraXSpeed = 0;
float gameCameraYSpeed = 0;

int scaleWindow = 2;

bool isRunning = true;

XInputController controller0 = XInputController::SetupController(0);

KeyboardMouse keyboardMouse = KeyboardMouse::Setup();

void CameraMovement()
{
    // Camera movement
    const float moveSpeed = 0.1f;

    if(keyboardMouse.keyStates[(int)KeyCodes::ArrowLeft] == ButtonState::Held)
    {
        gameCameraXSpeed = -moveSpeed;
    }
    else if(keyboardMouse.keyStates[(int)KeyCodes::ArrowRight] == ButtonState::Held)
    {
        gameCameraXSpeed = moveSpeed;
    }
    else
    {
        gameCameraXSpeed = 0;
    }

    if(keyboardMouse.keyStates[(int)KeyCodes::ArrowUp] == ButtonState::Held)
    {
        gameCameraYSpeed = moveSpeed;
    }
    else if(keyboardMouse.keyStates[(int)KeyCodes::ArrowDown] == ButtonState::Held)
    {
        gameCameraYSpeed = -moveSpeed;
    }
    else
    {
        gameCameraYSpeed = 0;
    }
}

float current_ticks = 0, delta_ticks = 0;
float begin = 0;
float fps = 0;
float dt = 0;

void MainLoop()
{
    dt = (clock() - current_ticks) / 1000.0f;
    current_ticks = clock();

    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UpdateGamepadStates(&controller0, &keyboardMouse);
    UpdateKeyboardStates(&keyboardMouse);
    UpdateEmulatorUI(&controller0, &keyboardMouse, &emuRe);

    CameraMovement();

    cameras[1].positionX += gameCameraXSpeed;
    cameras[1].positionY += gameCameraYSpeed;

    if(emuState == EmulatorState::SantaGameStart)
    {
        gameSanta = {};

        OnSantaStart(&gameSanta, &cameras[1], ctx);

        emuState = EmulatorState::SantaGameUpdate;
    }
    else if(emuState == EmulatorState::SantaGameUpdate)
    {
        OnSantaUpdate(&gameSanta, &cameras[1], &controller0, &keyboardMouse, dt, ctx);
    }
    else if(emuState == EmulatorState::Intro)
    {
        if(emuRe.introAnimation->currentTime == emuRe.introAnimation->animations[emuRe.introAnimation->currentAnimationID].animationTime)
        {
            emuState = EmulatorState::SantaGameStart;
        }
    }

    RenderToScreen(window, hdc, dt);

    cs_mix(ctx);

    delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene

    if(delta_ticks > 0)
    {
        fps = CLOCKS_PER_SEC / delta_ticks;
    }
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

    WNDCLASSW wc = { 0 };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"Pixels";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    screenData.nativeWidth = 224;
    screenData.nativeHeight = 248;

    int borderSizeWidth = 16;
    int borderSizeHeight = 39;

    int actualScreenWidth = (screenData.nativeWidth + borderSizeWidth) * scaleWindow;
    int actualScreenHeight = (screenData.nativeHeight + borderSizeHeight) * scaleWindow;

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    RegisterClassW(&wc);
    window = CreateWindowW(wc.lpszClassName, L"MG Games Collection",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        (width / 2) - (actualScreenWidth / 2), (height / 2) - (actualScreenHeight / 2), actualScreenWidth, actualScreenHeight, NULL, NULL, hInstance, NULL);

    RECT rect;
    GetClientRect(window, &rect);
    screenData.targetWidth = rect.right - rect.left;
    screenData.targetHeight = rect.bottom - rect.top;

    screenData.memory = VirtualAlloc(0,
        screenData.nativeWidth * screenData.nativeHeight * 4,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    printf("creating window %d, %d \n", screenData.nativeWidth, screenData.nativeHeight);

    screenData.sortDepthBuffer = new int[screenData.nativeWidth * screenData.nativeHeight];
    screenData.bitmap_info.bmiHeader.biSize = sizeof(screenData.bitmap_info.bmiHeader);
    screenData.bitmap_info.bmiHeader.biWidth = screenData.nativeWidth;
    screenData.bitmap_info.bmiHeader.biHeight = screenData.nativeHeight;
    screenData.bitmap_info.bmiHeader.biPlanes = 1;
    screenData.bitmap_info.bmiHeader.biBitCount = 32;
    screenData.bitmap_info.bmiHeader.biCompression = BI_RGB;

    // Cameras
    camerasSize = 2;
    cameras = new CameraRect[camerasSize];
    cameras[0] = CameraRect(0, 0, screenData.nativeWidth, screenData.nativeHeight, false);
    cameras[1] = CameraRect(32, 32, 160, 144, true);

    emuRe = EmulatorRenderingSetup(&cameras[0]);

    printf("emulator render setup \n");

    srand(time(NULL));

    hdc = GetDC(window);

    begin = clock();

    ctx = cs_make_context(window, 44100, 8192, 0, NULL);

    printf("finished window setup \n");

    while(isRunning)
    {
        MainLoop();
    }

    OnSantaEnd(&gameSanta, &cameras[1], ctx);

    cs_shutdown_context(ctx);

    for(int i = 0; i < emuRe.imagesSize; i++)
    {
        DestroyImageFromFile(&emuRe.newImages[i]);
    }

    delete[] emuRe.sprites;
    delete[] emuRe.newImages;

    delete[] screenData.sortDepthBuffer;

    srand((unsigned int)time(NULL));

    return 0;
}

void RenderToScreen(HWND hwnd, HDC hdc, float dt)
{
    memset(screenData.memory, 0, screenData.nativeWidth * screenData.nativeHeight * 4);
    memset(screenData.sortDepthBuffer, 0, (screenData.nativeWidth * screenData.nativeHeight) * sizeof(int));

    int screenSize = screenData.nativeWidth * screenData.nativeHeight * 4;

    EmulatorRender(&screenData, screenSize, &emuRe, dt);

    if(emuState == EmulatorState::SantaGameUpdate)
    {
        OnSantaRender(&gameSanta, &cameras[1], &screenData, screenSize);
    }
    else if(emuState == EmulatorState::Intro)
    {
        EmulatorRenderIntro(&screenData, screenSize, &emuRe, dt);
    }

    StretchDIBits(hdc,
        0,
        screenData.targetHeight,
        screenData.targetWidth,
        -screenData.targetHeight,
        0,
        0,
        screenData.nativeWidth,
        screenData.nativeHeight,
        screenData.memory,
        &screenData.bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_PAINT:
        break;

        case WM_DESTROY:
        isRunning = false;
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}