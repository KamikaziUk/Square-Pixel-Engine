#pragma once
#ifndef WINDOW_DATA
#define WINDOW_DATA

#include "Drawing/drawing.h"

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
		deltaTicks = 0;
		begin = 0;
		fps = 0;
		dt = 0;
	}

	ScreenData screenData;
	MSG msg;
	HWND window;
	HDC hdc;
	int scaleWindow;
	bool isRunning;
	float currentTicks;
	float deltaTicks;
	float begin;
	float fps;
	float dt;
};

#endif