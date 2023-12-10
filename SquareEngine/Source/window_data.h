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
#ifndef WINDOW_DATA
#define WINDOW_DATA

#include "Drawing/drawing.h"

namespace SquarePixelEngine
{
	enum CameraTypes : int
	{
		Launcher = 0,
		Gameplay
	};

	struct WindowData
	{
		WindowData()
		{
			screenData = {};
			msg = {};
			window = {};
			hdc = {};
			scaleWindow = 2;
			isRunning = false;

			currentTicks = 0;
			deltaTime = 0;
			startTime = 0;
			fps = 0;

			camerasSize = 0;
			cameras = nullptr;
			gameCameraXSpeed = 0;
			gameCameraYSpeed = 0;
		}

		ScreenData screenData;
		MSG msg;
		HWND window;
		HDC hdc;
		int scaleWindow;
		bool isRunning;

		float currentTicks;
		float deltaTime;
		float startTime;
		float fps;

		int camerasSize;
		CameraRect* cameras;
		float gameCameraXSpeed;
		float gameCameraYSpeed;
	};
}

#endif