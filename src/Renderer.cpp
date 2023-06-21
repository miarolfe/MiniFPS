#include <iostream>
#include <cmath>
#include <unordered_map>

#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Menu.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utilities.h"

namespace MiniFPS {
    Renderer::Renderer(SDL_Renderer* sdlRenderer, const Settings& settings) : sdlRenderer(sdlRenderer) {
        streamingFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                  static_cast<int>(settings.screenWidth),
                                                  static_cast<int>(settings.screenHeight));
        renderFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
                                               static_cast<int>(settings.screenWidth),
                                               static_cast<int>(settings.screenHeight));
    }

    void Renderer::SetTextureMap(const std::unordered_map<short, Texture>& newTextureMap) {
        textureMap = newTextureMap;
    }

    void Renderer::SetPixel(void* pixels, int pitch, Color color, int x, int y) {
        uint32_t* row = (uint32_t*) ((uint8_t*) pixels + y * pitch);
        row[x] = color.argb;
    }

    Texture Renderer::GetTexBuffer(short textureId) {
        Texture texture = textureMap[textureId];

        if (texture.name.empty()) {
            std::cerr << "Invalid texture: no texture mapped to id " << textureId << std::endl;
            texture = textureMap[-1]; // Fallback texture
        }

        return texture;
    }

    void Renderer::CopyTextureToFrameTexture(void* pixels, int pitch, const Texture& texture, int x, int y, int w, int h) {
        const float scaleX = static_cast<float>(texture.size) / w;
        const float scaleY = static_cast<float>(texture.size) / h;

        for (int destinationY = 0; destinationY < h; destinationY++) {
            for (int destinationX = 0; destinationX < w; destinationX++) {
                const int sourceX = static_cast<int>(destinationX * scaleX);
                const int sourceY = static_cast<int>(destinationY * scaleY);

                const Color pixel = texture.buffer[sourceY][sourceX];

                // Check if alpha value is 0 (transparent)
                if ((pixel.argb & TRANSPARENCY_MASK) != 0) {
                    SetPixel(pixels, pitch, pixel, x + destinationX - (w/2), y + destinationY - (h/2));
                }
            }
        }
    }

