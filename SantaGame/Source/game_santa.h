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
#ifndef GAME_SANTA
#define GAME_SANTA

#include "Drawing/drawing.h"
#include "Input/launcher_input.h"

#include "input_data.h"
#include "sound_data.h"

#define CUTE_SOUND_IMPLEMENTATION
#include "External/cute_sound.h"

#include "Utilities/carray.h"
#include "Utilities/aabb.h"

using namespace SquarePixelEngine;
using namespace Utilities;

namespace SantaGame
{
	struct Present
	{
		Present()
		{
			active = false;
			sprite = {};
			aabb = {};
		}

		bool active;
		Sprite sprite;
		AABB aabb;
	};

	struct BuildingImages
	{
		BuildingImages()
		{
			for(int i = 0; i < ARRAY_COUNT(roofImage1); i++)
			{
				roofImage1[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(roofImage2); i++)
			{
				roofImage2[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(roofImage3); i++)
			{
				roofImage3[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(roofImage4); i++)
			{
				roofImage4[i] = {};
			}

			chimneyImage = {};
			chimneyParticleImage = {};
			lifeParticleImage = {};
			naughtySignImage = {};

			for(int i = 0; i < ARRAY_COUNT(buildingBlockImage); i++)
			{
				buildingBlockImage[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(windowImages); i++)
			{
				windowImages[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(foregroundImages); i++)
			{
				foregroundImages[i] = {};
			}
		}

		Image roofImage1[1];
		Image roofImage2[1];
		Image roofImage3[1];
		Image roofImage4[1];

		Image chimneyImage;
		Image chimneyParticleImage;
		Image lifeParticleImage;
		Image naughtySignImage;

		Image buildingBlockImage[4];
		Image windowImages[3];
		Image foregroundImages[3];
	};

	struct Building
	{
		Building()
		{
			active = false;

			widthSize = 0;
			heightSize = 0;

			chimneyAABB = {};
			lifeAABB = {};

			roof = {};

			hasChimney = false;
			chimney = {};
			chimneyParticle = {};

			lifeParticle = {};
			lifeActive = false;

			naughtySign = {};
			isNaughty = false;

			foreground = {};

			blocksSize = false;

			for(int i = 0; i < ARRAY_COUNT(buildingBlocks); i++)
			{
				buildingBlocks[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(windows); i++)
			{
				windows[i] = {};
			}
		}

		bool active;

		int widthSize;
		int heightSize;

		AABB chimneyAABB;
		AABB lifeAABB;

		Sprite roof;

		bool hasChimney;
		Sprite chimney;
		SpriteAnimated chimneyParticle;

		SpriteAnimated lifeParticle;
		bool lifeActive;

		SpriteAnimated naughtySign;
		bool isNaughty;

		Sprite foreground;

		int blocksSize;
		Sprite buildingBlocks[12]; // Max: 4x3
		Sprite windows[12]; // Max: 4x3
	};

	struct MissParticle
	{
		MissParticle()
		{
			chimneyParticle = {};
			active = false;
		}

		SpriteAnimated chimneyParticle;
		bool active;
	};

	struct AudioData
	{
		AudioData()
		{
			musicData = {};
			music = {};

			buttonUIData = {};
			buttonUI = {};

			dropData = {};
			drop = {};

			hitData = {};
			hit = {};

			missedData = {};
			missed = {};

			pickupData = {};
			pickup = {};

			lostData = {};
			lost = {};
		}

		cs_loaded_sound_t musicData;
		cs_playing_sound_t music;

		cs_loaded_sound_t buttonUIData;
		cs_playing_sound_t buttonUI;

		cs_loaded_sound_t dropData;
		cs_playing_sound_t drop;

		cs_loaded_sound_t hitData;
		cs_playing_sound_t hit;

		cs_loaded_sound_t missedData;
		cs_playing_sound_t missed;

		cs_loaded_sound_t pickupData;
		cs_playing_sound_t pickup;

		cs_loaded_sound_t lostData;
		cs_playing_sound_t lost;
	};

	enum class GameState
	{
		Menu = 0,
		Game,
		End
	};

	struct GameData
	{
		GameData()
		{
			santaImage = {};
			skyImage = {};
			backdropImage = {};
			moonImage = {};
			lifeImage = {};
			cloudImage = {};
			menuImage = {};
			missParticleImage = {};

			for(int i = 0; i < ARRAY_COUNT(presentImages); i++)
			{
				presentImages[i] = {};
			}

			santa = {};
			sky = {};
			backdrop = {};
			clouds = {};
			menu = {};

			textFont = {};
			playText = {};
			titleText = {};
			titleText2 = {};
			scoreText = {};
			comboText = {};

			for(int i = 0; i < ARRAY_COUNT(playStr); i++)
			{
				playStr[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(titleStr); i++)
			{
				titleStr[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(titleStr2); i++)
			{
				titleStr2[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(scoreStr); i++)
			{
				scoreStr[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(comboStr); i++)
			{
				comboStr[i] = {};
			}

			score = 0;
			bestScore = 0;
			combo = 0;
			lives = 0;

			for(int i = 0; i < ARRAY_COUNT(liveSprites); i++)
			{
				liveSprites[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(presents); i++)
			{
				presents[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(snowParticleX); i++)
			{
				snowParticleX[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(snowParticleY); i++)
			{
				snowParticleY[i] = {};
			}

			for(int i = 0; i < ARRAY_COUNT(missParticle); i++)
			{
				missParticle[i] = {};
			}

			skyScrollX = 0;
			backScrollX = 0;
			cloudScrollX = 0;

			flickerTimer = 0;
			delayTimer = 0;
			gameTimer = 0;

			nextLife = 0;
			nextNaughty = 0;
			nextChimney = 0;

			nextBuildingXOffset = 0.0f;

			buildingImages = {};

			for(int i = 0; i < ARRAY_COUNT(buildings); i++)
			{
				buildings[i] = {};
			}

			audioData = {};
			gameState = GameState::Menu;
		}

		Image santaImage;
		Image skyImage;
		Image backdropImage;
		Image moonImage;
		Image lifeImage;
		Image cloudImage;
		Image menuImage;
		Image missParticleImage;

		Image presentImages[3];

		Image textFont;

		SpriteAnimated santa;
		Sprite sky;
		Sprite backdrop;
		Sprite clouds;
		SpriteAnimated menu;

		Text playText;
		Text titleText;
		Text titleText2;
		Text scoreText;

		char playStr[32];
		char titleStr[32];
		char titleStr2[32];
		char scoreStr[32];
		char comboStr[32];

		int score;
		int bestScore;
		int combo;
		Text comboText;

		int lives;
		Sprite liveSprites[3];

		Present presents[10];

		float snowParticleX[32];
		float snowParticleY[32];

		MissParticle missParticle[32];

		float skyScrollX;
		float backScrollX;
		float cloudScrollX;

		float flickerTimer;

		float delayTimer;

		float gameTimer;

		int nextLife;
		int nextNaughty;
		int nextChimney;

		float nextBuildingXOffset = 0.0f;
		BuildingImages buildingImages;
		Building buildings[10];

		AudioData audioData;

		GameState gameState;
	};

	#include "game_export.h"

	extern GameData* game;
}

#endif