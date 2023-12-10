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
#ifndef KEYBOARD_MOUSE
#define KEYBOARD_MOUSE

#include "button_states.h"

namespace SquarePixelEngine
{
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

    struct KeyboardMouse
    {
        KeyboardMouse()
        {
            for(int i = 0; i < 256; i++)
            {
                keyStates[i] = {};
            }

            for(int i = 0; i < 256; i++)
            {
                keyPressed[i] = false;
            }
        }

        ButtonState keyStates[256];
        bool keyPressed[256];

        static KeyboardMouse Setup()
        {
            KeyboardMouse keyboard = {};
            return keyboard;
        }
    };

    bool UpdateKeyboardStates(KeyboardMouse* keyboardMouse);
}

#endif
