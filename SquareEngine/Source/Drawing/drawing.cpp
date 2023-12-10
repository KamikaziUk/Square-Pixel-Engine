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

#include "drawing.h"

#include "Utilities/math.h"

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"

namespace SquarePixelEngine
{
    Image LoadImageFromFile(const char* fileName)
    {
        Image newImage;
        unsigned char* img = stbi_load(fileName, &newImage.width, &newImage.height, &newImage.channels, 0);

        if(img == NULL)
        {
            printf("Can't load image file: ");
            printf("%s", fileName);
            assert(false);
            exit(1);
        }
        else
        {
            newImage.imageData = new Color[newImage.width * newImage.height];

            // Copy the texture data over to our own image format
            int currentID = 0;
            for(int i = 0; i < (newImage.width * newImage.height); i++)
            {
                Color color;
                color.r = img[currentID++];
                color.g = img[currentID++];
                color.b = img[currentID++];
                color.a = 255;

                if(newImage.channels >= 4)
                {
                    currentID++;
                }

                newImage.imageData[i] = color;
            }
        }

        stbi_image_free(img);

        return newImage;
    }

    void DestroyImageFromFile(Image* image)
    {
        delete[] image->imageData;
        image->imageData = NULL;
    }

    void LoadAnimation(SpriteAnimated* animSprite, const char* fileName)
    {
        char readLine[512];
        FILE* fp = nullptr;
        fopen_s(&fp, fileName, "r");

        if(fp == NULL)
        {
            printf("Can't load animation file: ");
            printf("%s", fileName);
            assert(false);
            exit(1);
        }
        else
        {
            int currentAnim = 0;

            while(fgets(readLine, 512, fp) != NULL)
            {
                int endElement = 0;
                for(int i = 0; i < 512; i++)
                {
                    if(readLine[i] == '\n')
                    {
                        endElement = i - 1;
                        break;
                    }
                }

                // Read number of animations (such as idle, move etc)
                const char* headerName = "AnimCount";
                if(strstr(readLine, headerName))
                {
                    int headerNumber = (int)strlen(headerName);
                    int numberSize = endElement - headerNumber;

                    char* number = new char[numberSize];
                    memcpy(number, &readLine[headerNumber + 1], numberSize);
                    int numberInt = atoi(number);

                    animSprite->animationCount = numberInt;
                    animSprite->animations = new SpriteAnimationFrames[animSprite->animationCount];

                    delete[] number;
                }

                // Read number of frames in the sheet
                const char* headerName2 = "FrameCount";
                if(strstr(readLine, headerName2))
                {
                    int headerNumber = (int)strlen(headerName2);
                    int numberSize = endElement - headerNumber;

                    char* number = new char[numberSize];
                    memcpy(number, &readLine[headerNumber + 1], numberSize);
                    int numberInt = atoi(number);

                    animSprite->animations[currentAnim].frameCount = numberInt;
                    animSprite->animations[currentAnim].frames = new int[animSprite->animations[currentAnim].frameCount];

                    delete[] number;
                }

                // Read the ID's for the tiles to make the animation
                const char* headerName3 = "FrameIDs";
                if(strstr(readLine, headerName3))
                {
                    int headerNumber = (int)strlen(headerName3);

                    int currentID = headerNumber + 1;
                    for(int j = 0; j < animSprite->animations[currentAnim].frameCount; j++)
                    {
                        int numberSize = 0;
                        for(int k = 0; k < 512; k++)
                        {
                            if(readLine[currentID + k] == ',' || readLine[currentID + k] == '\n')
                            {
                                numberSize = k;
                                break;
                            }
                        }

                        char* number = new char[numberSize + 1];
                        memcpy(number, &readLine[currentID], numberSize);
                        number[numberSize] = '\0';

                        int numberInt = atoi(number);

                        animSprite->animations[currentAnim].frames[j] = numberInt;

                        delete[] number;

                        currentID += numberSize + 1;
                    }
                }

                // Total time for animation to play
                const char* headerName4 = "AnimTime";
                if(strstr(readLine, headerName4))
                {
                    int headerNumber = (int)strlen(headerName4);
                    int numberSize = endElement - headerNumber;

                    char* number = new char[numberSize];
                    memcpy(number, &readLine[headerNumber + 1], numberSize);
                    float numberFloat = (float)atof(number);

                    animSprite->animations[currentAnim].animationTime = numberFloat;

                    delete[] number;
                }

                // Should we loop or be a one shot
                const char* headerName5 = "Loop";
                if(strstr(readLine, headerName5))
                {
                    int headerNumber = (int)strlen(headerName5);
                    int numberSize = endElement - headerNumber;

                    char* number = new char[numberSize];
                    memcpy(number, &readLine[headerNumber + 1], numberSize);
                    int numberInt = atoi(number);

                    animSprite->animations[currentAnim].isLoop = numberInt == 1;

                    delete[] number;
                }

                // End of animation (go to next animation if we have one in file)
                const char* headerName6 = "End";
                if(strstr(readLine, headerName6))
                {
                    currentAnim++;
                }
            }

            fclose(fp);
        }
    }

