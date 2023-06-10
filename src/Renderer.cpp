#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Level.h"
#include "Menu.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utilities.h"

const uint32_t CEILING = 0xFFA5A5A5;
const uint32_t FLOOR   = 0xFF0000A5;

namespace MiniFPS {
    Renderer::Renderer(SDL_Renderer* sdlRenderer, Settings settings) : sdlRenderer(sdlRenderer) {
        streamingFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                               static_cast<int>(settings.screenWidth), static_cast<int>(settings.screenHeight));
        renderFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
                                                            static_cast<int>(settings.screenWidth), static_cast<int>(settings.screenHeight));
    }

    void Renderer::SetPixel(void* pixels, int pitch, uint32_t color, int x, int y) {
        uint32_t* row;
        row = (uint32_t*) ((Uint8*) pixels + y * pitch);
        row[x] = color;
    }

    Texture Renderer::GetTexBuffer(short textureId) {
        Texture texture = textureMap[textureId];

        if (texture.name.empty()) {
            std::cerr << "Invalid texture: no texture mapped to id " << textureId << std::endl;
            texture = textureMap[-1]; // Fallback texture
        }

        return texture;
    }

    void Renderer::DrawCeiling(Camera camera, void* pixels, int pitch) {
        for (int frameY = 0; frameY < camera.viewportHeight / 2; frameY++) {
            for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
                SetPixel(pixels, pitch, CEILING, frameX, frameY);
            }
        }
    }

    void Renderer::DrawFloor(Camera camera, void* pixels, int pitch) {
        for (int frameY = static_cast<int>(camera.viewportHeight / 2); frameY < camera.viewportHeight; frameY++) {
            for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
                SetPixel(pixels, pitch, FLOOR, frameX, frameY);
            }
        }
    }

    void Renderer::DrawText(const std::string& text, Font font, float x, float y, int width) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        int requestedWidth;
        int requestedHeight;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        float ratio = static_cast<float>(requestedWidth) / static_cast<float>(requestedHeight);

        int height = static_cast<int>(static_cast<float>(width) / ratio);

        SDL_Rect destRect{static_cast<int>(x), static_cast<int>(y), width, height};

        SDL_RenderCopy(sdlRenderer, RenderTextToTexture(sdlRenderer, font, text, 255, 255, 255), nullptr, &destRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);
    }

    void Renderer::DrawMainMenu(const Settings& settings, const Font &font, Camera camera) {
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
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // UI draw calls
        Renderer::DrawText("MiniFPS", font, titleTextX, titleTextY, titleTextWidth);
        Renderer::DrawText(settings.version, font, versionTextX, versionTextY, versionTextWidth);
        Renderer::DrawText("Press [SPACE] or [ENTER] to start", font, startTextX, startTextY,startTextWidth);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }

    void Renderer::Draw(Player player) {
        int pitch;
        void* pixels;
        SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

        DrawCeiling(player.camera, pixels, pitch);
        DrawFloor(player.camera, pixels, pitch);

        // Cast rays
        for (int ray = 0; ray < player.camera.viewportWidth; ray++) {
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
                    Texture texture = GetTexBuffer(cell);
                    float distance = t * cos(rayAngle - player.camera.angle);
                    int columnHeight =
                            ((player.camera.viewportHeight) * player.camera.distanceToProjectionPlane) / distance;

                    float hitX = cx - floor(cx + 0.5f);
                    float hitY = cy - floor(cy + 0.5f);
                    int texX = static_cast<int>(hitX * static_cast<float>(texture.size));

                    if (std::abs(hitY) > std::abs(hitX)) {
                        texX = hitY * static_cast<float>(texture.size);
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

        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // UI draw here

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }
}