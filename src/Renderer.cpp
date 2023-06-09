#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utilities.h"

const Uint32 CEILING = 0xFFA5A5A5;
const Uint32 FLOOR   = 0xFF0000A5;

namespace MiniFPS {
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

    Texture GetTexBuffer(short textureId, std::unordered_map<short, Texture> &textureMap) {
        Texture texture = textureMap[textureId];

        if (texture.name == "") {
            std::cerr << "Invalid texture: no texture mapped to id " << textureId << std::endl;
            texture = textureMap[-1]; // Fallback texture
        }

        return texture;
    }

    void DrawText(SDL_Renderer* sdlRenderer, SDL_Texture* renderFrameTexture, const std::string &text, Font font, int x,
                  int y, int width) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        int requestedWidth;
        int requestedHeight;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        float ratio = static_cast<float>(requestedWidth) / static_cast<float>(requestedHeight);

        int height = static_cast<int>(static_cast<float>(width) / ratio);

        SDL_Rect destRect{x, y, width, height};

        SDL_RenderCopy(sdlRenderer, RenderTextToTexture(sdlRenderer, font, text, 255, 255, 255), NULL, &destRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);
    }

    void Draw(SDL_Renderer* renderer, Player player, std::unordered_map<short, Texture> &textures,
              SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture) {
        int pitch;
        void* pixels;
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

                const short cell = player.level->Get(static_cast<int>(cx), static_cast<int>(cy));

                if (cell != 0) {
                    Texture texture = GetTexBuffer(cell, textures);
                    float distance = t * cos(rayAngle - player.camera.angle);
                    size_t columnHeight =
                            ((player.camera.viewportHeight) * player.camera.distanceToProjectionPlane) / distance;

                    float hitX = cx - floor(cx + 0.5f);
                    float hitY = cy - floor(cy + 0.5f);
                    int texX = static_cast<int>(hitX * static_cast<float>(texture.size));

                    if (std::abs(hitY) > std::abs(hitX)) {
                        texX = hitY * texture.size;
                    }

                    if (texX < 0) texX += texture.size;

                    int drawStart =
                            (static_cast<int>(player.camera.viewportHeight) / 2) - (static_cast<int>(columnHeight) / 2);

                    int drawEnd = drawStart + columnHeight;

                    for (int y = drawStart; y < drawEnd; y++) {
                        if (y < 0 || y >= player.camera.viewportHeight) {
                            continue;
                        }

                        int texY = ((y - drawStart) * texture.size) / columnHeight;
                        SetPixel(pixels, pitch, texture.buffer[texY][texX], ray, y);
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

    void DrawMainMenu(const Settings &settings, SDL_Renderer* renderer, const Font &font, Camera camera,
                      SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture) {
        int pitch;
        void* pixels;
        SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

        for (int frameY = 0; frameY < static_cast<int>(camera.viewportHeight); frameY++) {
            for (int frameX = 0; frameX < static_cast<int>(camera.viewportWidth); frameX++) {
                SetPixel(pixels, pitch, 0xFF000000, frameX, frameY);
            }
        }

        int titleTextX = static_cast<int>(camera.viewportWidth / 4);
        int titleTextY = static_cast<int>(camera.viewportHeight / 16);
        int titleTextWidth = static_cast<int>(camera.viewportWidth / 2);

        int versionTextX = static_cast<int>(3 * (camera.viewportWidth / 8));
        int versionTextY = static_cast<int>(5 * (camera.viewportHeight / 16));
        int versionTextWidth = static_cast<int>(camera.viewportWidth / 4);

        int startTextX = static_cast<int>(camera.viewportWidth / 12);
        int startTextY = static_cast<int>(6 * (camera.viewportHeight / 8));
        int startTextWidth = static_cast<int>(10 * (camera.viewportWidth / 12));

        SDL_UnlockTexture(streamingFrameTexture);
        SDL_SetRenderTarget(renderer, renderFrameTexture);
        SDL_RenderCopy(renderer, streamingFrameTexture, nullptr, nullptr);

        // UI draw calls
        DrawText(renderer, renderFrameTexture, "MiniFPS", font, titleTextX, titleTextY, titleTextWidth);
        DrawText(renderer, renderFrameTexture, settings.version, font, versionTextX, versionTextY, versionTextWidth);
        DrawText(renderer, renderFrameTexture, "Press [SPACE] or [ENTER] to start", font, startTextX, startTextY,
                 startTextWidth);

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
}