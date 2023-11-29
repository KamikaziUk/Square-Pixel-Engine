#pragma once
#ifndef CONTROLLER 
#define CONTROLLER 
         
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

#include "button_states.h"

namespace SquarePixelEngine
{
    enum class ControllerButtonCodes
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

    struct XInputController
    {
        XINPUT_STATE controllerState;
        int controllerNum;
        ButtonState gamepadStates[(int)ControllerButtonCodes::MAX];

        static XInputController SetupController(int id)
        {
            XInputController controller = {};
            controller.controllerNum = id;
            return controller;
        }
    };

    bool UpdateControllerStates(XInputController* controller);
    XINPUT_STATE GetState(XInputController* controller);
    bool IsConnected(XInputController* controller);
    void Vibrate(XInputController* controller, int leftVal, int rightVal);
}

#endif
