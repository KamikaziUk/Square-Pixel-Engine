#pragma once
#ifndef GAME_SANTA
#define GAME_SANTA

#include "../../SquareEngine/Source/drawing.h"
#include "../../SquareEngine/Source/input.h"
#include "../../SquareEngine/Source/External/cute_sound.h"

struct AABB
{
	float x;
	float y;
	float w;
	float h;

	AABB()
	{		
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}

	AABB(float inX, float inY, float inW, float inH)
	{
		x = inX;
		y = inY;
		w = inW;
		h = inH;
	}
};

struct Present
{
	Sprite sprite;
	bool active;

	AABB aabb;
};

struct BuildingImages
{
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
	SpriteAnimated chimneyParticle;
	bool active;
};

struct AudioData
{
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

struct GameSanta
{
	enum GameState
	{
		Menu,
		Game,
		End
	};

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
	char playStr[32];

	Text titleText;
	char titleStr[32];

	Text titleText2;
	char titleStr2[32];

	int bestScore;
	int score;
	Text scoreText;
	char scoreStr[32];

	int combo;
	Text comboText;
	char comboStr[32];

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

void OnSantaStart(GameSanta* game, CameraRect* mainCamera, cs_context_t* ctx);
void OnSantaUpdate(GameSanta* game, CameraRect* mainCamera, XInputController* controller, KeyboardMouse* keyboardMouse, float dt, cs_context_t* ctx);
void OnSantaRender(GameSanta* game, CameraRect* mainCamera, ScreenData* sD, int screenSize);
void OnSantaEnd(GameSanta* game, CameraRect* mainCamera, cs_context_t* ctx);

#endif
