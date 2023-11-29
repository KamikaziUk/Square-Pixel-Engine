#pragma once
#ifndef GAME_EXPORT
#define GAME_EXPORT

#ifdef GAME_EXPORTS
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif

#define EXPORTDLL extern "C" GAME_API

// NOTE: If changed, remember to update in the main.cpp of launcher
EXPORTDLL void OnGameStart(SoundData* soundData, CameraRect* mainCamera);
EXPORTDLL void OnGameUpdate(InputData* inputData, SoundData* soundData, CameraRect* mainCamera, float deltaTime);
EXPORTDLL void OnGameRender(CameraRect* mainCamera, ScreenData* sD, int screenSize);
EXPORTDLL void OnGameEnd(SoundData* soundData, CameraRect* mainCamera);

#endif