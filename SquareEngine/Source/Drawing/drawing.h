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
#ifndef DRAWING
#define DRAWING

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace SquarePixelEngine
{
    struct Color
    {
        Color() { r = 0; g = 0; b = 0; a = 255; }
        Color(int inR, int inG, int inB)
        {
            r = inR;
            g = inG;
            b = inB;
            a = 255;
        }

        int b;
        int g;
        int r;
        int a;
    };

    #define colorWhite Color(255, 255, 255)
    #define colorGreen Color(0, 155, 0)

    struct CameraRect
    {
        CameraRect()
        {
            screenX = 0; 
            screenY = 0;
            width = 0; 
            height = 0;
            gameCamera = false;
            positionX = 0;
            positionY = 0;
        }

        CameraRect(int inScreenX, int inScreenY, int inWidth, int inHeight, bool inGameCamera)
        {
            screenX = inScreenX;
            screenY = inScreenY;
            width = inWidth;
            height = inHeight;
            gameCamera = inGameCamera;
            positionX = 0;
            positionY = 0;
        }

        bool gameCamera;
        int screenX;
        int screenY;
        int width;
        int height;

        float positionX;
        float positionY;
    };

    struct ScreenData
    {
        ScreenData()
        {
            targetWidth = 0;
            targetHeight = 0;

            nativeWidth = 0;
            nativeHeight = 0;

            memory = nullptr;
            sortDepthBuffer = nullptr;

            bitmap_info = {};
        }

        int targetWidth;
        int targetHeight;

        int nativeWidth;
        int nativeHeight;

        void* memory;
        int* sortDepthBuffer;

        BITMAPINFO bitmap_info;
    };

    struct Image
    {
        Image()
        {
            width = 0;
            height = 0;
            channels = 0;
            imageData = nullptr;
        }

        int width;
        int height;
        int channels;
        Color* imageData;
    };

    struct CameraRect;

    struct Text
    {
        Text() 
        { 
            x = 0; 
            y = 0; 
            image = 0; 
            cameraRect = 0; 
            kerningLeading = 0; 
            kerning = 0; 
            stringLength = 0; 
            stringArray = 0; 
            characterSize = 0;
            mainColor = {};
            sortOrder = 0; 
        }
        
        Text(int inX, int inY, int inKerning, int inKerningLeading,
            int inStringLength, char* inStringArray, int inCharacterSize, 
            int inSortOrder, Color inMainColor, Image* inImage, CameraRect* inCamera)
        {
            x = inX;
            y = inY;
            image = inImage;
            cameraRect = inCamera;
            kerning = inKerning;
            kerningLeading = inKerningLeading;
            stringLength = inStringLength;
            stringArray = inStringArray;
            characterSize = inCharacterSize;
            mainColor = inMainColor;
            sortOrder = inSortOrder;
        }

        CameraRect* cameraRect;
        Image* image;
        int x;
        int y;
        int kerningLeading; // Vertical spacing
        int kerning; // Space between characters
        int characterSize;
        Color mainColor;

        int stringLength;
        char* stringArray;

        int sortOrder;
    };

    struct Sprite
    {
        Sprite() 
        { 
            x = 0; 
            y = 0;
            image = 0;
            cameraRect = 0;
            sortOrder = 0; 
        }

        Sprite(float inX, float inY, int inSortOrder, Image* inImage, CameraRect* inCamera)
        {
            x = inX;
            y = inY;
            image = inImage;
            cameraRect = inCamera;
            sortOrder = inSortOrder;
        }

        CameraRect* cameraRect;
        Image* image;
        float x;
        float y;

        int sortOrder;
    };

    struct SpriteInSheet
    {
        SpriteInSheet() 
        {
            x = 0;
            y = 0; 
            tileID = 0;
            tileSize = 0;
            image = 0; 
            cameraRect = 0; 
            sortOrder = 0; 
        }
        
        SpriteInSheet(float inX, float inY, int inTileID, int inTileSize, int inSortOrder, Image* inImage, CameraRect* inCamera)
        {
            x = inX;
            y = inY;
            tileID = inTileID;
            tileSize = inTileSize;
            image = inImage;
            cameraRect = inCamera;
            sortOrder = inSortOrder;
        }

        CameraRect* cameraRect;
        Image* image;
        int tileID;
        int tileSize;
        float x;
        float y;
        int sortOrder;
    };

    struct SpriteAnimationFrames
    {
        SpriteAnimationFrames()
        {
            frameCount = 0;
            frames = nullptr;
            animationTime = 0;
            isLoop = false;
        }

        int frameCount;
        int* frames;
        float animationTime;
        bool isLoop;
    };

    struct SpriteAnimated
    {
        SpriteAnimated()
        {
            x = 0; 
            y = 0; 
            tileSizeX = 0; 
            tileSizeY = 0; 
            image = 0;
            currentTime = 0;
            currentAnimationID = 0;
            animations = 0; 
            animationCount = 0;
            cameraRect = 0;
            sortOrder = 0;
        }

        SpriteAnimated(float inX, float inY, int inTileSizeX, int inTileSizeY, int inSortOrder, Image* inImage, CameraRect* inCamera)
        {
            x = inX;
            y = inY;
            tileSizeX = inTileSizeX;
            tileSizeY = inTileSizeY;
            image = inImage;
            cameraRect = inCamera;
            currentTime = 0;
            currentAnimationID = 0;
            animations = 0;
            animationCount = 0;
            sortOrder = inSortOrder;
        }

        CameraRect* cameraRect;
        Image* image;
        int tileSizeX;
        int tileSizeY;
        float x;
        float y;

        int animationCount = 0;
        SpriteAnimationFrames* animations;

        int currentAnimationID;
        float currentTime;

        int sortOrder;
    };

    Image LoadImageFromFile(const char* fileName);
    void DestroyImageFromFile(Image* image);

    void RenderText(ScreenData* screenData, const Text text, const int screenSize);
    void RenderSpriteAnimated(ScreenData* screenData, const SpriteAnimated spriteAnim, const int screenSize);
    void RenderSprite(ScreenData* screenData, const Sprite sprite, const int screenSize);
    void RenderBox(ScreenData* screenData, const CameraRect* cameraRect, 
        const float xIn, const float yIn, const float w, const float h, const int screenSize);
    void RenderScrollSprite(ScreenData* screenData, 
        const Sprite sprite, const int screenSize, const int scrollOffset);

    bool OutsideRect(const CameraRect* cameraRect, const int screenX, const int screenY);
    bool RenderPixelToScreen(ScreenData* screenData, const int pixelID, const int screenSize, const int sortOrder, const Color tintColor);

    void UpdateSpriteAnimated(SpriteAnimated* spriteAnim, const float dt);

    void LoadAnimation(SpriteAnimated* animSprite, const char* fileName);
}

#endif