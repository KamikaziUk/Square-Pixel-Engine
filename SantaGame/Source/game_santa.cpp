#include "game_santa.h"

#include "drawing.h"
#include "Emulator/emulator_input.h"

#include <time.h>
#include <cmath>
#include <iostream>

#define ARRAY_COUNT(arrayName) sizeof(arrayName) / sizeof(arrayName[0])

GameSanta* game;

int RandomMinMax(int min, int max)
{
	if(min == 0 && max == 0)
	{
		return 0;
	}

	int random = (rand() % (max - min + 1)) + min;
	return random;
}

float clampVal(float min, float max, float value)
{
	if(value < min)
		return min;

	else if(value > max)
		return max;

	return value;
}

void GenerateBuilding(GameSanta* game, CameraRect* mainCamera)
{
	int currentBuildingId = -1;

	for(int i = 0; i < ARRAY_COUNT(game->buildings); i++)
	{
		if(!game->buildings[i].active)
		{
			game->buildings[i].active = true;
			currentBuildingId = i;
			break;
		}
	}

	if(currentBuildingId == -1)
	{
		// Ran out of buildings in pool
		return;
	}

	Building* building = &game->buildings[currentBuildingId];

	building->widthSize = RandomMinMax(1, 4);
	building->heightSize = RandomMinMax(1, 3);

	const auto tileSize = 16;

	if(building->widthSize == 1)
	{
		building->roof = Sprite(game->nextBuildingXOffset - tileSize, ((float)(building->heightSize-1) * 16.0f), 1, &game->buildingImages.roofImage1[0], mainCamera);
	}
	else if(building->widthSize == 2)
	{
		building->roof = Sprite(game->nextBuildingXOffset - tileSize, ((float)(building->heightSize - 1) * 16.0f), 1, &game->buildingImages.roofImage2[0], mainCamera);
	}
	else if(building->widthSize == 3)
	{
		building->roof = Sprite(game->nextBuildingXOffset - tileSize, ((float)(building->heightSize - 1) * 16.0f), 1, &game->buildingImages.roofImage3[0], mainCamera);
	}
	else if(building->widthSize == 4)
	{
		building->roof = Sprite(game->nextBuildingXOffset - tileSize, ((float)(building->heightSize - 1) * 16.0f), 1, &game->buildingImages.roofImage4[0], mainCamera);
	}

	int randomChimney = building->widthSize > 1 ? RandomMinMax(0, building->widthSize-1) : 0;
	building->chimney = Sprite(game->nextBuildingXOffset + ((float)randomChimney * 16.0f), ((float)(building->heightSize) * 16.0f), 1, &game->buildingImages.chimneyImage, mainCamera);

	building->chimneyParticle.x = building->chimney.x;
	building->chimneyParticle.y = building->chimney.y + 16;
	building->chimneyParticle.currentTime = building->chimneyParticle.animations[building->chimneyParticle.currentAnimationID].animationTime;

	building->lifeParticle.x = building->chimney.x;
	building->lifeParticle.y = building->chimney.y + 16;
	building->lifeParticle.currentTime = building->lifeParticle.animations[building->lifeParticle.currentAnimationID].animationTime * ((float)RandomMinMax(0, 100) / 100.0f);

	building->naughtySign.x = building->chimney.x;
	building->naughtySign.y = building->chimney.y;
	building->naughtySign.currentTime = building->naughtySign.animations[building->naughtySign.currentAnimationID].animationTime * ((float)RandomMinMax(0, 100) / 100.0f);

	building->lifeAABB = AABB(building->chimney.x, building->chimney.y + 16, 16, 16);
	building->chimneyAABB = AABB(building->chimney.x, building->chimney.y, 16, 16);

	if(game->nextChimney <= 0)
	{
		building->hasChimney = true;
		game->nextChimney = RandomMinMax(0, 1);
	}
	else
	{
		building->hasChimney = false;
		game->nextChimney--;
	}

	if(building->hasChimney)
	{
		if(building->widthSize >= 2 &&
			randomChimney < building->widthSize - 1 &&
			game->nextNaughty <= 0)
		{
			building->isNaughty = true;
			game->nextNaughty = RandomMinMax(2, 5);
		}
		else
		{
			building->isNaughty = false;
			game->nextNaughty--;
		}

		if(!building->isNaughty)
		{
			if(game->lives < 2 && game->nextLife <= 0)
			{
				building->lifeActive = true;
				game->nextLife = RandomMinMax(4, 10);
			}
			else
			{
				building->lifeActive = false;
				game->nextLife--;
			}
		}
	}

	building->foreground = Sprite(game->nextBuildingXOffset + ((float)(building->widthSize) * 16.0f), 0, 1, &game->buildingImages.foregroundImages[RandomMinMax(0, 2)], mainCamera);

	building->blocksSize = building->widthSize * building->heightSize;

	bool buildingType = RandomMinMax(0, 100) < 50;

	for(int x = 0; x < building->widthSize; x++)
	{
		for(int y = 0; y < building->heightSize; y++)
		{
			int arrayID = (y * building->widthSize) + x;

			int windowId = 0;
			auto randWindow = RandomMinMax(0, 100);

			if(randWindow < 50)
			{
				windowId = 0;
			}
			else if(randWindow < 90)
			{
				windowId = 2;
			}
			else
			{
				windowId = 1;
			}

			if(buildingType)
			{
				building->buildingBlocks[arrayID] = Sprite(game->nextBuildingXOffset + ((float)x * 16.0f), ((float)y * 16.0f), 1, &game->buildingImages.buildingBlockImage[RandomMinMax(0, 1)], mainCamera);
			}
			else
			{
				building->buildingBlocks[arrayID] = Sprite(game->nextBuildingXOffset + ((float)x * 16.0f), ((float)y * 16.0f), 1, &game->buildingImages.buildingBlockImage[RandomMinMax(2, 3)], mainCamera);
			}

			building->windows[arrayID] = Sprite( game->nextBuildingXOffset + ((float)x * 16.0f), ((float)y * 16.0f), 1, &game->buildingImages.windowImages[windowId], mainCamera);
		}
	}

	game->nextBuildingXOffset += tileSize * (building->widthSize + 1);
}

