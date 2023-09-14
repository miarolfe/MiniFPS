#include "Renderer.h"
#include "Utilities.h"

namespace MiniFPS {
    Renderer::Renderer() = default;

    Renderer::Renderer(SDL_Renderer* sdlRenderer, const Settings& settings) : sdlRenderer(sdlRenderer) {
        streamingFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                  static_cast<int>(settings.screenWidth),
                                                  static_cast<int>(settings.screenHeight));
        renderFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
                                               static_cast<int>(settings.screenWidth),
                                               static_cast<int>(settings.screenHeight));

        zBuffer = static_cast<float*>(malloc(sizeof(float) * settings.screenWidth));
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

    bool Renderer::WallIsWestOrEastFacing(FloatPoint point) {
        return (std::abs(point.x - floor(point.x + 0.5f)) > std::abs(point.y - floor(point.y + 0.5f)));
    }

    int Renderer::GetTexX(const FloatVector2& startPos, const FloatVector2& intersectionPos,
                          const FloatVector2& rayLength1D, const FloatVector2& rayUnitStepSize,
                          const FloatVector2& rayDir, const int textureSize) {
        float wallX;

        const bool wallIsWestOrEastFacing = WallIsWestOrEastFacing({intersectionPos.x, intersectionPos.y});

        if (!wallIsWestOrEastFacing) {
            float perpWallDist = (rayLength1D.x - rayUnitStepSize.x);
            wallX = startPos.y + perpWallDist * rayDir.y;
        } else {
            float perpWallDist = (rayLength1D.y - rayUnitStepSize.y);
            wallX = startPos.x + perpWallDist * rayDir.x;
        }

        wallX -= floor((wallX));

        int texX = int(wallX * float(textureSize));
        if (!wallIsWestOrEastFacing && rayDir.x > 0) texX = textureSize - texX - 1;
        if (wallIsWestOrEastFacing && rayDir.y < 0) texX = textureSize - texX - 1;

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
                                      FloatPoint cell, int rayX, int texX) {
        const int columnHeight = ((camera.viewportHeight) * camera.distanceToProjectionPlane) / distance;



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
        const int startTextY = static_cast<int>(mainMenu.startButton.pos.y - mainMenu.startButton.height/2);

        SDL_UnlockTexture(streamingFrameTexture);
        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        // UI draw calls

        DrawButton(mainMenu.startButton);
        DrawTextStr("MiniFPS", mainMenu.font, {static_cast<float>(titleTextX), static_cast<float>(titleTextY)}, titleTextWidth);
        DrawTextStr(mainMenu.settings.version, mainMenu.font, {static_cast<float>(versionTextX), static_cast<float>(versionTextY)}, versionTextWidth);

        // TODO: Scale this properly with resolution, it doesn't stay with button atm
        DrawTextStrH("Start", mainMenu.font, {static_cast<float>(startTextX), static_cast<float>(startTextY)}, mainMenu.startButton.height);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }

    void Renderer::DrawEnemies(const Player& player, const std::vector<Enemy>& enemies, void* pixels, int pitch) {
        std::vector<std::pair<float, Enemy>> enemyDistances;

        const Camera& cam = player.camera;

        for (const Enemy& enemy : enemies) {
            enemyDistances.emplace_back(player.camera.pos.Distance(enemy.pos), enemy);
        }

        std::sort(enemyDistances.begin(), enemyDistances.end(), CompareEnemyDistancePair);

        for (auto it = enemyDistances.begin(); it != enemyDistances.end(); ++it) {
            FloatVector2 enemy = it->second.pos - cam.pos;

            float invDet = 1.0f / (cam.plane.x * cam.direction.y - cam.direction.x * cam.plane.y);

            FloatVector2 transform = {
                    invDet * (cam.direction.y * enemy.x - cam.direction.x * enemy.y),
                    invDet * (-cam.plane.y * enemy.x + cam.plane.x * enemy.y)
            };

            int enemyScreenX = static_cast<int>((cam.viewportWidth / 2) * (1 + transform.x / transform.y));

            int enemyHeight = std::abs(static_cast<int>(cam.viewportHeight / (transform.y)));
            int drawStartY = -enemyHeight / 2 + cam.viewportHeight / 2;
            if (drawStartY < 0) drawStartY = 0;
            int drawEndY = enemyHeight / 2 + cam.viewportHeight / 2;
            if (drawEndY >= cam.viewportHeight) drawEndY = cam.viewportHeight - 1;

            int enemyWidth = std::abs(static_cast<int>(cam.viewportHeight / (transform.y)));
            int drawStartX = -enemyWidth / 2 + enemyScreenX;
            if (drawStartX < 0) drawStartX = 0;
            int drawEndX = enemyWidth / 2 + enemyScreenX;
            if (drawEndX >= cam.viewportWidth) drawEndX = cam.viewportWidth - 1;

            for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
                if (transform.y > 0 && transform.y < zBuffer[stripe]) {
                    for (int y = drawStartY; y < drawEndY; y++) {
                        SetPixel(pixels, pitch, BLACK, {stripe, y});
                    }
                } else if (transform.y > 0) {
                    std::cout << transform.y << " " << zBuffer[stripe] << std::endl;
                }

//                if (transform.y > 0) {
//                    for (int y = drawStartY; y < drawEndY; y++) {
//                        SetPixel(pixels, pitch, BLACK, {stripe, y});
//                    }
//                }

            }
        }

    }

    void Renderer::Draw(const Player& player, const std::vector<Enemy>& enemies, const Font& font, const float frameDelta) {
        int pitch = -1;
        void* pixels = nullptr;
        SDL_LockTexture(streamingFrameTexture, nullptr, &pixels, &pitch);

        DrawCeiling(player.camera, pixels, pitch);
        DrawFloor(player.camera, pixels, pitch);

        // Cast rays
        for (int ray = 0; ray < player.camera.viewportWidth; ray++) {
            const float rayScreenPos = (2 * ray / float(player.camera.viewportWidth) - 1) * player.camera.aspectRatio;
            // const float rayScreenPos = (2 * ray / float(player.camera.viewportWidth) - 1);
            const float rayAngle = atan2f(player.camera.direction.y, player.camera.direction.x) + atanf(rayScreenPos * tanf(player.camera.horizontalFieldOfView / 2));

            FloatVector2 rayDirection = {
                    // Need to multiply by -1 to avoid flipped rendering
                    player.camera.direction.x + player.camera.plane.x * rayScreenPos * -1,
                    player.camera.direction.y + player.camera.plane.y * rayScreenPos * -1
            };

            rayDirection.Normalize();

            const FloatVector2 deltaDistance = {std::abs(1.0f / rayDirection.x), std::abs(1.0f / rayDirection.y)};
            IntVector2 mapCheck(player.camera.pos);

            FloatVector2 sideDistance;
            IntVector2 step;

            if (rayDirection.x < 0) {
                step.x = -1;
                sideDistance.x = (player.camera.pos.x - static_cast<float>(mapCheck.x)) * deltaDistance.x;
            } else {
                step.x = 1;
                sideDistance.x = (static_cast<float>(mapCheck.x + 1) - player.camera.pos.x) * deltaDistance.x;
            }

            if (rayDirection.y < 0) {
                step.y = -1;
                sideDistance.y = ((player.camera.pos.y - static_cast<float>(mapCheck.y)) * deltaDistance.y);
            } else {
                step.y = 1;
                sideDistance.y = (static_cast<float>(mapCheck.y + 1) - player.camera.pos.y) * deltaDistance.y;
            }

            bool tileFound = false;
            float distance = 0.0f;
            short cellID = -1;
            int side = 0;

            while (!tileFound && distance < player.camera.maxRenderDistance) {
                if (sideDistance.x < sideDistance.y) {
                    distance = sideDistance.x;
                    sideDistance.x += deltaDistance.x;
                    mapCheck.x += step.x;
                    side = 0;

                } else {
                    distance = sideDistance.y;
                    sideDistance.y += deltaDistance.y;
                    mapCheck.y += step.y;
                    side = 1;
                }

                if (player.level->IsPositionValid({static_cast<float>(mapCheck.x), static_cast<float>(mapCheck.y)})) {
                    cellID = player.level->Get({mapCheck.x, mapCheck.y});
                    if (cellID != 0) {
                        tileFound = true;
                    }
                }
            }

            if (side == 0) distance = sideDistance.x - deltaDistance.x;
            else           distance = sideDistance.y - deltaDistance.y;

            if (!tileFound) distance = 1000.0f;
            zBuffer[ray] = distance;

            FloatVector2 intersection;
            if (tileFound) {
                intersection = player.camera.pos;
                intersection += rayDirection * distance;
            }

            const Texture texture = GetTexBuffer(cellID);
            const int texX = GetTexX(player.camera.pos, intersection, sideDistance, deltaDistance, rayDirection, texture.size);

            DrawTexturedColumn(texture, player.camera, pixels, pitch, distance * cosf(rayAngle - atan2f(player.camera.direction.y, player.camera.direction.x)), {intersection.x, intersection.y}, ray, texX);
        }

        DrawEnemies(player, enemies, pixels, pitch);

        const int weaponTextureSize = player.camera.viewportWidth / 4;
        CopyTextureToFrameTexture(pixels, pitch, player.weaponTexture, {player.camera.viewportWidth/2, player.camera.viewportHeight - (weaponTextureSize/2)}, weaponTextureSize, weaponTextureSize);

        SDL_UnlockTexture(streamingFrameTexture);

        SDL_SetRenderTarget(sdlRenderer, renderFrameTexture);
        SDL_RenderCopy(sdlRenderer, streamingFrameTexture, nullptr, nullptr);

        std::string planeInfo = std::to_string(player.camera.plane.x);
        planeInfo += " ";
        planeInfo += std::to_string(player.camera.plane.y);

        std::string directionInfo = std::to_string(player.camera.direction.x);
        directionInfo += " ";
        directionInfo += std::to_string(player.camera.direction.y);

        std::string dotProductInfo = std::to_string(FloatVector2::DotProduct(player.camera.plane, player.camera.direction));

        // TODO: Scale this with frame
        // UI draw here
        DrawTextStrH(planeInfo, font, {25, 25}, 25, 255, 0, 0);
        DrawTextStrH(directionInfo, font, {25, 50}, 25, 255, 0, 0);
        DrawTextStrH(dotProductInfo, font, {25, 75}, 25, 255, 0, 0);

        SDL_SetRenderTarget(sdlRenderer, nullptr);
        SDL_RenderCopy(sdlRenderer, renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(sdlRenderer);
    }

    bool Renderer::CompareEnemyDistancePair(const std::pair<float, Enemy>& pair1, const std::pair<float, Enemy>& pair2) {
        return pair1.first > pair2.first;
    }
}