    // Check if image is in front of depth sorting
    bool SortOrder(const ScreenData* screenData, const int screenX, const int screenY, const int sortOrder)
    {
        int screenID = (screenY * screenData->nativeWidth) + screenX;

        if(sortOrder >= screenData->sortDepthBuffer[screenID])
        {
            return true;
        }

        return false;
    }

    bool OutsideRect(const CameraRect* cameraRect, const int screenX, const int screenY)
    {
        if((screenX < cameraRect->screenX || screenX >= cameraRect->screenX + cameraRect->width) ||
            (screenY < cameraRect->screenY || screenY >= cameraRect->screenY + cameraRect->height))
        {
            return true;
        }

        return false;
    }

    // Render pixel to screen
    bool RenderPixelToScreen(ScreenData* screenData, 
        const int pixelID, const int screenSize, const int sortOrder, const Color tintColor)
    {
        if(pixelID > 0 && pixelID < screenSize)
        {
            screenData->sortDepthBuffer[pixelID] = sortOrder;

            // TODO: Improve this with memcpy
            ((char*)screenData->memory)[(pixelID * 4)] = (char)tintColor.b;
            ((char*)screenData->memory)[(pixelID * 4) + 1] = (char)tintColor.g;
            ((char*)screenData->memory)[(pixelID * 4) + 2] = (char)tintColor.r;
            ((char*)screenData->memory)[(pixelID * 4) + 3] = (char)0;
        }

        return true;
    }

    // Render pixel from image to screen
    bool RenderImagePixelToScreen(ScreenData* screenData, 
        const int pixelID, const int screenSize,
        const Color* imageData, const int spriteArrayID, const int sortOrder, const Color* tintColor)
    {
        if(pixelID > 0 && pixelID < screenSize)
        {
            // Alpha cutout color
            if(imageData[spriteArrayID].r == 0 && imageData[spriteArrayID].g == 255 && imageData[spriteArrayID].b == 0)
            {
                return false;
            }

            Color pixelColor;
            pixelColor.a = 255;

            if(tintColor != nullptr)
            {
                pixelColor.r = (int)((float)imageData[spriteArrayID].r * ((float)tintColor->r / 255.0f));
                pixelColor.g = (int)((float)imageData[spriteArrayID].g * ((float)tintColor->g / 255.0f));
                pixelColor.b = (int)((float)imageData[spriteArrayID].b * ((float)tintColor->b / 255.0f));
            }
            else
            {
                pixelColor.r = (int)((float)imageData[spriteArrayID].r);
                pixelColor.g = (int)((float)imageData[spriteArrayID].g);
                pixelColor.b = (int)((float)imageData[spriteArrayID].b);
            }

            RenderPixelToScreen(screenData, pixelID, screenSize, sortOrder, pixelColor);
        }

        return true;
    }

    void RenderBox(ScreenData* screenData, 
        const CameraRect* cameraRect, const float xIn, const float yIn, const float w, const float h, const int screenSize)
    {
        int cameraOffsetX = cameraRect->screenX;
        int cameraOffsetY = cameraRect->screenY;

        auto spriteY = (int)yIn;

        // Flip Y so coordinates are easier to understand
        if(cameraRect->gameCamera)
        {
            spriteY = (cameraRect->height - (int)yIn) - (int)h;
        }

        for(int y = 0; y < (int)h; y++)
        {
            for(int x = 0; x < (int)w; x++)
            {
                int screenX = ((int)xIn + x) + cameraOffsetX;
                int screenY = (spriteY + y) + cameraOffsetY;

                if(OutsideRect(cameraRect, screenX, screenY) ||
                    !SortOrder(screenData, screenX, screenY, 100))
                {
                    continue;
                }

                int screenID = (screenY * screenData->nativeWidth) + screenX;
                int arrayID = (y * (int)w) + x;

                if(!RenderPixelToScreen(screenData, screenID, screenSize, 100, colorGreen))
                {
                    continue;
                }
            }
        }
    }

