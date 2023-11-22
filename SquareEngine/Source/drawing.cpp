#include "drawing.h"

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"

Image LoadImageFromFile(const char* fileName)
{
    Image newImage;
    unsigned char* img = stbi_load(fileName, &newImage.width, &newImage.height, &newImage.channels, 0);

    if(img == NULL)
    {
        printf("Error in loading the image\n");
        assert(false);
        exit(1);
    }
    else
    {
        newImage.imageData = new Color[newImage.width * newImage.height];

        int currentID = 0;
        for(int i = 0; i < (newImage.width * newImage.height); i++)
        {
            Color color;
            color.r = img[currentID++];
            color.g = img[currentID++];
            color.b = img[currentID++];

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

bool SortOrder(ScreenData* screenData, int screenX, int screenY, int sortOrder)
{
    int screenID = (screenY * screenData->nativeWidth) + screenX;

    if(sortOrder >= screenData->sortDepthBuffer[screenID])
    {
        return true;
    }

    return false;
}

bool OutsideRect(CameraRect* cameraRect, int screenX, int screenY)
{
    if((screenX < cameraRect->screenX || screenX >= cameraRect->screenX + cameraRect->width) ||
        (screenY < cameraRect->screenY || screenY >= cameraRect->screenY + cameraRect->height))
    {
        return true;
    }

    return false;
}

bool RenderImagePixelToScreen(ScreenData* screenData, int pixelID, int screenSize,
    Color* imageData, int spriteArrayID, int sortOrder, Color tintColor)
{
    if(pixelID > 0 && pixelID < screenSize)
    {
        // Alpha cutout color
        if(imageData[spriteArrayID].r == 0 && imageData[spriteArrayID].g == 255 && imageData[spriteArrayID].b == 0)
        {
            return false;
        }

        screenData->sortDepthBuffer[pixelID] = sortOrder;

        // TODO: Improve this with memcpy
        ((char*)screenData->memory)[(pixelID * 4)] =     (char)((float)imageData[spriteArrayID].b * ((float)tintColor.b / 255.0f));
        ((char*)screenData->memory)[(pixelID * 4) + 1] = (char)((float)imageData[spriteArrayID].g * ((float)tintColor.g / 255.0f));
        ((char*)screenData->memory)[(pixelID * 4) + 2] = (char)((float)imageData[spriteArrayID].r * ((float)tintColor.r / 255.0f));
        ((char*)screenData->memory)[(pixelID * 4) + 3] = (char)0;
    }

    return true;
}


bool RenderPixelToScreen(ScreenData* screenData, int pixelID, int screenSize, int sortOrder, Color tintColor)
{
    if(pixelID > 0 && pixelID < screenSize)
    {
        screenData->sortDepthBuffer[pixelID] = sortOrder;

        // TODO: Improve this with memcpy
        ((char*)screenData->memory)[(pixelID * 4)] = (char)((float)tintColor.b);
        ((char*)screenData->memory)[(pixelID * 4) + 1] = (char)((float)tintColor.g);
        ((char*)screenData->memory)[(pixelID * 4) + 2] = (char)((float)tintColor.r);
        ((char*)screenData->memory)[(pixelID * 4) + 3] = (char)0;
    }

    return true;
}

void RenderBox(ScreenData* screenData, CameraRect* cameraRect, float xIn, float yIn, float w, float h, int screenSize)
{
    int cameraOffsetX = cameraRect->screenX;
    int cameraOffsetY = cameraRect->screenY;

    auto spriteY = (int)yIn;

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

void RenderScrollSprite(ScreenData* screenData, Sprite sprite, int screenSize, int scrollOffset)
{
    Image* image = sprite.image;

    int cameraOffsetX = sprite.cameraRect->screenX;
    int cameraOffsetY = sprite.cameraRect->screenY;

    auto spriteY = (int)sprite.y;

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
            else if(texX > image->width-1)
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

            if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, sprite.sortOrder, colorWhite))
            {
                continue;
            }
        }
    }
}

void RenderSprite(ScreenData* screenData, Sprite sprite, int screenSize)
{
    Image* image = sprite.image;

    int cameraOffsetX = sprite.cameraRect->screenX;
    int cameraOffsetY = sprite.cameraRect->screenY;

    auto spriteY = (int)sprite.y;
    
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

            if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, sprite.sortOrder, colorWhite))
            {
                continue;
            }
        }
    }
}

