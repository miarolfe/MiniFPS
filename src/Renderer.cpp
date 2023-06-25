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
#include "Vector.h"

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

    void Renderer::SetPixel(void* pixels, int pitch, Color color, IntPoint point) {
        uint32_t* row = (uint32_t*) ((uint8_t*) pixels + point.y * pitch);
        row[point.x] = color.argb;
    }

    bool Renderer::ShouldShadePixel(const FloatPoint point) {
        const float hitX = point.x - floor(point.x + 0.5f); // Fractional part of cellX
        const float hitY = point.y - floor(point.y + 0.5f); // Fractional part of cellY

        bool shouldShadePixel = false;

        if (std::abs(hitY) > std::abs(hitX)) { // West-East
            shouldShadePixel = true;
        }

        return shouldShadePixel;
    }

    Texture Renderer::GetTexBuffer(short textureId) {
        Texture texture = textureMap[textureId];

        if (texture.name.empty()) {
            std::cerr << "Invalid texture: no texture mapped to id " << textureId << std::endl;
            texture = textureMap[-1]; // Fallback texture
        }

        return texture;
    }

    int Renderer::GetTexX(FloatPoint point, int textureSize) {
        const float hitX = point.x - floor(point.x + 0.5f); // Fractional part of cellX
        const float hitY = point.y - floor(point.y + 0.5f); // Fractional part of cellY

        int texX = -1;

        if (std::abs(hitY) > std::abs(hitX)) { // West-East
            texX = static_cast<int>(hitY * static_cast<float>(textureSize));
        } else { // North-South
            texX = static_cast<int>(hitX * static_cast<float>(textureSize));
        }

        const float floorCellX = floor(point.x);
        const float floorCellY = floor(point.y);

        const float x1 = floorCellX;
        const float y1 = floorCellY;
        const float x2 = floorCellX + 1;
        const float y2 = floorCellY + 1;
        const float x3 = floorCellX + 1;
        const float y3 = floorCellY;

        if (IsPointInRightAngledTriangle(point, {x1, y1}, {x2, y2}, {x3, y3})) {
            texX = textureSize - texX - 1;
        }

        if (texX < 0) texX += textureSize;
        if (texX >= textureSize) texX -= textureSize;

        return texX;
    }

    void Renderer::CopyTextureToFrameTexture(void* pixels, int pitch, const Texture& texture, IntPoint point, int w, int h) {
        const float scaleX = static_cast<float>(texture.size) / w;
        const float scaleY = static_cast<float>(texture.size) / h;

        for (int destinationY = 0; destinationY < h; destinationY++) {
            for (int destinationX = 0; destinationX < w; destinationX++) {
                const int sourceX = static_cast<int>(destinationX * scaleX);
                const int sourceY = static_cast<int>(destinationY * scaleY);

                const Color pixel = texture.buffer[sourceY][sourceX];

                // Check if alpha value is 0 (transparent)
                if ((pixel.argb & TRANSPARENCY_MASK) != 0) {
                    SetPixel(pixels, pitch, pixel, {point.x + destinationX - (w/2), point.y + destinationY - (h/2)});
                }
            }
        }
    }

    void Renderer::FreeTextures() {
        for (const auto& idTexturePair : textureMap) {
            for (int row = 0; row < idTexturePair.second.size; row++) {
                delete[] idTexturePair.second.buffer[row];
            }

            delete[] idTexturePair.second.buffer;
        }
    }

    void Renderer::DrawTexturedColumn(const Texture &texture, Camera camera, void* pixels, int pitch, float distance,
                                      FloatPoint cell, int rayX) {

        const int columnHeight = ((camera.viewportHeight) * camera.distanceToProjectionPlane) / distance;

        const int texX = GetTexX(cell, texture.size);

        const bool shadePixel = ShouldShadePixel(cell);

        const int drawStart = ((camera.viewportHeight / 2) - (columnHeight / 2));
        const int drawEnd = drawStart + columnHeight;

        for (int rayY = drawStart; rayY < drawEnd; rayY++) {
            if (rayY < 0 || rayY >= camera.viewportHeight) {
                continue;
            }

            const int texY = ((rayY - drawStart) * texture.size) / columnHeight;

            if (shadePixel) {
                const Color shadedTexPixel = Color::ShadePixel(texture.buffer[texY][texX]);
                SetPixel(pixels, pitch, shadedTexPixel, {rayX, rayY});
            } else {
                SetPixel(pixels, pitch, texture.buffer[texY][texX], {rayX, rayY});
            }
        }
    }

    void Renderer::DrawCeiling(Camera camera, void* pixels, int pitch) {
        for (int frameY = 0; frameY < camera.viewportHeight / 2; frameY++) {
            for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
                SetPixel(pixels, pitch, CEILING, {frameX, frameY});
            }
        }
    }

    void Renderer::DrawFloor(Camera camera, void* pixels, int pitch) {
        for (int frameY = static_cast<int>(camera.viewportHeight / 2); frameY < camera.viewportHeight; frameY++) {
            for (int frameX = 0; frameX < camera.viewportWidth; frameX++) {
                SetPixel(pixels, pitch, FLOOR, {frameX, frameY});
            }
        }
    }

    void Renderer::DrawButton(Button button) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        // TODO: Make this more efficient
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
                SetPixel(pixels, pitch, BUTTON, {x, y});
            }
        }

        SDL_UnlockTexture(buttonTexture);

        SDL_RenderCopy(sdlRenderer, buttonTexture, nullptr, &sdlRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);

        SDL_DestroyTexture(buttonTexture);
    }

    void Renderer::DrawTextStr(const std::string& text, const Font& font, FloatPoint point, int width, int r=255, int g=255, int b=255) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        int requestedWidth;
        int requestedHeight;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        const float ratio = static_cast<float>(requestedWidth) / static_cast<float>(requestedHeight);

        const int height = static_cast<int>(static_cast<float>(width) / ratio);

        const SDL_Rect destRect{static_cast<int>(point.x), static_cast<int>(point.y), width, height};

        SDL_Texture* texture = RenderTextToTexture(sdlRenderer, font, text, r, g, b);

        SDL_RenderCopy(sdlRenderer, texture, nullptr, &destRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);

        SDL_DestroyTexture(texture);
    }

    void Renderer::DrawTextStrH(const std::string& text, const Font& font, FloatPoint point, int height, int r=255, int g=255, int b=255) {
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);

        int requestedWidth;
        int requestedHeight;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        const float ratio = static_cast<float>(requestedHeight) / static_cast<float>(requestedWidth);

        const int width = static_cast<int>(static_cast<float>(height) / ratio);

        const SDL_Rect destRect{static_cast<int>(point.x), static_cast<int>(point.y), width, height};

        SDL_Texture* texture = RenderTextToTexture(sdlRenderer, font, text, r, g, b);

        SDL_RenderCopy(sdlRenderer, texture, nullptr, &destRect);
        SDL_SetRenderTarget(sdlRenderer, nullptr);

        SDL_DestroyTexture(texture);
    }

    void Renderer::DrawMainMenu(const MainMenu& mainMenu) {
        int pitch;
        void* pixels;
        SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

        for (int frameY = 0; frameY < static_cast<int>(mainMenu.player.camera.viewportHeight); frameY++) {
            for (int frameX = 0; frameX < static_cast<int>(mainMenu.player.camera.viewportWidth); frameX++) {
                SetPixel(pixels, pitch, MAIN_MENU_BACKGROUND, {frameX, frameY});
            }
        }

        const int titleTextX = static_cast<int>(mainMenu.player.camera.viewportWidth / 4);
        const int titleTextY = static_cast<int>(mainMenu.player.camera.viewportHeight / 16);
        const int titleTextWidth = static_cast<int>(mainMenu.player.camera.viewportWidth / 2);

        const int versionTextX = static_cast<int>(3 * (mainMenu.player.camera.viewportWidth / 8));
        const int versionTextY = static_cast<int>(5 * (mainMenu.player.camera.viewportHeight / 16));
        const int versionTextWidth = static_cast<int>(mainMenu.player.camera.viewportWidth / 4);

        const int startTextX = static_cast<int>(mainMenu.startButton.pos.x - mainMenu.startButton.width/4);
        const int startTextY = static_cast<int>(mainMenu.startButton.pos.x - mainMenu.startButton.height/2);

        SDL_UnlockTexture(streamingFrameTexture);
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // UI draw calls

        DrawButton(mainMenu.startButton);
        DrawTextStr("MiniFPS", mainMenu.font, {static_cast<float>(titleTextX), static_cast<float>(titleTextY)}, titleTextWidth);
        DrawTextStr(mainMenu.settings.version, mainMenu.font, {static_cast<float>(versionTextX), static_cast<float>(versionTextY)}, versionTextWidth);
        DrawTextStrH("Start", mainMenu.font, {static_cast<float>(startTextX), static_cast<float>(startTextY)}, mainMenu.startButton.height);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }

    void Renderer::Draw(const Player& player, const Font& font, const float frameDelta) {
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

            FloatVector2 rayStart(player.camera.pos);
            FloatVector2 rayMax = {
                    player.camera.pos.x + player.camera.maxRenderDistance * cosRayAngle,
                    player.camera.pos.y + player.camera.maxRenderDistance * sinRayAngle
            };

            FloatVector2 rayDirection = rayMax - rayStart;
            rayDirection.Normalize();

            FloatVector2 rayUnitStepSize = {abs(1.0f / rayDirection.x), abs(1.0f / rayDirection.y)};
            IntVector2 mapCheck(rayStart);
            FloatVector2 rayLength1D;
            FloatVector2 step;

            if (rayDirection.x < 0) {
                step.x = -1;
                rayLength1D.x = ((rayStart.x - mapCheck.x) * rayUnitStepSize.x);
            } else {
                step.x = 1;
                rayLength1D.y = ((mapCheck.x ))
            }

            if (rayDirection.y < 0) {
                step.y = -1;
                rayLength1D.y = ((rayStart.y - mapCheck.y) * rayUnitStepSize.y);
            } else {
                step.y = 1;
            }


            for (float rayDistance = 0; rayDistance < player.camera.maxRenderDistance; rayDistance += player.camera.rayIncrement) {
                const float cellX = player.camera.pos.x + rayDistance * cosRayAngle;
                const float cellY = player.camera.pos.y + rayDistance * sinRayAngle;

                if (player.level->IsPositionValid({cellX, cellY})) {
                    const short cellID = player.level->Get({static_cast<int>(cellX), static_cast<int>(cellY)});

                    if (cellID != 0) {
                        const Texture texture = GetTexBuffer(cellID);
                        const float distance = rayDistance * cos(rayAngle - player.camera.angle);

                        DrawTexturedColumn(texture, player.camera, pixels, pitch, distance, {cellX, cellY}, ray);

                        break;
                    }
                } else {
                    break;
                }
            }
        }

        const int weaponTextureSize = player.camera.viewportWidth / 4;
        CopyTextureToFrameTexture(pixels, pitch, player.weaponTexture, {player.camera.viewportWidth/2, player.camera.viewportHeight - (weaponTextureSize/2)}, weaponTextureSize, weaponTextureSize);

        SDL_UnlockTexture(streamingFrameTexture);

        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // TODO: Scale this with frame
        // UI draw here
        DrawTextStrH("MiniFPS", font, {25, 25}, 100);
        DrawTextStrH("peterrolfe.com", font, {25, 125}, 50);
        DrawTextStrH(GetFramesPerSecond(frameDelta), font, {static_cast<float>(player.camera.viewportWidth - 100), 25}, 50, 255, 0, 0);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }
}