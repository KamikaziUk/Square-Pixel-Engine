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
        XInputController()
        {
            controllerState = {};
            controllerNum = 0;

            for(int i = 0; i < (int)ControllerButtonCodes::MAX; i++)
            {
                gamepadStates[i] = {};
            }
        }

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
