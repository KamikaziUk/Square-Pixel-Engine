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

#include "input_data.h"
#include "window_data.h"

namespace SquarePixelEngine
{
    void DebugCameraMovement(const InputData* inputData, WindowData* windowData)
    {
        const float moveSpeed = 0.1f;

        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::ArrowLeft] == ButtonState::Held)
        {
            windowData->gameCameraXSpeed = -moveSpeed;
        }
        else if(inputData->keyboardMouse.keyStates[(int)KeyCodes::ArrowRight] == ButtonState::Held)
        {
            windowData->gameCameraXSpeed = moveSpeed;
        }
        else
        {
            windowData->gameCameraXSpeed = 0;
        }

        if(inputData->keyboardMouse.keyStates[(int)KeyCodes::ArrowUp] == ButtonState::Held)
        {
            windowData->gameCameraYSpeed = moveSpeed;
        }
        else if(inputData->keyboardMouse.keyStates[(int)KeyCodes::ArrowDown] == ButtonState::Held)
        {
            windowData->gameCameraYSpeed = -moveSpeed;
        }
        else
        {
            windowData->gameCameraYSpeed = 0;
        }
    }
}