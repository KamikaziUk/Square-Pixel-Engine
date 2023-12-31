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

#include "game_santa.h"

using namespace SquarePixelEngine;
using namespace Utilities;

#include "Utilities/math.h"
#include "Utilities/carray.h"

namespace SantaGame
{
	GameData* game = nullptr;

	void GenerateBuilding(GameData* game, CameraRect* mainCamera)
	{
		int currentBuildingId = -1;

		// Find in pool
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

		// Choose the best roof that fits size
		if(building->widthSize == 1)
		{
			building->roof = Sprite(game->nextBuildingXOffset - tileSize,
				((float)(building->heightSize - 1) * 16.0f), 1,
				&game->buildingImages.roofImage1[0], mainCamera);
		}
		else if(building->widthSize == 2)
		{
			building->roof = Sprite(game->nextBuildingXOffset - tileSize,
				((float)(building->heightSize - 1) * 16.0f), 1,
				&game->buildingImages.roofImage2[0], mainCamera);
		}
		else if(building->widthSize == 3)
		{
			building->roof = Sprite(game->nextBuildingXOffset - tileSize,
				((float)(building->heightSize - 1) * 16.0f), 1,
				&game->buildingImages.roofImage3[0], mainCamera);
		}
		else if(building->widthSize == 4)
		{
			building->roof = Sprite(game->nextBuildingXOffset - tileSize,
				((float)(building->heightSize - 1) * 16.0f), 1,
				&game->buildingImages.roofImage4[0], mainCamera);
		}

		// Random chimney placement
		int randomChimney = building->widthSize > 1 ? RandomMinMax(0, building->widthSize - 1) : 0;
		building->chimney = Sprite(
			game->nextBuildingXOffset + ((float)randomChimney * 16.0f),
			((float)(building->heightSize) * 16.0f), 1, &game->buildingImages.chimneyImage, mainCamera);

		building->chimneyParticle.x = building->chimney.x;
		building->chimneyParticle.y = building->chimney.y + 16;
		building->chimneyParticle.currentTime = 
			building->chimneyParticle.animations[building->chimneyParticle.currentAnimationID].animationTime;

		building->lifeParticle.x = building->chimney.x;
		building->lifeParticle.y = building->chimney.y + 16;
		building->lifeParticle.currentTime = 
			building->lifeParticle.animations[
				building->lifeParticle.currentAnimationID].animationTime * ((float)RandomMinMax(0, 100) / 100.0f);

		building->naughtySign.x = building->chimney.x;
		building->naughtySign.y = building->chimney.y;
		building->naughtySign.currentTime = 
			building->naughtySign.animations[
				building->naughtySign.currentAnimationID].animationTime * ((float)RandomMinMax(0, 100) / 100.0f);

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
			// Place naughty sign
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

			// Place life
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

		building->foreground = 
			Sprite(game->nextBuildingXOffset + ((float)(building->widthSize) * 16.0f), 0, 1, 
				   &game->buildingImages.foregroundImages[RandomMinMax(0, 2)], mainCamera);

		building->blocksSize = building->widthSize * building->heightSize;

		bool buildingType = RandomMinMax(0, 100) < 50;

		// Random building with random windows
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
					building->buildingBlocks[arrayID] = 
						Sprite(game->nextBuildingXOffset + ((float)x * 16.0f), ((float)y * 16.0f), 1, 
							&game->buildingImages.buildingBlockImage[RandomMinMax(0, 1)], mainCamera);
				}
				else
				{
					building->buildingBlocks[arrayID] = 
						Sprite(game->nextBuildingXOffset + ((float)x * 16.0f), ((float)y * 16.0f), 1, 
							&game->buildingImages.buildingBlockImage[RandomMinMax(2, 3)], mainCamera);
				}

