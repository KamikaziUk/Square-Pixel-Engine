#pragma once
#ifndef INPUT_CODE
#define INPUT_CODE

enum class ButtonState
{
    None,
    Down,
    Held,
    Up
};

enum class InputType
{
    KeyboardMouse = 0,
    Gamepad = 1
};

struct KeyboardMouse
{
    InputType inputType;
    ButtonState keyStates[256];
    bool keyPressed[256];

    static KeyboardMouse Setup()
    {
        KeyboardMouse keyboard = {};
        return keyboard;
    }
};

// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
enum class KeyCodes 
{
    LeftMouse = 0x01,
    RightMouse = 0x02,
    MiddleMouse = 0x04,
    AKey = 0x41,
    BKey = 0x42,
    CKey = 0x43,
    DKey = 0x44,
    EKey = 0x45,
    FKey = 0x46,
    GKey = 0x47,
    HKey = 0x48,
    IKey = 0x49,
    JKey = 0x4A,
    KKey = 0x4B,
    LKey = 0x4C,
    MKey = 0x4D,
    NKey = 0x4E,
    OKey = 0x4F,
    PKey = 0x50,
    QKey = 0x51,
    RKey = 0x52,
    SKey = 0x53,
    TKey = 0x54,
    UKey = 0x55,
    VKey = 0x56,
    WKey = 0x57,
    XKey = 0x58,
    YKey = 0x59,
    ZKey = 0x5A,
    Escape = 0x1B,
    ArrowLeft = 0x25,
    ArrowUp = 0x26,
    ArrowRight = 0x27,
    ArrowDown = 0x28
};

#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

enum class ButtonCodes
{
    DPAD_UP = 0,
    DPAD_DOWN = 1,
    DPAD_LEFT = 2,
    DPAD_RIGHT = 3,
    START = 4,
    BACK = 5,
    LEFT_THUMB = 6,
    RIGHT_THUMB = 7,
    LEFT_SHOULDER = 8,
    RIGHT_SHOULDER = 9,
    A = 10,
    B = 11,
    X = 12,
    Y = 13,
    L_THUMB_UP = 14,
    L_THUMB_DOWN = 15,
    L_THUMB_LEFT = 16,
    L_THUMB_RIGHT = 17,
    MAX = 18
};

struct XInputController {
    XINPUT_STATE controllerState;
    int controllerNum;
    ButtonState gamepadStates[(int)ButtonCodes::MAX];

    static XInputController SetupController(int id)
    {
        XInputController controller = {};
        controller.controllerNum = id;
        return controller;
    }
};

void UpdateKeyboardStates(KeyboardMouse* keyboardMouse);
void UpdateGamepadStates(XInputController* controller, KeyboardMouse* keyboardMouse);
XINPUT_STATE GetState(XInputController* controller);
bool IsConnected(XInputController* controller);
void Vibrate(XInputController* controller, int leftVal, int rightVal);

#endif
