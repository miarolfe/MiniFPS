#include <iostream>
#include <cmath>
#include <cstdint>
#include <algorithm>

#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"
#include "Utilities.h"

void set_pixel(void* pixels, int pitch, const Uint32 color, int x, int y) {
    Uint32* row;
    row = (Uint32*) ((Uint8*) pixels + y * pitch);
    row[x] = color;
}

void draw(SDL_Renderer* renderer, Camera camera, Level &level, Uint32**** texBuffers, size_t numTexBuffers, size_t texSize,
          SDL_Texture* frameTexture) {
    int pitch;
    void* pixels;
    SDL_LockTexture(frameTexture, nullptr, &pixels, &pitch);

    // Draw ceiling
    for (int frameY = 0; frameY < camera.viewportHeight / 2; frameY++) {
        for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
            set_pixel(pixels, pitch, 0xFFFFFFFF, frameX, frameY);
        }
    }

    // Draw floor
    for (int frameY = camera.viewportHeight / 2; frameY < camera.viewportHeight; frameY++) {
        for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
            set_pixel(pixels, pitch, 0xFF0000FF, frameX, frameY);
        }
    }

    for (size_t ray = 0; ray < camera.viewportWidth; ray++) {
        float rayAngle =
                (camera.angle - camera.fieldOfView / 2) + (camera.fieldOfView * ray / float(camera.viewportWidth));

        float t;
        float cosRayAngle = cos(rayAngle);
        float sinRayAngle = sin(rayAngle);

        for (t = 0; t < camera.maxRenderDistance; t += camera.rayIncrement) {
            float cx = camera.x + t * cosRayAngle;
            float cy = camera.y + t * sinRayAngle;

            const Uint32 levelCellColor = level.get(int(cx), int(cy));

            if (levelCellColor != ARGB_WHITE) {
                double distance = t * cos(rayAngle - camera.angle);

                size_t columnHeight = (camera.viewportHeight / distance) * camera.distanceToProjectionPlane;

                float hitX = cx - floor(cx + 0.5);
                float hitY = cy - floor(cy + 0.5);
                int texX = hitX * texSize;

                if (std::abs(hitY) > std::abs(hitX)) {
                    texX = hitY * texSize;
                }

                if (texX < 0) texX += texSize;

                int drawStart = (camera.viewportHeight / 2) - (columnHeight / 2);

                int drawEnd = drawStart + columnHeight;

                Uint32** texBuffer;

                // Map cell color to texture buffer

                switch (levelCellColor) {

                    case ARGB_BLACK:
                        texBuffer = (*texBuffers)[0];
                        break;
                    case ARGB_RED:
                        texBuffer = (*texBuffers)[clamp(1, 0, numTexBuffers-1)];
                        break;
                    case ARGB_BLUE:
                        texBuffer = (*texBuffers)[clamp(2, 0, numTexBuffers-1)];
                        break;
                    case ARGB_GREEN:
                        texBuffer = (*texBuffers)[clamp(3, 0, numTexBuffers-1)];
                        break;
                    default:
                        texBuffer = (*texBuffers)[clamp(4, 0, numTexBuffers-1)];
                        break;
                }

                for (int y = drawStart; y < drawEnd; y++) {
                    if (y < 0 || y >= camera.viewportHeight) {
                        continue;
                    }

                    int texY = ((y - drawStart) * texSize) / columnHeight;
                    set_pixel(pixels, pitch, texBuffer[texY][texX], ray, y);
                }

                break;
            }
        }
    }

    SDL_RenderCopy(renderer, frameTexture, nullptr, nullptr);
    SDL_UnlockTexture(frameTexture);
    SDL_RenderPresent(renderer);
}