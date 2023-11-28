#pragma once
#ifndef EMULATOR_INPUT
#define EMULATOR_INPUT

#include "../input_data.h"

ButtonState GetMenuButton(InputData* inputData);
ButtonState GetActionButton(InputData* inputData);
ButtonState GetBackButton(InputData* inputData);
float GetXAxis(InputData* inputData);
float GetYAxis(InputData* inputData);

#endif
