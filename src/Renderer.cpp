#include <iostream>
#include <cmath>
#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"

void draw(SDL_Renderer* renderer, Camera camera, Level& level, Uint32** texBuffer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (size_t ray = 0; ray < camera.viewportWidth; ray++) {
        float rayAngle = (camera.angle - camera.fieldOfView / 2) + (camera.fieldOfView * ray / float(camera.viewportWidth));

        float t;
        float cosRayAngle = cos(rayAngle);
        float sinRayAngle = sin(rayAngle);

        for (t = 0; t < camera.maxRenderDistance; t += camera.rayIncrement) {
            float cx = camera.x + t * cosRayAngle;
            float cy = camera.y + t * sinRayAngle;

            if (level.get(int(cx), int(cy)) != RGBA_WHITE) {
                Uint8 r, g, b, a;
                SDL_GetRGBA(level.get(int(cx), int(cy)), &level.pixelFormat, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);

                float distance = t * cos(rayAngle - camera.angle);

                size_t columnHeight = (camera.viewportHeight / distance) * camera.distanceToProjectionPlane;
                SDL_Rect column;
                column.x = ray;
                column.y = camera.viewportHeight/2 - columnHeight/2;
                column.w = 1;
                column.h = columnHeight;

                int texX = int(cx * double(32)); // replace 32 with TEX_WIDTH;
                double step = 1.0 * 32 / columnHeight;
                double texPos = (column.y) * step;

                for (int y = column.y; y < column.y + columnHeight; y++) {
                    int texY = (int) texPos & (32 - 1); // cast and mask in case of overflow
                    texPos += step;

                    Uint8 r, g, b, a;
                    // std::cout << texX << " " << texY << std::endl;
                    SDL_GetRGBA(texBuffer[texY % 32][texX % 32], &level.pixelFormat, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                    SDL_RenderDrawPoint(renderer, column.x, y);
                }

                // SDL_RenderFillRect(renderer, &column);
                break;
            }
        }

    }

    SDL_RenderPresent(renderer);
}