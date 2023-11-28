#include <stdio.h>
#include <windows.h>

#include "button_states.h"
#include "keyboard_mouse.h"

bool UpdateKeyboardStates(KeyboardMouse* keyboardMouse)
{
    bool pressedButton = false;

    for(int i = 0; i < 256; i++)
    {
        bool keyHeld = (GetAsyncKeyState(i) & (1 << 16));
        UpdateButtonState(&keyboardMouse->keyStates[i], keyHeld);

        // Don't count mouse for now
        if(keyHeld && i > 3)
        {
            pressedButton = true;
        }
    }

    return pressedButton;
}