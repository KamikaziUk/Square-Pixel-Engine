#include <stdio.h>

#include "controller.h"

XINPUT_STATE GetState(XInputController* controller)
{
    //Set the memory to 0
    ZeroMemory(&controller->controllerState, sizeof(XINPUT_STATE));

    //Get the current state
    XInputGetState(controller->controllerNum, &controller->controllerState);

    return controller->controllerState;
}

bool IsConnected(XInputController* controller)
{
    //Set the memory to 0
    ZeroMemory(&controller->controllerState, sizeof(XINPUT_STATE));

    //Get the current state
    DWORD Result = XInputGetState(controller->controllerNum, &controller->controllerState);

    if(Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Vibrate(XInputController* controller, int leftVal, int rightVal)
{
    //Create vibration state
    XINPUT_VIBRATION vibration;

    //Set the memory to 0
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    //Set the values
    vibration.wLeftMotorSpeed = leftVal;
    vibration.wRightMotorSpeed = rightVal;

    //Vibrate the controller
    XInputSetState(controller->controllerNum, &vibration);
}

bool UpdateControllerStates(XInputController* controller)
{
    if(IsConnected(controller))
    {
        int buttons[] =
        {
            0x0001, //XINPUT_GAMEPAD_DPAD_UP
            0x0002, //XINPUT_GAMEPAD_DPAD_DOWN
            0x0004, //XINPUT_GAMEPAD_DPAD_LEFT
            0x0008, //XINPUT_GAMEPAD_DPAD_RIGHT
            0x0010, //XINPUT_GAMEPAD_START
            0x0020, //XINPUT_GAMEPAD_BACK
            0x0040, //XINPUT_GAMEPAD_LEFT_THUMB
            0x0080, //XINPUT_GAMEPAD_RIGHT_THUMB
            0x0100, //XINPUT_GAMEPAD_LEFT_SHOULDER
            0x0200, //XINPUT_GAMEPAD_RIGHT_SHOULDER
            0x1000, //XINPUT_GAMEPAD_A
            0x2000, //XINPUT_GAMEPAD_B
            0x4000, //XINPUT_GAMEPAD_X
            0x8000  //XINPUT_GAMEPAD_Y
        };

        auto gamepadState = GetState(controller);

        bool anyButtonPressed = false;

        // Buttons
        for(int i = 0; i < 14; i++)
        {
            bool pressedDown = gamepadState.Gamepad.wButtons & buttons[i];
            UpdateButtonState(&controller->gamepadStates[i], pressedDown);

            anyButtonPressed |= pressedDown;
        }

        bool upHeld = gamepadState.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        UpdateButtonState(&controller->gamepadStates[14], upHeld);
        anyButtonPressed |= upHeld;

        bool downHeld = gamepadState.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        UpdateButtonState(&controller->gamepadStates[15], downHeld);
        anyButtonPressed |= downHeld;

        bool leftHeld = gamepadState.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        UpdateButtonState(&controller->gamepadStates[16], leftHeld);
        anyButtonPressed |= leftHeld;

        bool rightHeld = gamepadState.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
        UpdateButtonState(&controller->gamepadStates[17], rightHeld);
        anyButtonPressed |= rightHeld;

        if(anyButtonPressed)
        {
            return true;
        }
    }
    else
    {
        for(int i = 0; i < 18; i++)
        {
            controller->gamepadStates[i] = ButtonState::None;
        }
    }

    return false;
}