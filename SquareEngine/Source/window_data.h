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