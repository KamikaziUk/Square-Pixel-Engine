#pragma once
#ifndef EMULATOR_INPUT_CODE
#define EMULATOR_INPUT_CODE

#include "emulator_rendering.h"

#include "../input.h"
#include "../drawing.h"

void UpdateEmulatorUI(XInputController* controller, KeyboardMouse* keyboardMouse, EmulatorRendering* emuRe);

ButtonState GetMenuButton(XInputController* controller, KeyboardMouse* keyboardMouse);
ButtonState GetActionButton(XInputController* controller, KeyboardMouse* keyboardMouse);
ButtonState GetBackButton(XInputController* controller, KeyboardMouse* keyboardMouse);
float GetXAxis(XInputController* controller, KeyboardMouse* keyboardMouse);
float GetYAxis(XInputController* controller, KeyboardMouse* keyboardMouse);

#endif