				building->windows[arrayID] = 
					Sprite(game->nextBuildingXOffset + ((float)x * 16.0f), ((float)y * 16.0f), 1, 
						&game->buildingImages.windowImages[windowId], mainCamera);
			}
		}

		game->nextBuildingXOffset += tileSize * (building->widthSize + 1);
	}

	void OnGameStart(SoundData* soundData, CameraRect* mainCamera)
	{
		game = new GameData();

		game->gameState = GameState::Menu;

		// Setup audio
		soundData->result = ma_engine_init(NULL, &soundData->engine);
		if(soundData->result != MA_SUCCESS)
		{
			printf("failed to load mini audio\n");
		}

		const auto colorTitle = Color(255, 233, 0);

		// Load images
		game->santaImage = LoadImageFromFile("SantaGame/Sprites/Santa.png");
		game->skyImage = LoadImageFromFile("SantaGame/Sprites/Sky.png");
		game->backdropImage = LoadImageFromFile("SantaGame/Sprites/Backdrop.png");
		game->cloudImage = LoadImageFromFile("SantaGame/Sprites/Clouds.png");
		game->moonImage = LoadImageFromFile("SantaGame/Sprites/Moon.png");
		game->menuImage = LoadImageFromFile("SantaGame/Sprites/Menu.png");
		game->lifeImage = LoadImageFromFile("SantaGame/Sprites/Life.png");

		game->presentImages[0] = LoadImageFromFile("SantaGame/Sprites/Present0.png");
		game->presentImages[1] = LoadImageFromFile("SantaGame/Sprites/Present1.png");
		game->presentImages[2] = LoadImageFromFile("SantaGame/Sprites/Present2.png");

		// Load sprite data
		for(int i = 0; i < ARRAY_COUNT(game->liveSprites); i++)
		{
			game->liveSprites[i] = 
				Sprite(160 - (((float)(i + 1) * 10) + 8), 144 - 16, 0, &game->lifeImage, mainCamera);
		}

		game->sky = Sprite(0, 0, 0, &game->skyImage, mainCamera);
		game->backdrop = Sprite(0, 0, 1, &game->backdropImage, mainCamera);
		game->clouds = Sprite(0, 0, 1, &game->cloudImage, mainCamera);

		game->santa = SpriteAnimated(48, 80, 64, 32, 1, &game->santaImage, mainCamera);
		LoadAnimation(&game->santa, "SantaGame/AnimFiles/Santa.anim");

		game->menu = SpriteAnimated(0, 0, 160, 144, 1, &game->menuImage, mainCamera);
		LoadAnimation(&game->menu, "SantaGame/AnimFiles/Menu.anim");

		// Load font
		game->textFont = LoadImageFromFile("SantaGame/Fonts/Font16.png");

		// Load text
		sprintf_s(game->scoreStr, "SCORE 0");
		game->scoreText = Text(0, 144 - 16, 6, 6, 7, game->scoreStr, 16, 10, colorWhite, &game->textFont, mainCamera);

		sprintf_s(game->comboStr, "COMBO 0");
		game->comboText = Text(0, 144 - 24, 6, 6, 7, game->comboStr, 16, 10, colorWhite, &game->textFont, mainCamera);

		sprintf_s(game->playStr, "PLAY");
		game->playText = Text(88, 144 - 54, 6, 6, 4, game->playStr, 16, 10, colorWhite, &game->textFont, mainCamera);

		sprintf_s(game->titleStr, "KRINGLES");
		game->titleText = Text(76, 144 - 25, 6, 6, 8, game->titleStr, 16, 10, colorTitle, &game->textFont, mainCamera);

		sprintf_s(game->titleStr2, "DELIVERY");
		game->titleText2 = Text(76, 144 - 35, 6, 6, 17, game->titleStr2, 16, 10, colorTitle, &game->textFont, mainCamera);

		// Load building images
		game->buildingImages.roofImage1[0] = LoadImageFromFile("SantaGame/Sprites/RoofTile1_0.png");
		game->buildingImages.roofImage2[0] = LoadImageFromFile("SantaGame/Sprites/RoofTile2_0.png");
		game->buildingImages.roofImage3[0] = LoadImageFromFile("SantaGame/Sprites/RoofTile3_0.png");
		game->buildingImages.roofImage4[0] = LoadImageFromFile("SantaGame/Sprites/RoofTile4_0.png");

		game->buildingImages.chimneyImage = LoadImageFromFile("SantaGame/Sprites/Chimney0.png");

		game->buildingImages.buildingBlockImage[0] = LoadImageFromFile("SantaGame/Sprites/BuildingBlock0.png");
		game->buildingImages.buildingBlockImage[1] = LoadImageFromFile("SantaGame/Sprites/BuildingBlock1.png");
		game->buildingImages.buildingBlockImage[2] = LoadImageFromFile("SantaGame/Sprites/BuildingBlock2.png");
		game->buildingImages.buildingBlockImage[3] = LoadImageFromFile("SantaGame/Sprites/BuildingBlock3.png");

		game->buildingImages.windowImages[0] = LoadImageFromFile("SantaGame/Sprites/Window0.png");
		game->buildingImages.windowImages[1] = LoadImageFromFile("SantaGame/Sprites/Window1.png");
		game->buildingImages.windowImages[2] = LoadImageFromFile("SantaGame/Sprites/Window2.png");

		game->buildingImages.foregroundImages[0] = LoadImageFromFile("SantaGame/Sprites/Foreground0.png");
		game->buildingImages.foregroundImages[1] = LoadImageFromFile("SantaGame/Sprites/Foreground1.png");
		game->buildingImages.foregroundImages[2] = LoadImageFromFile("SantaGame/Sprites/Foreground2.png");

		game->buildingImages.chimneyParticleImage = LoadImageFromFile("SantaGame/Sprites/ChimneyParticle.png");
		game->buildingImages.lifeParticleImage = LoadImageFromFile("SantaGame/Sprites/LifePickup.png");
		game->buildingImages.naughtySignImage = LoadImageFromFile("SantaGame/Sprites/NaughtySign.png");

		// Load building props
		for(int i = 0; i < ARRAY_COUNT(game->buildings); i++)
		{
			game->buildings[i].chimneyParticle = 
				SpriteAnimated(0, 0, 16, 16, 2, &game->buildingImages.chimneyParticleImage, mainCamera);
			LoadAnimation(&game->buildings[i].chimneyParticle, "SantaGame/AnimFiles/ChimneyParticle.anim");

			game->buildings[i].lifeParticle = 
				SpriteAnimated(0, 0, 16, 16, 3, &game->buildingImages.lifeParticleImage, mainCamera);
			LoadAnimation(&game->buildings[i].lifeParticle, "SantaGame/AnimFiles/LifePickup.anim");

			game->buildings[i].naughtySign = 
				SpriteAnimated(0, 0, 32, 16, 3, &game->buildingImages.naughtySignImage, mainCamera);
			LoadAnimation(&game->buildings[i].naughtySign, "SantaGame/AnimFiles/NaughtySign.anim");

			GenerateBuilding(game, mainCamera);
		}

		// Load missed particle effects
		game->missParticleImage = LoadImageFromFile("SantaGame/Sprites/MissParticle.png");
		for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
		{
			game->missParticle[i].chimneyParticle = SpriteAnimated(0, 0, 16, 16, 4, &game->missParticleImage, mainCamera);
			LoadAnimation(&game->missParticle[i].chimneyParticle, "SantaGame/AnimFiles/MissParticle.anim");
		}

		// Randomize snow particles
		for(int y = 0; y < ARRAY_COUNT(game->snowParticleY); y++)
		{
			game->snowParticleX[y] = (float)RandomMinMax(0, 256);
			game->snowParticleY[y] = (float)RandomMinMax(144, 288);
		}

		ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Music.wav", NULL);

		game->score = 0;
		game->lives = ARRAY_COUNT(game->liveSprites);
	}

	void OnGameEnd(SoundData* soundData, CameraRect* mainCamera)
	{
		if(game == nullptr)
		{
			return;
		}

		ma_engine_uninit(&soundData->engine);

		delete game;
		game = nullptr;
	}

	void UpdateHUD(GameData* game)
	{
		sprintf_s(game->scoreStr, "SCORE %d", game->score);
		game->scoreText.stringLength = 6 + ((game->score / 10) + 1);

		sprintf_s(game->comboStr, "COMBO %d", game->combo);
		game->comboText.stringLength = 6 + ((game->combo / 10) + 1);
	}

	// Update final UI & play sound when game over
	void GameOver(SoundData* soundData, GameData* game)
	{
		game->gameState = GameState::End;

		sprintf_s(game->titleStr, "LAST %d", game->score);
		game->titleText.stringLength = 6 + ((game->score / 10) + 1);

		if(game->score > game->bestScore)
		{
			game->bestScore = game->score;
		}

		sprintf_s(game->titleStr2, "BEST %d", game->bestScore);
		game->titleText2.stringLength = 6 + ((game->bestScore / 10) + 1);

		ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Lost.wav", NULL);

		game->delayTimer = 0.5f;
	}

	// Spawn present that falls
	void DropPresent(SoundData* soundData, GameData* game, CameraRect* mainCamera)
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

				ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Drop.wav", NULL);
				break;
			}
		}
	}

	// Spawn miss particle from pool
	void SpawnMissParticle(GameData* game)
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

	void RestartGame(InputData* inputData, SoundData* soundData)
	{
		game->gameState = GameState::Game;
		game->delayTimer = 0.5f;

		game->lives = 3;
		game->score = 0;
		game->combo = 0;
		game->gameTimer = 0.0f;

		ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/ButtonUI.wav", NULL);

		for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
		{
			game->missParticle[i].active = false;
		}
	}

	bool OnGameUpdate(InputData* inputData, SoundData* soundData, CameraRect* mainCamera, float deltaTime)
	{
		game->gameTimer += deltaTime;
		game->delayTimer -= deltaTime;

		if(game->gameState == GameState::Menu)
		{
			if(GetMenuButton(inputData) == ButtonState::Down)
			{
				return true;
			}

			// Start the game
			if(game->delayTimer <= 0.0f && GetActionButton(inputData) == ButtonState::Down)
			{
				game->gameState = GameState::Game;
				RestartGame(inputData, soundData);
			}

			// Show a little santa animation & flicker text
			UpdateSpriteAnimated(&game->menu, deltaTime);

			game->flickerTimer += deltaTime * 0.5f;

			if(game->flickerTimer > 1.0f)
			{
				game->flickerTimer = 0.0f;
			}
		}
		else if(game->gameState == GameState::End)
		{
			// Restart the game
			if(game->delayTimer <= 0.0f && GetActionButton(inputData) == ButtonState::Down)
			{
				RestartGame(inputData, soundData);
			}

			// Animated background
			UpdateSpriteAnimated(&game->menu, deltaTime);

			game->flickerTimer += deltaTime * 0.5f;

			if(game->flickerTimer > 1.0f)
			{
				game->flickerTimer = 0.0f;
			}
		}
		else
		{
			if(GetMenuButton(inputData) == ButtonState::Down)
			{
				game->gameState = GameState::Menu;
				game->delayTimer = 0.5f;
			}

			// Drop present when user presses button
			if(game->delayTimer <= 0.0f && GetActionButton(inputData) == ButtonState::Down)
			{
				DropPresent(soundData, game, mainCamera);
			}

			// Scroll buildings and generate new ones
			const auto scrollSpeed = 96.0f + (66.0f * Clamp(0.0f, 1.0f, game->gameTimer / 90.0f));
			for(int i = 0; i < ARRAY_COUNT(game->buildings); i++)
			{
				if(game->buildings[i].active)
				{
					for(int j = 0; j < game->buildings[i].blocksSize; j++)
					{
						game->buildings[i].buildingBlocks[j].x -= scrollSpeed * deltaTime;
						game->buildings[i].windows[j].x -= scrollSpeed * deltaTime;
					}

					game->buildings[i].chimney.x -= scrollSpeed * deltaTime;
					game->buildings[i].roof.x -= scrollSpeed * deltaTime;
					game->buildings[i].foreground.x -= scrollSpeed * deltaTime;

					if(game->buildings[i].hasChimney)
					{
						game->buildings[i].chimneyParticle.x -= scrollSpeed * deltaTime;
						game->buildings[i].lifeParticle.x -= scrollSpeed * deltaTime;
						game->buildings[i].naughtySign.x -= scrollSpeed * deltaTime;
						game->buildings[i].chimneyAABB.x -= scrollSpeed * deltaTime;
						game->buildings[i].lifeAABB.x -= scrollSpeed * deltaTime;
					}

					if(game->buildings[i].buildingBlocks->x <= -(6 * 32))
					{
						game->buildings[i].active = false;
						GenerateBuilding(game, mainCamera);
					}

					UpdateSpriteAnimated(&game->buildings[i].chimneyParticle, deltaTime);
					UpdateSpriteAnimated(&game->buildings[i].lifeParticle, deltaTime);
					UpdateSpriteAnimated(&game->buildings[i].naughtySign, deltaTime);
				}
			}

			// Fall (gravitySpeed) pixels per second
			const float gravitySpeed = 128.0f;
			for(int i = 0; i < ARRAY_COUNT(game->presents); i++)
			{
				if(game->presents[i].active)
				{
					game->presents[i].sprite.x -= (scrollSpeed * 0.55f) * deltaTime;
					game->presents[i].sprite.y -= gravitySpeed * deltaTime;

					game->presents[i].aabb.x = game->presents[i].sprite.x;
					game->presents[i].aabb.y = game->presents[i].sprite.y;

					// Gone off screen, so we reduce lives
					if(game->presents[i].sprite.y < -32.0f)
					{
						game->presents[i].active = false;

						game->lives--;
						game->combo = 0;
						UpdateHUD(game);

						ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Missed.wav", NULL);

						SpawnMissParticle(game);

						if(game->lives <= 0)
						{
							GameOver(soundData, game);
						}
					}
				}
			}

			// Update presents to see if we hit chimney
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

							// Inside bounds
							if(aabb0 < aabb1)
							{
								game->presents[i].active = false;

								// We hit the naughty sign, so reduce lives
								if(game->buildings[j].isNaughty)
								{
									game->combo = 0;
									game->lives--;

									ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Missed.wav", NULL);

									SpawnMissParticle(game);
								}
								else
								{
									// We hit the normal chimney, so increase score
									game->buildings[j].chimneyParticle.currentTime = 0.0f;

									game->combo += 1;
									game->score += game->combo;

									ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Hit.wav", NULL);
								}

								UpdateHUD(game);
							}

							if(game->buildings[j].lifeActive)
							{
								auto aabb2 = game->buildings[j].lifeAABB;

								// Inside bounds of the extra life powerup, so reduce lives
								if(aabb0 < aabb2)
								{
									game->presents[i].active = false;

									game->buildings[j].lifeActive = false;
									game->lives = Clamp(0, 3, game->lives + 1);

									ma_engine_play_sound(&soundData->engine, "SantaGame/Sounds/Pickup.wav", NULL);

									UpdateHUD(game);
								}
							}
						}
					}
				}
			}

			// Scroll backdrop
			game->nextBuildingXOffset -= scrollSpeed * deltaTime;
			game->skyScrollX = (float)fmod(game->skyScrollX - (scrollSpeed * 0.4f * deltaTime), 160);
			game->backScrollX = (float)fmod(game->backScrollX - (scrollSpeed * 0.5f * deltaTime), 160);
			game->cloudScrollX = (float)fmod(game->cloudScrollX - (scrollSpeed * 0.6f * deltaTime), 160);

			if(game->santa.currentAnimationID == 1 &&
				game->santa.currentTime >= game->santa.animations[game->santa.currentAnimationID].animationTime)
			{
				game->santa.currentAnimationID = 0;
				game->santa.currentTime = 0.0f;
			}

			// Update santa character animations
			UpdateSpriteAnimated(&game->santa, deltaTime);

			// Snow particles
			for(int y = 0; y < ARRAY_COUNT(game->snowParticleY); y++)
			{
				game->snowParticleX[y] -= (scrollSpeed * 0.55f) * deltaTime;
				game->snowParticleY[y] -= gravitySpeed * deltaTime;

				if(game->snowParticleY[y] < 0.0f)
				{
					game->snowParticleX[y] = (float)RandomMinMax(0, 256);
					game->snowParticleY[y] = (float)RandomMinMax(144, 288);
				}
			}

			// Miss particles
			for(int i = 0; i < ARRAY_COUNT(game->missParticle); i++)
			{
				if(game->missParticle[i].active)
				{
					if(game->missParticle[i].chimneyParticle.currentTime >= 
						game->missParticle[i].chimneyParticle.animations[
							game->missParticle[i].chimneyParticle.currentAnimationID].animationTime)
					{
						game->missParticle[i].active = false;
					}

					UpdateSpriteAnimated(&game->missParticle[i].chimneyParticle, deltaTime);
				}
			}
		}

		return false;
	}

	void OnGameRender(CameraRect* mainCamera, ScreenData* sD, int screenSize)
	{
		// Render main menu
		if(game->gameState == GameState::Menu)
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
		// Render game over screen
		else if(game->gameState == GameState::End)
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
		// Render game view
		else
		{
			// Render background
			RenderScrollSprite(sD, game->sky, screenSize, (int)game->skyScrollX);
			RenderScrollSprite(sD, game->backdrop, screenSize, (int)game->backScrollX);
			RenderScrollSprite(sD, game->clouds, screenSize, (int)game->cloudScrollX);
			RenderSpriteAnimated(sD, game->santa, screenSize);

			// Render presents
			for(int i = 0; i < ARRAY_COUNT(game->presents); i++)
			{
				if(game->presents[i].active)
				{
					RenderSprite(sD, game->presents[i].sprite, screenSize);
				}
			}

			// Render buildings
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

			// Render UI
			for(int i = 0; i < game->lives; i++)
			{
				RenderSprite(sD, game->liveSprites[i], screenSize);
			}

			RenderText(sD, game->scoreText, screenSize);
			RenderText(sD, game->comboText, screenSize);

			// Render miss particles
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

				int screenX = mainCamera->screenX + (int)game->snowParticleX[y];
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
}