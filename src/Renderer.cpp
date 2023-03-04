#include <iostream>
#include <cmath>
#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"

void set_pixel(void* pixels, int pitch, const Uint32 color, int x, int y) {
    Uint32* row;
    row = (Uint32*)((Uint8*)pixels + y * pitch);
    row[x] = color;
}

void draw(SDL_Renderer *renderer, Camera camera, Level &level, Uint32 **texBuffer, SDL_Texture* frameTexture) {
    int pitch;
    void* pixels;
    SDL_LockTexture(frameTexture, nullptr, &pixels, &pitch);

    for (int frameY = 0; frameY < camera.viewportHeight; frameY++) {
        for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
            set_pixel(pixels, pitch, 0xFFFFFFFF, frameX, frameY);
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

            if (level.get(int(cx), int(cy)) != RGBA_WHITE) {
                double distance = t * cos(rayAngle - camera.angle);

                size_t columnHeight = (camera.viewportHeight / distance) * camera.distanceToProjectionPlane;

                float hitX = cx - floor(cx + 0.5);
                float hitY = cy - floor(cy + 0.5);
                int texX = hitX * 32;

                if (std::abs(hitY) > std::abs(hitX)) {
                    texX = hitY * 32;
                }

                if (texX < 0) texX += 32;

                int drawStart = (camera.viewportHeight / 2) - (columnHeight / 2);
                if (drawStart < 0) drawStart = 0;
                int drawEnd = drawStart + columnHeight;
                if (drawEnd > camera.viewportHeight) drawEnd = camera.viewportHeight - 1;

                for (int y = drawStart; y < drawEnd; y++) {
                    int texY = ((y - drawStart) * 32) / columnHeight;
                    set_pixel(pixels, pitch,texBuffer[texY][texX], ray, y);
                }

                break;
            }
        }
    }

    SDL_RenderCopy(renderer, frameTexture, nullptr, nullptr);
    SDL_UnlockTexture(frameTexture);
    SDL_RenderPresent(renderer);
}