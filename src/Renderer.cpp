#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"
#include "Utilities.h"

const Uint32 CEILING = 0xFFA5A5A5;
const Uint32 FLOOR   = 0xFF0000A5;

void SetPixel(void* pixels, int pitch, Uint32 color, int x, int y) {
    Uint32* row;
    row = (Uint32*) ((Uint8*) pixels + y * pitch);
    row[x] = color;
}

void DrawCeiling(Camera camera, int pitch, void* pixels) {
    for (int frameY = 0; frameY < camera.viewportHeight / 2; frameY++) {
        for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
            SetPixel(pixels, pitch, CEILING, frameX, frameY);
        }
    }
}

void DrawFloor(Camera camera, int pitch, void* pixels) {
    for (int frameY = static_cast<int>(camera.viewportHeight / 2); frameY < camera.viewportHeight; frameY++) {
        for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
            SetPixel(pixels, pitch, FLOOR, frameX, frameY);
        }
    }
}

Uint32** GetTexBuffer(Uint32**** texBuffers, size_t numTexBuffers, Uint32 cellColor) {
    Uint32** texBuffer;

    switch (cellColor) {
        case ARGB_RED:
            texBuffer = (*texBuffers)[Clamp(1, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        case ARGB_YELLOW:
            texBuffer = (*texBuffers)[Clamp(2, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        case ARGB_GREEN:
            texBuffer = (*texBuffers)[Clamp(3, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        case ARGB_CYAN:
            texBuffer = (*texBuffers)[Clamp(4, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        case ARGB_BLUE:
            texBuffer = (*texBuffers)[Clamp(5, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        case ARGB_INDIGO:
            texBuffer = (*texBuffers)[Clamp(6, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        case ARGB_BLACK:
            texBuffer = (*texBuffers)[Clamp(7, 0, static_cast<int>(numTexBuffers - 1))];
            break;
        default:
            texBuffer = (*texBuffers)[0];
            break;
    }

    return texBuffer;
}

void Draw(SDL_Renderer* renderer, Camera camera, Level &level, Uint32**** texBuffers, size_t numTexBuffers, size_t texSize,
          SDL_Texture* frameTexture) {
    int pitch;
    void *pixels;
    SDL_LockTexture(frameTexture, nullptr, &pixels, &pitch);

    DrawCeiling(camera, pitch, pixels);
    DrawFloor(camera, pitch, pixels);

    // Cast rays
    for (size_t ray = 0; ray < camera.viewportWidth; ray++) {
        float rayScreenPos = (2 * ray / float(camera.viewportWidth) - 1) * camera.aspectRatio;
        float rayAngle = camera.angle + atan(rayScreenPos * tan(camera.horizontalFieldOfView / 2));

        float t;
        float cosRayAngle = cos(rayAngle);
        float sinRayAngle = sin(rayAngle);

        for (t = 0; t < camera.maxRenderDistance; t += camera.rayIncrement) {
            float cx = camera.x + t * cosRayAngle;
            float cy = camera.y + t * sinRayAngle;

            const Uint32 levelCellColor = level.Get(static_cast<int>(cx), static_cast<int>(cy));

            if (levelCellColor != ARGB_WHITE) {
                float distance = t * cos(rayAngle - camera.angle);
                size_t columnHeight = ((camera.viewportHeight) * camera.distanceToProjectionPlane) / distance;

                float hitX = cx - floor(cx + 0.5f);
                float hitY = cy - floor(cy + 0.5f);
                int texX = static_cast<int>(hitX * static_cast<float>(texSize));

                if (std::abs(hitY) > std::abs(hitX)) {
                    texX = hitY * texSize;
                }

                if (texX < 0) texX += texSize;

                int drawStart = (static_cast<int>(camera.viewportHeight) / 2) - (static_cast<int>(columnHeight) / 2);

                int drawEnd = drawStart + columnHeight;

                Uint32 **texBuffer = GetTexBuffer(texBuffers, numTexBuffers, levelCellColor);

                for (int y = drawStart; y < drawEnd; y++) {
                    if (y < 0 || y >= camera.viewportHeight) {
                        continue;
                    }

                    int texY = ((y - drawStart) * texSize) / columnHeight;
                    SetPixel(pixels, pitch, texBuffer[texY][texX], ray, y);
                }

                break;
            }
        }
    }

    SDL_RenderCopy(renderer, frameTexture, nullptr, nullptr);
    SDL_UnlockTexture(frameTexture);
    SDL_RenderPresent(renderer);
}
