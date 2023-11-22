#include "emulator_input.h"

ButtonState GetMenuButton(XInputController* controller, KeyboardMouse* keyboardMouse)
{
    if(keyboardMouse->inputType == InputType::Gamepad)
    {
        return controller->gamepadStates[(int)ButtonCodes::START];
    }
    else
    {
        return keyboardMouse->keyStates[(int)KeyCodes::Escape];
    }
}

ButtonState GetActionButton(XInputController* controller, KeyboardMouse* keyboardMouse)
{
    if(keyboardMouse->inputType == InputType::Gamepad)
    {
        return controller->gamepadStates[(int)ButtonCodes::A];
    }
    else
    {
        return keyboardMouse->keyStates[(int)KeyCodes::JKey];
    }
}

ButtonState GetBackButton(XInputController* controller, KeyboardMouse* keyboardMouse)
{
    if(keyboardMouse->inputType == InputType::Gamepad)
    {
        return controller->gamepadStates[(int)ButtonCodes::B];
    }
    else
    {
        return keyboardMouse->keyStates[(int)KeyCodes::KKey];
    }
}

ButtonState GetDirection(XInputController* controller, KeyboardMouse* keyboardMouse, int directionID)
{
    int keyboardAxisIDs[] =
    {
        (int)KeyCodes::WKey,
        (int)KeyCodes::SKey,
        (int)KeyCodes::AKey,
        (int)KeyCodes::DKey
    };

    int leftAxisIDs[] =
    {
        (int)ButtonCodes::L_THUMB_UP,
        (int)ButtonCodes::L_THUMB_DOWN,
        (int)ButtonCodes::L_THUMB_LEFT,
        (int)ButtonCodes::L_THUMB_RIGHT
    };

    int dpadAxisIDs[] =
    {
        (int)ButtonCodes::DPAD_UP,
        (int)ButtonCodes::DPAD_DOWN,
        (int)ButtonCodes::DPAD_LEFT,
        (int)ButtonCodes::DPAD_RIGHT
    };

    if(keyboardMouse->inputType == InputType::Gamepad)
    {
        if(controller->gamepadStates[dpadAxisIDs[directionID]] != ButtonState::None)
        {
            return controller->gamepadStates[dpadAxisIDs[directionID]];
        }

        return controller->gamepadStates[leftAxisIDs[directionID]];
    }
    else
    {
        return keyboardMouse->keyStates[keyboardAxisIDs[directionID]];
    }
}

float GetXAxis(XInputController* controller, KeyboardMouse* keyboardMouse)
{
    ButtonState leftDir = GetDirection(controller, keyboardMouse, 2);
    ButtonState rightDir = GetDirection(controller, keyboardMouse, 3);

    float xLeft = (leftDir == ButtonState::Down || leftDir == ButtonState::Held) ? -1.0f : 0.0f;
    float xRight = (rightDir == ButtonState::Down || rightDir == ButtonState::Held) ? 1.0f : 0.0f;

    return xLeft + xRight;
}

float GetYAxis(XInputController* controller, KeyboardMouse* keyboardMouse)
{
    ButtonState upDir = GetDirection(controller, keyboardMouse, 0);
    ButtonState downDir = GetDirection(controller, keyboardMouse, 1);

    float yUp = (upDir == ButtonState::Down || upDir == ButtonState::Held) ? 1.0f : 0.0f;
    float yDown = (downDir == ButtonState::Down || downDir == ButtonState::Held) ? -1.0f : 0.0f;

    return yUp + yDown;
}