void RenderSpriteFrameSheet(ScreenData* screenData, SpriteInSheet sprite, int screenSize)
{
    Image* image = sprite.image;

    int tileAmountWidth = image->width / sprite.tileSize;
    int xOffset = (sprite.tileID % tileAmountWidth) * sprite.tileSize;
    int yOffset = ((int)floorf((float)(sprite.tileID / tileAmountWidth))) * sprite.tileSize;

    int cameraOffsetX = sprite.cameraRect->screenX;
    int cameraOffsetY = sprite.cameraRect->screenY;

    auto spriteY = (int)sprite.y;

    if(sprite.cameraRect->gameCamera)
    {
        spriteY = (sprite.cameraRect->height - (int)sprite.y) - image->height;
    }

    for(int y = 0; y < sprite.tileSize; y++)
    {
        for(int x = 0; x < sprite.tileSize; x++)
        {
            int screenX = ((int)sprite.x + x) + cameraOffsetX;
            int screenY = (spriteY + y) + cameraOffsetY;

            if(OutsideRect(sprite.cameraRect, screenX, screenY) ||
                !SortOrder(screenData, screenX, screenY, sprite.sortOrder))
            {
                continue;
            }

            int screenID = (screenY * screenData->nativeWidth) + screenX;
            int arrayID = ((y + yOffset) * image->width) + (x + xOffset);

            if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, sprite.sortOrder, colorWhite))
            {
                continue;
            }
        }
    }
}

void UpdateSpriteAnimated(SpriteAnimated* spriteAnim, float dt)
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

float clamp(float min, float max, float value)
{
    if(value < min)
        return min;

    else if(value > max)
        return max;

    return value;
}

void RenderSpriteAnimated(ScreenData* screenData, SpriteAnimated spriteAnim, int screenSize)
{
    Image* image = spriteAnim.image;

    int cameraOffsetX = spriteAnim.cameraRect->screenX;
    int cameraOffsetY = spriteAnim.cameraRect->screenY;

    // Calculate current tile ID
    float animTime01 = clamp(0.0f, 1.0f, (float)spriteAnim.currentTime / (float)spriteAnim.animations[spriteAnim.currentAnimationID].animationTime);
    int frameID = (int)floorf(animTime01 * (float)(spriteAnim.animations[spriteAnim.currentAnimationID].frameCount - 1));
    int tileID = spriteAnim.animations[spriteAnim.currentAnimationID].frames[frameID];

    // Calculate the x & y offset in pixels
    int tileAmountWidth = image->width / spriteAnim.tileSizeX;
    int xOffset = (tileID % tileAmountWidth) * spriteAnim.tileSizeX;

    int tileAmountHeight = image->height / spriteAnim.tileSizeY;
    int yOffset = (tileID / tileAmountWidth) * spriteAnim.tileSizeY;

    auto spriteY = (int)spriteAnim.y;

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

            if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, spriteAnim.sortOrder, colorWhite))
            {
                continue;
            }
        }
    }
}

void RenderText(ScreenData* screenData, Text text, int screenSize)
{
    Image* image = text.image;

    int cameraOffsetX = text.cameraRect->screenX;
    int cameraOffsetY = text.cameraRect->screenY;

    auto spriteY = (int)text.y;

    if(text.cameraRect->gameCamera)
    {
        spriteY = (text.cameraRect->height - (int)text.y) - text.characterSize;
    }

    for(int j = 0; j < text.stringLength; j++)
    {
        char character = text.stringArray[j];
        int characterID = -1;

        // PERF: Do this when setting text up rather than per frame
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

                if(!RenderImagePixelToScreen(screenData, screenID, screenSize, image->imageData, arrayID, text.sortOrder, text.mainColor))
                {
                    continue;
                }
            }
        }
    }
}

void LoadAnimation(SpriteAnimated* animSprite, const char* fileName)
{
    char readLine[512];
    FILE* fp = fopen(fileName, "r");

    if(fp == NULL)
    {
        printf("Can't load file: ");
        printf("%s", fileName);
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

            const char* headerName6 = "End";
            if(strstr(readLine, headerName6))
            {
                currentAnim++;
            }
        }

        fclose(fp);
    }
}