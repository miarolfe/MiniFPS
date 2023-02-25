#include <cmath>
#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"

void draw(SDL_Renderer* renderer, Camera camera, Level& level) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (size_t ray = 0; ray < camera.viewportWidth; ray++) {
        float rayAngle = (camera.angle - camera.fieldOfView / 2) + (camera.fieldOfView * ray / float(camera.viewportWidth));

        float t;
        for (t = 0; t < camera.maxRenderDistance; t += camera.rayIncrement) {
            float cx = camera.x + t * cos(rayAngle);
            float cy = camera.y + t * sin(rayAngle);

            if (level.get(int(cx), int(cy)) != RGBA_WHITE) {
                Uint8 r, g, b, a;
                SDL_GetRGBA(level.get(int(cx), int(cy)), &level.pixelFormat, &r, &g, &b, &a);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);

                float distance = t * cos(rayAngle - camera.angle);

                // size_t columnHeight = float(camera.viewportHeight)/t * cos(rayAngle-camera.angle);
                size_t columnHeight = (camera.viewportHeight / distance) * camera.distanceToProjectionPlane;
                SDL_Rect column;
                column.x = ray;
                column.y = camera.viewportHeight/2 - columnHeight/2;
                column.w = 1;
                column.h = columnHeight;

                SDL_RenderFillRect(renderer, &column);
                break;
            }
        }

    }

    SDL_RenderPresent(renderer);
}