//    size = tempTextureSurface->w;
//    buffer = new Color* [size];
//    for (int row = 0; row < size; row++)
//        buffer[row] = new Color[size];

    void Renderer::FreeTextures() {
        for (const auto& idTexturePair : textureMap) {
            for (int row = 0; row < idTexturePair.second.size; row++) {
                delete[] idTexturePair.second.buffer[row];
            }

            delete[] idTexturePair.second.buffer;
        }
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

    void Renderer::DrawButton(Button button) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_Texture* buttonTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, button.width, button.height);

        const SDL_Rect sdlRect {
            static_cast<int>(button.GetLeftBound()),
            static_cast<int>(button.GetTopBound()),
            static_cast<int>(button.width),
            static_cast<int>(button.height)
        };

        void* pixels = nullptr;
        int pitch = -1;

        SDL_LockTexture(buttonTexture, nullptr, &pixels, &pitch);

        for (int x = 0; x < button.width; x++) {
            for (int y = 0; y < button.height; y++) {
                SetPixel(pixels, pitch, BUTTON, x, y);
            }
        }

        SDL_UnlockTexture(buttonTexture);

        SDL_RenderCopy(sdlRenderer, buttonTexture, nullptr, &sdlRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);
    }

    void Renderer::DrawTextStr(const std::string& text, const Font& font, float x, float y, int width, int r=255, int g=255, int b=255) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        int requestedWidth;
        int requestedHeight;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        const float ratio = static_cast<float>(requestedWidth) / static_cast<float>(requestedHeight);

        const int height = static_cast<int>(static_cast<float>(width) / ratio);

        const SDL_Rect destRect{static_cast<int>(x), static_cast<int>(y), width, height};

        SDL_RenderCopy(sdlRenderer, RenderTextToTexture(sdlRenderer, font, text, r, g, b), nullptr, &destRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);
    }

    void Renderer::DrawTextStrH(const std::string& text, const Font& font, float x, float y, int height, int r=255, int g=255, int b=255) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        int requestedWidth;
        int requestedHeight;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        const float ratio = static_cast<float>(requestedHeight) / static_cast<float>(requestedWidth);

        const int width = static_cast<int>(static_cast<float>(height) / ratio);

        const SDL_Rect destRect{static_cast<int>(x), static_cast<int>(y), width, height};

        SDL_RenderCopy(sdlRenderer, RenderTextToTexture(sdlRenderer, font, text, r, g, b), nullptr, &destRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);
    }

    void Renderer::DrawMainMenu(const MainMenu& mainMenu) {
        int pitch;
        void* pixels;
        SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

        for (int frameY = 0; frameY < static_cast<int>(mainMenu.player.camera.viewportHeight); frameY++) {
            for (int frameX = 0; frameX < static_cast<int>(mainMenu.player.camera.viewportWidth); frameX++) {
                SetPixel(pixels, pitch, MAIN_MENU_BACKGROUND, frameX, frameY);
            }
        }

        const int titleTextX = static_cast<int>(mainMenu.player.camera.viewportWidth / 4);
        const int titleTextY = static_cast<int>(mainMenu.player.camera.viewportHeight / 16);
        const int titleTextWidth = static_cast<int>(mainMenu.player.camera.viewportWidth / 2);

        const int versionTextX = static_cast<int>(3 * (mainMenu.player.camera.viewportWidth / 8));
        const int versionTextY = static_cast<int>(5 * (mainMenu.player.camera.viewportHeight / 16));
        const int versionTextWidth = static_cast<int>(mainMenu.player.camera.viewportWidth / 4);

        const int startTextX = static_cast<int>(mainMenu.startButton.x - mainMenu.startButton.width/4);
        const int startTextY = static_cast<int>(mainMenu.startButton.y - mainMenu.startButton.height/2);

        SDL_UnlockTexture(streamingFrameTexture);
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // UI draw calls

        DrawButton(mainMenu.startButton);
        DrawTextStr("MiniFPS", mainMenu.font, titleTextX, titleTextY, titleTextWidth);
        DrawTextStr(mainMenu.settings.version, mainMenu.font, versionTextX, versionTextY, versionTextWidth);
        DrawTextStrH("Start", mainMenu.font, startTextX, startTextY, mainMenu.startButton.height);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }

    void Renderer::Draw(Player player, const Font& font, const float frameDelta) {
        int pitch = -1;
        void* pixels = nullptr;
        SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

        DrawCeiling(player.camera, pixels, pitch);
        DrawFloor(player.camera, pixels, pitch);

        // Cast rays
        for (int ray = 0; ray < player.camera.viewportWidth; ray++) {
            const float rayScreenPos = (2 * ray / float(player.camera.viewportWidth) - 1) * player.camera.aspectRatio;
            const float rayAngle = player.camera.angle + atan(rayScreenPos * tan(player.camera.horizontalFieldOfView / 2));
            const float cosRayAngle = cos(rayAngle); // X component
            const float sinRayAngle = sin(rayAngle); // Y component

            // TODO: DDA?
            for (float rayDistance = 0; rayDistance < player.camera.maxRenderDistance; rayDistance += player.camera.rayIncrement) {
                const float cellX = player.camera.x + rayDistance * cosRayAngle;
                const float cellY = player.camera.y + rayDistance * sinRayAngle;

                const short cellID = player.level->Get(static_cast<int>(cellX), static_cast<int>(cellY));

                if (cellID != 0) {
                    const Texture texture = GetTexBuffer(cellID);
                    const float distance = rayDistance * cos(rayAngle - player.camera.angle);
                    const int columnHeight = ((player.camera.viewportHeight) * player.camera.distanceToProjectionPlane) / distance;

                    const float hitX = cellX - floor(cellX + 0.5f); // Fractional part of cellX
                    const float hitY = cellY - floor(cellY + 0.5f); // Fractional part of cellY#

                    int texX;

                    if (std::abs(hitY) > std::abs(hitX)) { // West-East
                        texX = static_cast<int>(hitY * static_cast<float>(texture.size));
                    } else { // North-South
                        texX = static_cast<int>(hitX * static_cast<float>(texture.size));
                    }

                    const float floorCellX = floor(cellX);
                    const float floorCellY = floor(cellY);

                    const float x1 = floorCellX;
                    const float y1 = floorCellY;
                    const float x2 = floorCellX + 1;
                    const float y2 = floorCellY + 1;
                    const float x3 = floorCellX + 1;
                    const float y3 = floorCellY;

                    if (IsPointInRightAngledTriangle(cellX, cellY, x1, y1, x2, y2, x3, y3)) {
                        texX = texture.size - texX - 1;
                    }

                    if (texX < 0) texX += texture.size;
                    if (texX >= texture.size) texX -= texture.size;

                    const int drawStart = ((player.camera.viewportHeight / 2) - (columnHeight / 2));

                    const int drawEnd = drawStart + columnHeight;

                    for (int y = drawStart; y < drawEnd; y++) {
                        if (y < 0 || y >= player.camera.viewportHeight) {
                            continue;
                        }

                        const int texY = ((y - drawStart) * texture.size) / columnHeight;
                        SetPixel(pixels, pitch, texture.buffer[texY][texX], ray, y);
                    }

                    break;
                }
            }
        }

        const int weaponTextureSize = player.camera.viewportWidth / 4;
        CopyTextureToFrameTexture(pixels, pitch, player.weaponTexture, player.camera.viewportWidth/2, player.camera.viewportHeight - (weaponTextureSize/2), weaponTextureSize, weaponTextureSize);

        SDL_UnlockTexture(streamingFrameTexture);

        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // TODO: Scale this with frame
        // UI draw here
        DrawTextStrH("MiniFPS", font, 25, 25, 100);
        DrawTextStrH("peterrolfe.com", font, 25, 125, 50);
        DrawTextStrH(GetFramesPerSecond(frameDelta), font, player.camera.viewportWidth - 100, 25, 50, 255, 0, 0);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }
}