void CuteLoadSound(cs_context_t* ctx, const char* soundPath, cs_playing_sound_t* playSound, cs_loaded_sound_t* loadSound)
{	
	if(ctx != nullptr)
	{
		*loadSound = cs_load_wav(soundPath);
		*playSound = cs_make_playing_sound(loadSound);
	}
}

void CutePlaySound(cs_context_t* ctx, cs_playing_sound_t* sound, bool loop)
{
	if(ctx != nullptr)
	{
		cs_insert_sound(ctx, sound);
		cs_loop_sound(sound, loop ? 1 : 0);
	}
}

void OnGameStart(CameraRect* mainCamera, cs_context_t* ctx)
{	
	game = new GameSanta();

	game->gameState = GameSanta::GameState::Menu;

	game->santaImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Santa.png");
	game->skyImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Sky.png");
	game->backdropImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Backdrop.png");
	game->cloudImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Clouds.png");
	game->moonImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Moon.png");
	game->menuImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Menu.png");

	game->presentImages[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/Present0.png");
	game->presentImages[1] = LoadImageFromFile("../SantaGame/Assets/Sprites/Present1.png");
	game->presentImages[2] = LoadImageFromFile("../SantaGame/Assets/Sprites/Present2.png");

	game->lives = ARRAY_COUNT(game->liveSprites);

	game->lifeImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Life.png");

	for(int i = 0; i < ARRAY_COUNT(game->liveSprites); i++)
	{
		game->liveSprites[i] = Sprite(160 - (((float)(i + 1) * 10) + 8), 144 - 16, 0, &game->lifeImage, mainCamera);
	}

	game->textFont = LoadImageFromFile("../SantaGame/Assets/Fonts/Font16.png");

	game->santa = SpriteAnimated(48, 80, 64, 32, 1, &game->santaImage, mainCamera);
	LoadAnimation(&game->santa, "../SantaGame/Assets/AnimFiles/Santa.anim");

	game->sky = Sprite(0, 0, 0, &game->skyImage, mainCamera);
	game->backdrop = Sprite(0, 0, 1, &game->backdropImage, mainCamera);
	game->clouds = Sprite(0, 0, 1, &game->cloudImage, mainCamera);

	game->menu = SpriteAnimated(0, 0, 160, 144, 1, &game->menuImage, mainCamera);
	LoadAnimation(&game->menu, "../SantaGame/Assets/AnimFiles/Menu.anim");

	sprintf(game->scoreStr, "SCORE 0");
	game->scoreText = Text(0, 144 - 16, 6, 6, 7, game->scoreStr, 16, 10, colorWhite, &game->textFont, mainCamera);

	sprintf(game->comboStr, "COMBO 0");
	game->comboText = Text(0, 144 - 24, 6, 6, 7, game->comboStr, 16, 10, colorWhite, &game->textFont, mainCamera);

	auto colorTitle = Color(255, 233, 0);

	sprintf(game->playStr, "PLAY");
	game->playText = Text(88, 144 - 54, 6, 6, 4, game->playStr, 16, 10, colorWhite, &game->textFont, mainCamera);

	sprintf(game->titleStr, "KRINGLES");
	game->titleText = Text(76, 144 - 25, 6, 6, 8, game->titleStr, 16, 10, colorTitle, &game->textFont, mainCamera);

	sprintf(game->titleStr2, "DELIVERY");
	game->titleText2 = Text(76, 144 - 35, 6, 6, 17, game->titleStr2, 16, 10, colorTitle, &game->textFont, mainCamera);

	// Load building images
	game->buildingImages.roofImage1[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/RoofTile1_0.png");
	game->buildingImages.roofImage2[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/RoofTile2_0.png");
	game->buildingImages.roofImage3[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/RoofTile3_0.png");
	game->buildingImages.roofImage4[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/RoofTile4_0.png");

	game->buildingImages.chimneyImage = LoadImageFromFile("../SantaGame/Assets/Sprites/Chimney0.png");

	game->buildingImages.buildingBlockImage[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/BuildingBlock0.png");
	game->buildingImages.buildingBlockImage[1] = LoadImageFromFile("../SantaGame/Assets/Sprites/BuildingBlock1.png");
	game->buildingImages.buildingBlockImage[2] = LoadImageFromFile("../SantaGame/Assets/Sprites/BuildingBlock2.png");
	game->buildingImages.buildingBlockImage[3] = LoadImageFromFile("../SantaGame/Assets/Sprites/BuildingBlock3.png");

	game->buildingImages.windowImages[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/Window0.png");
	game->buildingImages.windowImages[1] = LoadImageFromFile("../SantaGame/Assets/Sprites/Window1.png");
	game->buildingImages.windowImages[2] = LoadImageFromFile("../SantaGame/Assets/Sprites/Window2.png");

	game->buildingImages.foregroundImages[0] = LoadImageFromFile("../SantaGame/Assets/Sprites/Foreground0.png");
	game->buildingImages.foregroundImages[1] = LoadImageFromFile("../SantaGame/Assets/Sprites/Foreground1.png");
	game->buildingImages.foregroundImages[2] = LoadImageFromFile("../SantaGame/Assets/Sprites/Foreground2.png");

	game->buildingImages.chimneyParticleImage = LoadImageFromFile("../SantaGame/Assets/Sprites/ChimneyParticle.png");
	game->buildingImages.lifeParticleImage = LoadImageFromFile("../SantaGame/Assets/Sprites/LifePickup.png");
	game->buildingImages.naughtySignImage = LoadImageFromFile("../SantaGame/Assets/Sprites/NaughtySign.png");

	for(int y = 0; y < ARRAY_COUNT(game->snowParticleY); y++)
	{
		game->snowParticleX[y] = RandomMinMax(0, 256);
		game->snowParticleY[y] = RandomMinMax(144, 288);
	}

	for(int i = 0; i < ARRAY_COUNT(game->buildings); i++)
	{
		game->buildings[i].chimneyParticle = SpriteAnimated(0, 0, 16, 16, 2, &game->buildingImages.chimneyParticleImage, mainCamera);
		LoadAnimation(&game->buildings[i].chimneyParticle, "../SantaGame/Assets/AnimFiles/ChimneyParticle.anim");

		game->buildings[i].lifeParticle = SpriteAnimated(0, 0, 16, 16, 3, &game->buildingImages.lifeParticleImage, mainCamera);
		LoadAnimation(&game->buildings[i].lifeParticle, "../SantaGame/Assets/AnimFiles/LifePickup.anim");

		game->buildings[i].naughtySign = SpriteAnimated(0, 0, 32, 16, 3, &game->buildingImages.naughtySignImage, mainCamera);
		LoadAnimation(&game->buildings[i].naughtySign, "../SantaGame/Assets/AnimFiles/NaughtySign.anim");

		GenerateBuilding(game, mainCamera);
	}

	game->missParticleImage = LoadImageFromFile("../SantaGame/Assets/Sprites/MissParticle.png");
	for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
	{
		game->missParticle[i].chimneyParticle = SpriteAnimated(0, 0, 16, 16, 4, &game->missParticleImage, mainCamera);
		LoadAnimation(&game->missParticle[i].chimneyParticle, "../SantaGame/Assets/AnimFiles/MissParticle.anim");
	}

	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/Music.wav", &game->audioData.music, &game->audioData.musicData);
	CutePlaySound(ctx, &game->audioData.music, true);

	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/ButtonUI.wav", &game->audioData.buttonUI, &game->audioData.buttonUIData);

	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/Drop.wav", &game->audioData.drop, &game->audioData.dropData);
	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/Hit.wav", &game->audioData.hit, &game->audioData.hitData);
	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/Missed.wav", &game->audioData.missed, &game->audioData.missedData);
	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/Pickup.wav", &game->audioData.pickup, &game->audioData.pickupData);
	CuteLoadSound(ctx, "../SantaGame/Assets/Sounds/Lost.wav", &game->audioData.lost, &game->audioData.lostData);
}

void OnGameEnd(CameraRect* mainCamera, cs_context_t* ctx)
{	
	cs_free_sound(&game->audioData.musicData);
	cs_free_sound(&game->audioData.buttonUIData);
	cs_free_sound(&game->audioData.dropData);
	cs_free_sound(&game->audioData.hitData);
	cs_free_sound(&game->audioData.missedData);
	cs_free_sound(&game->audioData.pickupData);
	cs_free_sound(&game->audioData.lostData);

	delete game;
	game = nullptr;
}

void UpdateHUD(GameSanta* game)
{
	sprintf(game->scoreStr, "SCORE %d", game->score);
	game->scoreText.stringLength = 6 + ((game->score / 10) + 1);

	sprintf(game->comboStr, "COMBO %d", game->combo);
	game->comboText.stringLength = 6 + ((game->combo / 10) + 1);
}

void GameOver(cs_context_t* ctx, GameSanta* game, CameraRect* mainCamera)
{
	game->gameState = GameSanta::GameState::End;

	auto color2Title = Color(105, 157, 66);
	auto colorTitle = Color(255, 233, 0);

	sprintf(game->titleStr, "LAST %d", game->score);
	game->titleText.stringLength = 6 + ((game->score / 10) + 1);

	if(game->score > game->bestScore)
	{
		game->bestScore = game->score;
	}

	sprintf(game->titleStr2, "BEST %d", game->bestScore);
	game->titleText2.stringLength = 6 + ((game->bestScore / 10) + 1);

	CutePlaySound(ctx, &game->audioData.lost, false);

	game->delayTimer = 0.5f;
}

void DropPresent(cs_context_t* ctx, GameSanta* game, CameraRect* mainCamera)
{
	for(int i = 0; i < ARRAY_COUNT(game->presents); i++)
	{
		if(!game->presents[i].active)
		{
			int presentID = RandomMinMax(0, ARRAY_COUNT(game->presentImages) - 1);

			game->presents[i].active = true;
			game->presents[i].sprite = Sprite(54, 90, 1, &game->presentImages[presentID], mainCamera);
			game->presents[i].aabb = AABB(game->presents[i].sprite.x, game->presents[i].sprite.y, 4, 4);

			game->santa.currentAnimationID = 1;
			game->santa.currentTime = 0.0f;

			CutePlaySound(ctx, &game->audioData.drop, false);
			break;
		}
	}
}

void SpawnMissParticle(GameSanta* game)
{
	for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
	{
		if(!game->missParticle[i].active)
		{
			game->missParticle[i].active = true;
			game->missParticle[i].chimneyParticle.x = 160 - (((float)(game->lives + 1) * 10) + 8);
			game->missParticle[i].chimneyParticle.y = 144 - 16;
			game->missParticle[i].chimneyParticle.currentTime = 0.0f;
			break;
		}
	}
}

void OnGameUpdate(CameraRect* mainCamera, XInputController* controller, KeyboardMouse* keyboardMouse, float dt, cs_context_t* ctx)
{		
	game->gameTimer += dt;

	game->delayTimer -= dt;

	if(game->gameState == GameSanta::GameState::Menu)
	{
		if(game->delayTimer <= 0.0f && GetActionButton(controller, keyboardMouse) == ButtonState::Down)
		{
			game->gameState = GameSanta::GameState::Game;
		}

		UpdateSpriteAnimated(&game->menu, dt);

		game->flickerTimer += dt * 0.5f;

		if(game->flickerTimer > 1.0f)
		{
			game->flickerTimer = 0.0f;
		}
	}
	else if(game->gameState == GameSanta::GameState::End)
	{
		if(game->delayTimer <= 0.0f && GetActionButton(controller, keyboardMouse) == ButtonState::Down)
		{
			game->gameState = GameSanta::GameState::Game;
			game->delayTimer = 0.5f;

			game->lives = 3;
			game->score = 0;
			game->combo = 0;
			game->gameTimer = 0.0f;

			CutePlaySound(ctx, &game->audioData.buttonUI, false);

			for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
			{
				game->missParticle[i].active = false;
			}
		}

		UpdateSpriteAnimated(&game->menu, dt);

		game->flickerTimer += dt * 0.5f;

		if(game->flickerTimer > 1.0f)
		{
			game->flickerTimer = 0.0f;
		}
	}
	else
	{
		if(game->delayTimer <= 0.0f && GetActionButton(controller, keyboardMouse) == ButtonState::Down)
		{
			DropPresent(ctx, game, mainCamera);
		}

		auto scrollSpeed = 96.0f + (66.0f * clampVal(0.0f, 1.0f, game->gameTimer / 90.0f));

		for(int i = 0; i < ARRAY_COUNT(game->buildings); i++)
		{
			if(game->buildings[i].active)
			{
				for(int j = 0; j < game->buildings[i].blocksSize; j++)
				{
					game->buildings[i].buildingBlocks[j].x -= scrollSpeed * dt;
					game->buildings[i].windows[j].x -= scrollSpeed * dt;
				}

				game->buildings[i].chimney.x -= scrollSpeed * dt;
				game->buildings[i].roof.x -= scrollSpeed * dt;
				game->buildings[i].foreground.x -= scrollSpeed * dt;

				if(game->buildings[i].hasChimney)
				{
					game->buildings[i].chimneyParticle.x -= scrollSpeed * dt;
					game->buildings[i].lifeParticle.x -= scrollSpeed * dt;
					game->buildings[i].naughtySign.x -= scrollSpeed * dt;
					game->buildings[i].chimneyAABB.x -= scrollSpeed * dt;
					game->buildings[i].lifeAABB.x -= scrollSpeed * dt;
				}

				if(game->buildings[i].buildingBlocks->x <= -(6 * 32))
				{
					game->buildings[i].active = false;
					GenerateBuilding(game, mainCamera);
				}

				UpdateSpriteAnimated(&game->buildings[i].chimneyParticle, dt);
				UpdateSpriteAnimated(&game->buildings[i].lifeParticle, dt);
				UpdateSpriteAnimated(&game->buildings[i].naughtySign, dt);
			}
		}

		float gravitySpeed = 128.0f;

		for(int i = 0; i < ARRAY_COUNT(game->presents); i++)
		{
			if(game->presents[i].active)
			{
				game->presents[i].sprite.x -= (scrollSpeed * 0.55f) * dt;
				game->presents[i].sprite.y -= gravitySpeed * dt;

				game->presents[i].aabb.x = game->presents[i].sprite.x;
				game->presents[i].aabb.y = game->presents[i].sprite.y;

				if(game->presents[i].sprite.y < -32.0f)
				{
					game->presents[i].active = false;

					game->lives--;
					game->combo = 0;
					UpdateHUD(game);

					CutePlaySound(ctx, &game->audioData.missed, false);

					SpawnMissParticle(game);

					if(game->lives <= 0)
					{
						GameOver(ctx, game, mainCamera);
					}
				}
			}
		}

		for(int i = 0; i < ARRAY_COUNT(game->presents); i++)
		{
			if(game->presents[i].active)
			{
				for(int j = 0; j < ARRAY_COUNT(game->buildings); j++)
				{
					if(game->buildings[j].active && game->buildings[j].hasChimney)
					{
						auto aabb0 = game->presents[i].aabb;
						auto aabb1 = game->buildings[j].chimneyAABB;

						if(aabb0.x < aabb1.x + aabb1.w &&
							aabb0.x + aabb0.w > aabb1.x &&
							aabb0.y < aabb1.y + aabb1.h &&
							aabb0.h + aabb0.y > aabb1.y)
						{
							game->presents[i].active = false;

							if(game->buildings[j].isNaughty)
							{
								game->combo = 0;
								game->lives--;

								CutePlaySound(ctx, &game->audioData.missed, false);

								SpawnMissParticle(game);
							}
							else
							{
								game->buildings[j].chimneyParticle.currentTime = 0.0f;

								game->combo += 1;
								game->score += game->combo;

								CutePlaySound(ctx, &game->audioData.hit, false);
							}

							UpdateHUD(game);
						}

						if(game->buildings[j].lifeActive)
						{
							auto aabb2 = game->buildings[j].lifeAABB;

							if(aabb0.x < aabb2.x + aabb2.w &&
								aabb0.x + aabb0.w > aabb2.x &&
								aabb0.y < aabb2.y + aabb2.h &&
								aabb0.h + aabb0.y > aabb2.y)
							{
								game->presents[i].active = false;

								game->buildings[j].lifeActive = false;
								game->lives = clampVal(0, 3, game->lives + 1);

								CutePlaySound(ctx, &game->audioData.pickup, false);

								UpdateHUD(game);
							}
						}
					}
				}
			}
		}

		game->nextBuildingXOffset -= scrollSpeed * dt;
		game->skyScrollX = fmod(game->skyScrollX - (scrollSpeed * 0.4f * dt), 160);
		game->backScrollX = fmod(game->backScrollX - (scrollSpeed * 0.5f * dt), 160);
		game->cloudScrollX = fmod(game->cloudScrollX - (scrollSpeed * 0.6f * dt), 160);

		if(game->santa.currentAnimationID == 1 &&
			game->santa.currentTime >= game->santa.animations[game->santa.currentAnimationID].animationTime)
		{
			game->santa.currentAnimationID = 0.0f;
			game->santa.currentTime = 0.0f;
		}

		UpdateSpriteAnimated(&game->santa, dt);

		// Snow particles
		for(int y = 0; y < ARRAY_COUNT(game->snowParticleY); y++)
		{
			game->snowParticleX[y] -= (scrollSpeed * 0.55f) * dt;
			game->snowParticleY[y] -= gravitySpeed * dt;

			if(game->snowParticleY[y] < 0.0f)
			{
				game->snowParticleX[y] = RandomMinMax(0, 256);
				game->snowParticleY[y] = RandomMinMax(144, 288);
			}
		}

		// Miss particles
		for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
		{
			if(game->missParticle[i].active)
			{
				if(game->missParticle[i].chimneyParticle.currentTime >= game->missParticle[i].chimneyParticle.animations[game->missParticle[i].chimneyParticle.currentAnimationID].animationTime)
				{
					game->missParticle[i].active = false;
				}

				UpdateSpriteAnimated(&game->missParticle[i].chimneyParticle, dt);
			}
		}
	}
}

void OnGameRender(CameraRect* mainCamera, ScreenData* sD, int screenSize)
{	
	if(game->gameState == GameSanta::GameState::Menu)
	{
		RenderSpriteAnimated(sD, game->menu, screenSize);

		if(game->flickerTimer < 0.5f)
		{
			RenderText(sD, game->titleText2, screenSize);
			RenderText(sD, game->titleText, screenSize);
		}
		
		if(game->flickerTimer > 0.25f && 
			game->flickerTimer < 0.75f)
		{
			RenderText(sD, game->playText, screenSize);
		}
	}
	else if(game->gameState == GameSanta::GameState::End)
	{
		RenderSpriteAnimated(sD, game->menu, screenSize);

		RenderText(sD, game->titleText2, screenSize);
		RenderText(sD, game->titleText, screenSize);

		if(game->flickerTimer > 0.25f &&
			game->flickerTimer < 0.75f)
		{
			RenderText(sD, game->playText, screenSize);
		}
	}
	else
	{
		RenderScrollSprite(sD, game->sky, screenSize, (int)game->skyScrollX);
		RenderScrollSprite(sD, game->backdrop, screenSize, (int)game->backScrollX);
		RenderScrollSprite(sD, game->clouds, screenSize, (int)game->cloudScrollX);
		RenderSpriteAnimated(sD, game->santa, screenSize);

		for(int i = 0; i < ARRAY_COUNT(game->presents); i++)
		{
			if(game->presents[i].active)
			{
				RenderSprite(sD, game->presents[i].sprite, screenSize);
			}
		}

		for(int i = 0; i < ARRAY_COUNT(game->buildings); i++)
		{
			if(!game->buildings[i].active)
			{
				continue;
			}

			for(int j = 0; j < game->buildings[i].blocksSize; j++)
			{
				RenderSprite(sD, game->buildings[i].buildingBlocks[j], screenSize);
				RenderSprite(sD, game->buildings[i].windows[j], screenSize);
			}

			if(game->buildings[i].hasChimney)
			{
				RenderSprite(sD, game->buildings[i].chimney, screenSize);
				RenderSpriteAnimated(sD, game->buildings[i].chimneyParticle, screenSize);

				if(game->buildings[i].isNaughty)
				{
					RenderSpriteAnimated(sD, game->buildings[i].naughtySign, screenSize);
				}

				if(game->buildings[i].lifeActive)
				{
					RenderSpriteAnimated(sD, game->buildings[i].lifeParticle, screenSize);
				}
			}

			RenderSprite(sD, game->buildings[i].roof, screenSize);
			RenderSprite(sD, game->buildings[i].foreground, screenSize);
		}

		for(int i = 0; i < game->lives; i++)
		{
			RenderSprite(sD, game->liveSprites[i], screenSize);
		}

		RenderText(sD, game->scoreText, screenSize);
		RenderText(sD, game->comboText, screenSize);

		for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
		{
			if(game->missParticle[i].active)
			{
				RenderSpriteAnimated(sD, game->missParticle[i].chimneyParticle, screenSize);
			}
		}

		// Snow particles
		for(int y = 0; y < ARRAY_COUNT(game->snowParticleY); y++)
		{
			auto spriteY = (mainCamera->height - (int)game->snowParticleY[y]);

			int screenX = mainCamera->screenX + game->snowParticleX[y];
			int screenY = mainCamera->screenY + spriteY;

			if(OutsideRect(mainCamera, screenX, screenY))
			{
				continue;
			}

			int screenID = (screenY * sD->nativeWidth) + screenX;

			if(!RenderPixelToScreen(sD, screenID, screenSize, 100, colorWhite))
			{
				continue;
			}
		}
	}
}