void UpdateEmulatorUI(XInputController* controller, KeyboardMouse* keyboardMouse, EmulatorRendering* emuRe)
{
    // Update buttons if input type changes
    if(keyboardMouse->inputType == InputType::Gamepad)
    {
        emuRe->sprites[2].image = &emuRe->newImages[(int)EmulatorImages::ButtonA];
        emuRe->sprites[4].image = &emuRe->newImages[(int)EmulatorImages::ButtonB];
    }
    else
    {
        emuRe->sprites[2].image = &emuRe->newImages[(int)EmulatorImages::ButtonJ];
        emuRe->sprites[4].image = &emuRe->newImages[(int)EmulatorImages::ButtonK];
    }

    // Left
    if(keyboardMouse->keyStates[(int)KeyCodes::AKey] == ButtonState::Held 
        || controller->gamepadStates[(int)ButtonCodes::DPAD_LEFT] == ButtonState::Held ||
        controller->gamepadStates[(int)ButtonCodes::L_THUMB_LEFT] == ButtonState::Held
        )
    {
        emuRe->sprites[9].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonDown];
        emuRe->sprites[10].y = 200;
    }
    else
    {
        emuRe->sprites[9].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonUp];
        emuRe->sprites[10].y = 198;
    }

    // Right
    if(keyboardMouse->keyStates[(int)KeyCodes::DKey] == ButtonState::Held 
        || controller->gamepadStates[(int)ButtonCodes::DPAD_RIGHT] == ButtonState::Held ||
        controller->gamepadStates[(int)ButtonCodes::L_THUMB_RIGHT] == ButtonState::Held
        )
    {
        emuRe->sprites[11].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonDown];
        emuRe->sprites[12].y = 200;
    }
    else
    {
        emuRe->sprites[11].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonUp];
        emuRe->sprites[12].y = 198;
    }

    // Up
    if(keyboardMouse->keyStates[(int)KeyCodes::WKey] == ButtonState::Held
        || controller->gamepadStates[(int)ButtonCodes::DPAD_UP] == ButtonState::Held ||
        controller->gamepadStates[(int)ButtonCodes::L_THUMB_UP] == ButtonState::Held
        )
    {
        emuRe->sprites[5].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonDown];
        emuRe->sprites[6].y = 184;
    }
    else
    {
        emuRe->sprites[5].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonUp];
        emuRe->sprites[6].y = 182;
    }

    // Down
    if(keyboardMouse->keyStates[(int)KeyCodes::SKey] == ButtonState::Held 
        || controller->gamepadStates[(int)ButtonCodes::DPAD_DOWN] == ButtonState::Held ||
        controller->gamepadStates[(int)ButtonCodes::L_THUMB_DOWN] == ButtonState::Held
        )
    {
        emuRe->sprites[7].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonDown];
        emuRe->sprites[8].y = 216;
    }
    else
    {
        emuRe->sprites[7].image = &emuRe->newImages[(int)EmulatorImages::DpadButtonUp];
        emuRe->sprites[8].y = 214;
    }

    // Action
    if(keyboardMouse->keyStates[(int)KeyCodes::JKey] == ButtonState::Held 
        || controller->gamepadStates[(int)ButtonCodes::A] == ButtonState::Held
        )
    {
        emuRe->sprites[1].image = &emuRe->newImages[(int)EmulatorImages::ButtonDown];
        emuRe->sprites[2].y = 204;
    }
    else
    {
        emuRe->sprites[1].image = &emuRe->newImages[(int)EmulatorImages::ButtonUp];
        emuRe->sprites[2].y = 202;
    }

    // Back
    if(keyboardMouse->keyStates[(int)KeyCodes::KKey] == ButtonState::Held 
        || controller->gamepadStates[(int)ButtonCodes::B] == ButtonState::Held
        )
    {
        emuRe->sprites[3].image = &emuRe->newImages[(int)EmulatorImages::ButtonDown];
        emuRe->sprites[4].y = 182;
    }
    else
    {
        emuRe->sprites[3].image = &emuRe->newImages[(int)EmulatorImages::ButtonUp];
        emuRe->sprites[4].y = 180;
    }

    // Menu
    if(keyboardMouse->keyStates[(int)KeyCodes::Escape] == ButtonState::Held 
        || controller->gamepadStates[(int)ButtonCodes::START] == ButtonState::Held
        )
    {
        emuRe->sprites[13].image = &emuRe->newImages[(int)EmulatorImages::ButtonMenuDown];
    }
    else
    {
        emuRe->sprites[13].image = &emuRe->newImages[(int)EmulatorImages::ButtonMenuUp];
    }
}