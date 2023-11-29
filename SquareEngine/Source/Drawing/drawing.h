#pragma once
#ifndef DRAWING
#define DRAWING

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
            screenX = 0; screenY = 0;
            width = 0; height = 0;
        }

        CameraRect(int inScreenX, int inScreenY, int inWidth, int inHeight, bool inGameCamera)
        {
            screenX = inScreenX;
            screenY = inScreenY;
            width = inWidth;
            height = inHeight;
            gameCamera = inGameCamera;
        }

        bool gameCamera;
        int screenX = 0;
        int screenY = 0;
        int width = 0;
        int height = 0;

        float positionX = 0;
        float positionY = 0;
    };

    struct ScreenData
    {
        int targetWidth = 0;
        int targetHeight = 0;

        int nativeWidth = 0;
        int nativeHeight = 0;

        void* memory;

        BITMAPINFO bitmap_info;

        int* sortDepthBuffer;
    };

    struct Image
    {
        int width;
        int height;
        int channels;
        Color* imageData;
    };

    struct CameraRect;

    struct Text
    {
        Text() { x = 0; y = 0; image = 0; cameraRect = 0; kerningLeading = 0; kerning = 0; 
                 stringLength = 0; stringArray = 0; characterSize = 0; mainColor = {}; sortOrder = 0; }
        
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
        int x, y;
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
        Sprite() { x = 0; y = 0; image = 0; cameraRect = 0; sortOrder = 0; }
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
        float x, y;

        int sortOrder;
    };

    struct SpriteInSheet
    {
        SpriteInSheet() { x = 0; y = 0; tileID = 0; tileSize = 0; image = 0; cameraRect = 0; sortOrder = 0; }
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
        float x, y;

        int sortOrder;
    };

    struct SpriteAnimationFrames
    {
        int frameCount;
        int* frames;
        float animationTime;
        bool isLoop;
    };

    struct SpriteAnimated
    {
        SpriteAnimated()
        {
            x = 0; y = 0; tileSizeX = 0; tileSizeY = 0; image = 0;
            currentTime = 0; currentAnimationID = 0;
            animations = 0; animationCount = 0; cameraRect = 0;
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
        int tileSizeX, tileSizeY;
        float x, y;

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