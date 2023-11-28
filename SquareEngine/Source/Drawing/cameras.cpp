#include "input_data.h"
#include "window_data.h"

void DebugCameraMovement(InputData* inputData, WindowData* windowData)
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