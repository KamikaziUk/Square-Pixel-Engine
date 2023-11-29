#include "launcher_input.h"

namespace SquarePixelEngine
{
    ButtonState GetMenuButton(const InputData* inputData)
    {
        if(inputData->inputType == InputType::Controller)
        {
            return inputData->controller.gamepadStates[(int)ControllerButtonCodes::START];
        }
        else
        {
            return inputData->keyboardMouse.keyStates[(int)KeyCodes::Escape];
        }
    }

    ButtonState GetActionButton(const InputData* inputData)
    {
        if(inputData->inputType == InputType::Controller)
        {
            return inputData->controller.gamepadStates[(int)ControllerButtonCodes::A];
        }
        else
        {
            return inputData->keyboardMouse.keyStates[(int)KeyCodes::JKey];
        }
    }

    ButtonState GetBackButton(const InputData* inputData)
    {
        if(inputData->inputType == InputType::Controller)
        {
            return inputData->controller.gamepadStates[(int)ControllerButtonCodes::B];
        }
        else
        {
            return inputData->keyboardMouse.keyStates[(int)KeyCodes::KKey];
        }
    }

    ButtonState GetDirection(const InputData* inputData, const int directionID)
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
            (int)ControllerButtonCodes::L_THUMB_UP,
            (int)ControllerButtonCodes::L_THUMB_DOWN,
            (int)ControllerButtonCodes::L_THUMB_LEFT,
            (int)ControllerButtonCodes::L_THUMB_RIGHT
        };

        int dpadAxisIDs[] =
        {
            (int)ControllerButtonCodes::DPAD_UP,
            (int)ControllerButtonCodes::DPAD_DOWN,
            (int)ControllerButtonCodes::DPAD_LEFT,
            (int)ControllerButtonCodes::DPAD_RIGHT
        };

        if(inputData->inputType == InputType::Controller)
        {
            if(inputData->controller.gamepadStates[dpadAxisIDs[directionID]] != ButtonState::None)
            {
                return inputData->controller.gamepadStates[dpadAxisIDs[directionID]];
            }

            return inputData->controller.gamepadStates[leftAxisIDs[directionID]];
        }
        else
        {
            return inputData->keyboardMouse.keyStates[keyboardAxisIDs[directionID]];
        }
    }

    float GetXAxis(const InputData* inputData)
    {
        ButtonState leftDir = GetDirection(inputData, 2);
        ButtonState rightDir = GetDirection(inputData, 3);

        float xLeft = (leftDir == ButtonState::Down || leftDir == ButtonState::Held) ? -1.0f : 0.0f;
        float xRight = (rightDir == ButtonState::Down || rightDir == ButtonState::Held) ? 1.0f : 0.0f;

        return xLeft + xRight;
    }

    float GetYAxis(const InputData* inputData)
    {
        ButtonState upDir = GetDirection(inputData, 0);
        ButtonState downDir = GetDirection(inputData, 1);

        float yUp = (upDir == ButtonState::Down || upDir == ButtonState::Held) ? 1.0f : 0.0f;
        float yDown = (downDir == ButtonState::Down || downDir == ButtonState::Held) ? -1.0f : 0.0f;

        return yUp + yDown;
    }
}