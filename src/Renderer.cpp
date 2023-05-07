#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"
#include "Menu.h"
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

void DrawText(SDL_Renderer* sdlRenderer, SDL_Texture* renderFrameTexture, const std::string &text, Font font, SDL_Rect destRect) {
    SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
    SDL_RenderCopy(sdlRenderer, RenderTextToTexture(sdlRenderer, font, text, 255, 255, 255), NULL, &destRect);
    SDL_SetRenderTarget(sdlRenderer, nullptr);
}

void Draw(SDL_Renderer* renderer, Player player, Uint32**** texBuffers, size_t numTexBuffers, size_t texSize,
          SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture) {
    int pitch;
    void *pixels;
    SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

    DrawCeiling(player.camera, pitch, pixels);
    DrawFloor(player.camera, pitch, pixels);

    // Cast rays
    for (size_t ray = 0; ray < player.camera.viewportWidth; ray++) {
        float rayScreenPos = (2 * ray / float(player.camera.viewportWidth) - 1) * player.camera.aspectRatio;
        float rayAngle = player.camera.angle + atan(rayScreenPos * tan(player.camera.horizontalFieldOfView / 2));

        float t;
        float cosRayAngle = cos(rayAngle);
        float sinRayAngle = sin(rayAngle);

        for (t = 0; t < player.camera.maxRenderDistance; t += player.camera.rayIncrement) {
            float cx = player.camera.x + t * cosRayAngle;
            float cy = player.camera.y + t * sinRayAngle;

            const Uint32 levelCellColor = player.level->Get(static_cast<int>(cx), static_cast<int>(cy));

            if (levelCellColor != ARGB_WHITE) {
                float distance = t * cos(rayAngle - player.camera.angle);
                size_t columnHeight = ((player.camera.viewportHeight) * player.camera.distanceToProjectionPlane) / distance;

                float hitX = cx - floor(cx + 0.5f);
                float hitY = cy - floor(cy + 0.5f);
                int texX = static_cast<int>(hitX * static_cast<float>(texSize));

                if (std::abs(hitY) > std::abs(hitX)) {
                    texX = hitY * texSize;
                }

                if (texX < 0) texX += texSize;

                int drawStart = (static_cast<int>(player.camera.viewportHeight) / 2) - (static_cast<int>(columnHeight) / 2);

                int drawEnd = drawStart + columnHeight;

                Uint32 **texBuffer = GetTexBuffer(texBuffers, numTexBuffers, levelCellColor);

                for (int y = drawStart; y < drawEnd; y++) {
                    if (y < 0 || y >= player.camera.viewportHeight) {
                        continue;
                    }

                    int texY = ((y - drawStart) * texSize) / columnHeight;
                    SetPixel(pixels, pitch, texBuffer[texY][texX], ray, y);
                }

                break;
            }
        }
    }

    SDL_UnlockTexture(streamingFrameTexture);

    SDL_SetRenderTarget(renderer, renderFrameTexture);
    SDL_RenderCopy(renderer, streamingFrameTexture, nullptr, nullptr);

    // UI draw here

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, renderFrameTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void DrawMainMenu(SDL_Renderer* renderer, Font font, Camera camera, SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture) {
    int pitch;
    void *pixels;
    SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

    for (int frameY = 0; frameY < static_cast<int>(camera.viewportHeight); frameY++) {
        for (int frameX = 0; frameX < static_cast<int>(camera.viewportWidth); frameX++) {
            SetPixel(pixels, pitch, ARGB_BLACK, frameX, frameY);
        }
    }

    SDL_Rect titleTextRect = {static_cast<int>(camera.viewportWidth / 4),
                              0,
                              static_cast<int>(camera.viewportWidth / 2),
                              static_cast<int>(camera.viewportHeight / 4)};

    SDL_Rect startGameTextRect = {static_cast<int>(camera.viewportWidth / 12),
                                  static_cast<int>(camera.viewportHeight / 2),
                                  static_cast<int>(10 * (camera.viewportWidth / 12)),
                                  static_cast<int>(camera.viewportHeight / 8)};

    SDL_UnlockTexture(streamingFrameTexture);
    SDL_SetRenderTarget(renderer, renderFrameTexture);
    SDL_RenderCopy(renderer, streamingFrameTexture, nullptr, nullptr);

    // UI draw calls

    DrawText(renderer, renderFrameTexture, "mini-fps", font, titleTextRect);
    DrawText(renderer, renderFrameTexture, "Press [SPACE] or [ENTER] to start", font, startGameTextRect);

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, renderFrameTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}


