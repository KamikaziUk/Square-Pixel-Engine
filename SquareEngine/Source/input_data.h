#pragma once
#ifndef INPUT_DATA
#define INPUT_DATA

#include "Input/keyboard_mouse.h"
#include "Input/controller.h"

enum class InputType
{
    KeyboardMouse = 0,
    Controller = 1
};

struct InputData
{
    InputData()
    {
        controller = XInputController::SetupController(0);
        keyboardMouse = KeyboardMouse::Setup();
        inputType = InputType::KeyboardMouse;
    }

    InputType inputType;
    XInputController controller;
    KeyboardMouse keyboardMouse;
};

#endif