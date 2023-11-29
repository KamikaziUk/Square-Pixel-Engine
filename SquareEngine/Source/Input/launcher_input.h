#pragma once
#ifndef LAUNCHER_INPUT
#define LAUNCHER_INPUT

#include "input_data.h"

namespace SquarePixelEngine
{
	ButtonState GetMenuButton(const InputData* inputData);
	ButtonState GetActionButton(const InputData* inputData);
	ButtonState GetBackButton(const InputData* inputData);
	float GetXAxis(const InputData* inputData);
	float GetYAxis(const InputData* inputData);
}

#endif