    // Scrolling sprite such as a background that repeats
    void RenderScrollSprite(ScreenData* screenData, 
        const Sprite sprite, const int screenSize, const int scrollOffset)
    {
        Image* image = sprite.image;

        int cameraOffsetX = sprite.cameraRect->screenX;
        int cameraOffsetY = sprite.cameraRect->screenY;

        auto spriteY = (int)sprite.y;

        // Flip Y so coordinates are easier to understand
        if(sprite.cameraRect->gameCamera)
        {
            spriteY = (sprite.cameraRect->height - (int)sprite.y) - image->height;
        }

        for(int y = 0; y < image->height; y++)
        {
            for(int x = 0; x < image->width; x++)
            {
                int texX = (x + scrollOffset);

                if(texX < 0)
                {
                    texX = image->width - abs(texX);
                }
                else if(texX > image->width - 1)
                {
                    texX = image->width - texX;
                }

                int screenX = ((int)sprite.x + texX) + cameraOffsetX;
                int screenY = (spriteY + y) + cameraOffsetY;

                if(OutsideRect(sprite.cameraRect, screenX, screenY) ||
                    !SortOrder(screenData, screenX, screenY, sprite.sortOrder))
                {
                    continue;
                }

                int screenID = (screenY * screenData->nativeWidth) + screenX;
                int arrayID = (y * image->width) + x;

                if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, sprite.sortOrder, nullptr))
                {
                    continue;
                }
            }
        }
    }

    // Single sprite (non-animated)
    void RenderSprite(ScreenData* screenData, 
        const Sprite sprite, const int screenSize)
    {
        Image* image = sprite.image;

        int cameraOffsetX = sprite.cameraRect->screenX;
        int cameraOffsetY = sprite.cameraRect->screenY;

        auto spriteY = (int)sprite.y;

        // Flip Y so coordinates are easier to understand
        if(sprite.cameraRect->gameCamera)
        {
            spriteY = (sprite.cameraRect->height - (int)sprite.y) - image->height;
        }

        for(int y = 0; y < image->height; y++)
        {
            for(int x = 0; x < image->width; x++)
            {
                int screenX = ((int)sprite.x + x) + cameraOffsetX;
                int screenY = (spriteY + y) + cameraOffsetY;

                if(OutsideRect(sprite.cameraRect, screenX, screenY) ||
                    !SortOrder(screenData, screenX, screenY, sprite.sortOrder))
                {
                    continue;
                }

                int screenID = (screenY * screenData->nativeWidth) + screenX;
                int arrayID = (y * image->width) + x;

                if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, sprite.sortOrder, nullptr))
                {
                    continue;
                }
            }
        }
    }

    // Sprite sheet animation
    void RenderSpriteAnimated(ScreenData* screenData, 
        const SpriteAnimated spriteAnim, const int screenSize)
    {
        Image* image = spriteAnim.image;

        int cameraOffsetX = spriteAnim.cameraRect->screenX;
        int cameraOffsetY = spriteAnim.cameraRect->screenY;

        // Calculate current tile ID
        float animTime01 = Utilities::Clamp(0.0f, 1.0f, (float)spriteAnim.currentTime / (float)spriteAnim.animations[spriteAnim.currentAnimationID].animationTime);
        int frameID = (int)floorf(animTime01 * (float)(spriteAnim.animations[spriteAnim.currentAnimationID].frameCount - 1));
        int tileID = spriteAnim.animations[spriteAnim.currentAnimationID].frames[frameID];

        // Calculate the x & y offset in pixels
        int tileAmountWidth = image->width / spriteAnim.tileSizeX;
        int xOffset = (tileID % tileAmountWidth) * spriteAnim.tileSizeX;

        int tileAmountHeight = image->height / spriteAnim.tileSizeY;
        int yOffset = (tileID / tileAmountWidth) * spriteAnim.tileSizeY;

        auto spriteY = (int)spriteAnim.y;

        // Flip Y so coordinates are easier to understand
        if(spriteAnim.cameraRect->gameCamera)
        {
            spriteY = (spriteAnim.cameraRect->height - (int)spriteAnim.y) - spriteAnim.tileSizeY;
        }

        for(int y = 0; y < spriteAnim.tileSizeY; y++)
        {
            for(int x = 0; x < spriteAnim.tileSizeX; x++)
            {
                int screenX = ((int)spriteAnim.x + x) + cameraOffsetX;
                int screenY = (spriteY + y) + cameraOffsetY;

                if(OutsideRect(spriteAnim.cameraRect, screenX, screenY) ||
                    !SortOrder(screenData, screenX, screenY, spriteAnim.sortOrder))
                {
                    continue;
                }

                int screenID = (screenY * screenData->nativeWidth) + screenX;
                int arrayID = ((y + yOffset) * image->width) + (x + xOffset);

                if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, spriteAnim.sortOrder, nullptr))
                {
                    continue;
                }
            }
        }
    }

    // Render text from font image
    void RenderText(ScreenData* screenData, 
        const Text text, const int screenSize)
    {
        Image* image = text.image;

        int cameraOffsetX = text.cameraRect->screenX;
        int cameraOffsetY = text.cameraRect->screenY;

        auto spriteY = (int)text.y;

        // Flip Y so coordinates are easier to understand
        if(text.cameraRect->gameCamera)
        {
            spriteY = (text.cameraRect->height - (int)text.y) - text.characterSize;
        }

        for(int j = 0; j < text.stringLength; j++)
        {
            char character = text.stringArray[j];
            int characterID = -1;

            // PERF TODO: Do this when setting text up rather than per frame
            if((int)character >= 48 && (int)character <= 57)
            {
                // Numbers (123)
                characterID = (int)character - 48;
            }
            else if((int)character >= 65 && (int)character <= 90)
            {
                // Uppercase (ABC)
                // TODO: Support upper case on fonts
                characterID = (int)character - 55;
            }
            else if((int)character >= 97 && (int)character <= 122)
            {
                // Lowercase (abc)
                characterID = (int)character - 87;
            }

            if(characterID == -1)
            {
                continue;
            }

            int tileAmountWidth = image->width / text.characterSize;
            int xOffset = (characterID % tileAmountWidth) * text.characterSize;
            int yOffset = ((int)floorf((float)(characterID / tileAmountWidth))) * text.characterSize;

            int screenOffsetX = text.kerning * j;

            for(int y = 0; y < text.characterSize; y++)
            {
                for(int x = 0; x < text.characterSize; x++)
                {
                    int screenX = ((int)text.x + x + screenOffsetX) + cameraOffsetX;

                    // TODO: Add leading kerning
                    int screenY = (spriteY + y) + cameraOffsetY;

                    if(OutsideRect(text.cameraRect, screenX, screenY) ||
                        !SortOrder(screenData, screenX, screenY, text.sortOrder))
                    {
                        continue;
                    }

                    int screenID = (screenY * screenData->nativeWidth) + screenX;
                    int arrayID = ((y + yOffset) * image->width) + (x + xOffset);

                    if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, text.sortOrder, &text.mainColor))
                    {
                        continue;
                    }
                }
            }
        }
    }

    // Update timers for sprite animated's
    void UpdateSpriteAnimated(SpriteAnimated* spriteAnim, const float dt)
    {
        if(spriteAnim->animations[spriteAnim->currentAnimationID].isLoop)
        {
            spriteAnim->currentTime = fmodf(spriteAnim->currentTime + dt, spriteAnim->animations[spriteAnim->currentAnimationID].animationTime);
        }
        else
        {
            if(spriteAnim->currentTime < spriteAnim->animations[spriteAnim->currentAnimationID].animationTime)
            {
                spriteAnim->currentTime = spriteAnim->currentTime + dt;
            }
            else
            {
                spriteAnim->currentTime = spriteAnim->animations[spriteAnim->currentAnimationID].animationTime;
            }
        }
    }
}