#include <iostream>
#include <cmath>

#include <SDL.h>

#include "Camera.h"
#include "Color.h"
#include "Level.h"
#include "Menu.h"
#include "Utilities.h"
#include "Renderer.h"

const Uint32 CEILING = 0xFFA5A5A5;
const Uint32 FLOOR   = 0xFF0000A5;

Texture::Texture() {

}

Texture::Texture(std::string name, std::string filePath) {
    this->name = name;

    SDL_Surface* tempTextureSurface = IMG_Load(filePath.c_str());
    tempTextureSurface = SDL_ConvertSurfaceFormat(tempTextureSurface, SDL_PIXELFORMAT_ARGB8888, 0);

    size = tempTextureSurface->w;
    buffer = new Uint32*[size];
    for (int row = 0; row < size; row++) {
        buffer[row] = new Uint32[size];
    }

    Uint32* pixels = (Uint32*) tempTextureSurface->pixels;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            buffer[row][col] = pixels[row * size + col];
        }
    }

    SDL_FreeSurface(tempTextureSurface);
}

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

Texture GetTexBuffer(short cellColor, std::unordered_map<short, Texture>& textureMap) {
    Texture texture = textureMap[cellColor];

    if (texture.name == "") {
        std::cerr << "Invalid texture: no texture mapped to id " << cellColor << std::endl;
    }

    return texture;
}

void DrawText(SDL_Renderer* sdlRenderer, SDL_Texture* renderFrameTexture, const std::string &text, Font font, SDL_Rect destRect) {
    SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
    SDL_RenderCopy(sdlRenderer, RenderTextToTexture(sdlRenderer, font, text, 255, 255, 255), NULL, &destRect);
    SDL_SetRenderTarget(sdlRenderer, nullptr);
}

void Draw(SDL_Renderer* renderer, Player player, std::unordered_map<short, Texture>& textures, SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture) {
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

            const short cell = player.level->Get(static_cast<int>(cx), static_cast<int>(cy));

            if (cell != 0) {
                Texture texture = GetTexBuffer(cell, textures);
                float distance = t * cos(rayAngle - player.camera.angle);
                size_t columnHeight = ((player.camera.viewportHeight) * player.camera.distanceToProjectionPlane) / distance;

                float hitX = cx - floor(cx + 0.5f);
                float hitY = cy - floor(cy + 0.5f);
                int texX = static_cast<int>(hitX * static_cast<float>(texture.size));

                if (std::abs(hitY) > std::abs(hitX)) {
                    texX = hitY * texture.size;
                }

                if (texX < 0) texX += texture.size;

                int drawStart = (static_cast<int>(player.camera.viewportHeight) / 2) - (static_cast<int>(columnHeight) / 2);

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

void DrawMainMenu(const Settings& settings, SDL_Renderer* renderer, const Font& font, Camera camera, SDL_Texture* streamingFrameTexture, SDL_Texture* renderFrameTexture) {
    int pitch;
    void *pixels;
    SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

    for (int frameY = 0; frameY < static_cast<int>(camera.viewportHeight); frameY++) {
        for (int frameX = 0; frameX < static_cast<int>(camera.viewportWidth); frameX++) {
            SetPixel(pixels, pitch, ARGB_BLACK, frameX, frameY);
        }
    }

    SDL_Rect titleTextRect = {static_cast<int>(camera.viewportWidth / 4),
                              static_cast<int>(camera.viewportHeight / 16),
                              static_cast<int>(camera.viewportWidth / 2),
                              static_cast<int>(camera.viewportHeight / 4)};

    SDL_Rect versionTextRect = {static_cast<int>(3 * (camera.viewportWidth / 8)),
                                static_cast<int>(5 * (camera.viewportHeight / 16)),
                                static_cast<int>(camera.viewportWidth / 4),
                                static_cast<int>(camera.viewportHeight / 8)};

    SDL_Rect startGameTextRect = {static_cast<int>(camera.viewportWidth / 12),
                                  static_cast<int>(6 * (camera.viewportHeight / 8)),
                                  static_cast<int>(10 * (camera.viewportWidth / 12)),
                                  static_cast<int>(camera.viewportHeight / 8)};

    SDL_UnlockTexture(streamingFrameTexture);
    SDL_SetRenderTarget(renderer, renderFrameTexture);
    SDL_RenderCopy(renderer, streamingFrameTexture, nullptr, nullptr);

    // UI draw calls

    DrawText(renderer, renderFrameTexture, "mini-fps", font, titleTextRect);
    DrawText(renderer, renderFrameTexture, settings.version, font, versionTextRect);
    DrawText(renderer, renderFrameTexture, "Press [SPACE] or [ENTER] to start", font, startGameTextRect);

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, renderFrameTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
