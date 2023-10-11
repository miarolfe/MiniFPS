#include "Renderer.h"
#include "Utilities.h"

namespace MiniFPS
{
    Renderer::Renderer() = default;

    Renderer::Renderer(SDL_Renderer* sdlRenderer, const Settings& settings) : m_sdlRenderer(sdlRenderer)
    {
        m_streamingFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                    static_cast<int>(settings.screenWidth),
                                                    static_cast<int>(settings.screenHeight));
        m_renderFrameTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
                                                 static_cast<int>(settings.screenWidth),
                                                 static_cast<int>(settings.screenHeight));

        m_zBuffer = static_cast<float*>(malloc(sizeof(float) * settings.screenWidth));
    }

    void Renderer::SetTextureMap(const std::unordered_map<short, Texture>& newTextureMap)
    {
        m_textureMap = newTextureMap;
    }

    void Renderer::SetPixel(SDLTextureBuffer buffer, Color color, const Vec2Int& point)
    {
        uint32_t* row = (uint32_t*) ((uint8_t*) buffer.pixels + point.y * buffer.pitch);
        row[point.x] = color.argb;
    }

    bool Renderer::ShouldShadePixel(const Vec2& point)
    {
        const float hitX = point.x - floor(point.x + 0.5f); // Fractional part of cellX
        const float hitY = point.y - floor(point.y + 0.5f); // Fractional part of cellY

        bool shouldShadePixel = false;

        if (std::abs(hitY) > std::abs(hitX))
        { // West-East
            shouldShadePixel = true;
        }

        return shouldShadePixel;
    }

    Texture Renderer::GetTexBuffer(short textureId)
    {
        Texture texture;

        if (m_textureMap.count(textureId) == 1)
        {
            texture = m_textureMap[textureId];
        }
        else
        {
            std::cerr << "Invalid texture: no texture mapped to id " << textureId << std::endl;
            texture = m_textureMap[-1]; // Fallback texture
        }

        return texture;
    }

    bool Renderer::WallIsWestOrEastFacing(const Vec2& point)
    {
        return (std::abs(point.x - floor(point.x + 0.5f)) > std::abs(point.y - floor(point.y + 0.5f)));
    }

    int Renderer::GetTexX(const Vec2& startPos, const Vec2& intersectionPos,
                          const Vec2& rayLength1D, const Vec2& rayUnitStepSize,
                          const Vec2& rayDir, const int textureSize)
    {
        float wallX;

        const bool wallIsWestOrEastFacing = WallIsWestOrEastFacing({intersectionPos.x, intersectionPos.y});

        if (!wallIsWestOrEastFacing)
        {
            float perpWallDist = (rayLength1D.x - rayUnitStepSize.x);
            wallX = startPos.y + perpWallDist * rayDir.y;
        }
        else
        {
            float perpWallDist = (rayLength1D.y - rayUnitStepSize.y);
            wallX = startPos.x + perpWallDist * rayDir.x;
        }

        wallX -= floor((wallX));

        int texX = int(wallX * float(textureSize));
        if (!wallIsWestOrEastFacing && rayDir.x > 0)
        { texX = textureSize - texX - 1; }
        if (wallIsWestOrEastFacing && rayDir.y < 0)
        { texX = textureSize - texX - 1; }

        return texX;
    }

    void
    Renderer::CopyTextureToFrameTexture(SDLTextureBuffer buffer, const Texture& texture, const Vec2Int& point, int w, int h)
    {
        const float scaleX = static_cast<float>(texture.size) / w;
        const float scaleY = static_cast<float>(texture.size) / h;

        for (int destinationY = 0; destinationY < h; destinationY++)
        {
            for (int destinationX = 0; destinationX < w; destinationX++)
            {
                const int sourceX = static_cast<int>(destinationX * scaleX);
                const int sourceY = static_cast<int>(destinationY * scaleY);

                const Color pixel = texture.buffer[sourceY][sourceX];

                // Check if alpha value is 0 (transparent)
                if ((pixel.argb & TRANSPARENCY_MASK) != 0)
                {
                    SetPixel(buffer, pixel, {point.x + destinationX - (w / 2), point.y + destinationY - (h / 2)});
                }
            }
        }
    }

    void Renderer::FreeTextures()
    {
        for (const auto& idTexturePair: m_textureMap)
        {
            for (int row = 0; row < idTexturePair.second.size; row++)
            {
                delete[] idTexturePair.second.buffer[row];
            }

            delete[] idTexturePair.second.buffer;
        }
    }

    void
    Renderer::DrawTexturedColumn(const Texture& texture, const Camera& camera, SDLTextureBuffer buffer, float distance,
                                 const Vec2& cell, int rayX, int texX)
    {
        const int columnHeight = ((camera.viewportHeight)) / distance;

        const bool shadePixel = ShouldShadePixel(cell);

        const int drawStart = ((camera.viewportHeight / 2) - (columnHeight / 2));
        const int drawEnd = drawStart + columnHeight;

        for (int rayY = drawStart; rayY < drawEnd; rayY++)
        {
            if (rayY < 0 || rayY >= camera.viewportHeight)
            {
                continue;
            }

            const int texY = ((rayY - drawStart) * texture.size) / columnHeight;

            if (shadePixel)
            {
                const Color shadedTexPixel = Color::ShadePixel(texture.buffer[texY][texX]);
                SetPixel(buffer, shadedTexPixel, {rayX, rayY});
            }
            else
            {
                SetPixel(buffer, texture.buffer[texY][texX], {rayX, rayY});
            }
        }
    }

    void Renderer::DrawCeiling(const Camera& camera, SDLTextureBuffer buffer)
    {
        for (int frameY = 0; frameY < camera.viewportHeight / 2; frameY++)
        {
            for (int frameX = 0; frameX < camera.viewportWidth; frameX++)
            {
                SetPixel(buffer, CEILING, {frameX, frameY});
            }
        }
    }

    void Renderer::DrawFloor(const Camera& camera, SDLTextureBuffer buffer)
    {
        for (int frameY = static_cast<int>(camera.viewportHeight / 2); frameY < camera.viewportHeight; frameY++)
        {
            for (int frameX = 0; frameX < camera.viewportWidth; frameX++)
            {
                SetPixel(buffer, FLOOR, {frameX, frameY});
            }
        }
    }

    void Renderer::DrawButton(Button button)
    {
        SDL_SetRenderTarget(m_sdlRenderer, m_renderFrameTexture);

        // TODO: Make this more efficient
        SDL_Texture* buttonTexture = SDL_CreateTexture(m_sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                                                       SDL_TEXTUREACCESS_STREAMING, button.m_width, button.m_height);

        const SDL_Rect sdlRect{
            static_cast<int>(button.GetLeftBound()),
            static_cast<int>(button.GetTopBound()),
            static_cast<int>(button.m_width),
            static_cast<int>(button.m_height)
        };

        SDLTextureBuffer buffer;
        SDL_LockTexture(buttonTexture, nullptr, &buffer.pixels, &buffer.pitch);

        for (int x = 0; x < button.m_width; x++)
        {
            for (int y = 0; y < button.m_height; y++)
            {
                SetPixel(buffer, BUTTON, {x, y});
            }
        }

        SDL_UnlockTexture(buttonTexture);

        SDL_RenderCopy(m_sdlRenderer, buttonTexture, nullptr, &sdlRect);
        SDL_SetRenderTarget(m_sdlRenderer, nullptr);

        SDL_DestroyTexture(buttonTexture);
    }

    void Renderer::DrawTextStr(const string& text, const Font& font, const Vec2& point, int width, int r = 255,
                               int g = 255, int b = 255)
    {
        SDL_SetRenderTarget(m_sdlRenderer, m_renderFrameTexture);

        int requestedWidth = 0;
        int requestedHeight = 0;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        const float ratio = static_cast<float>(requestedWidth) / static_cast<float>(requestedHeight);

        const int height = static_cast<int>(static_cast<float>(width) / ratio);

        const SDL_Rect destRect{static_cast<int>(point.x), static_cast<int>(point.y), width, height};

        SDL_Texture* texture = RenderTextToTexture(m_sdlRenderer, font, text, r, g, b);

        SDL_RenderCopy(m_sdlRenderer, texture, nullptr, &destRect);
        SDL_SetRenderTarget(m_sdlRenderer, nullptr);

        SDL_DestroyTexture(texture);
    }

    void Renderer::DrawTextStrH(const string& text, const Font& font, const Vec2& point, int height, int r = 255,
                                int g = 255, int b = 255)
    {
        SDL_SetRenderTarget(m_sdlRenderer, m_renderFrameTexture);

        int requestedWidth = 0;
        int requestedHeight = 0;
        TTF_SizeUTF8(font.ttf, text.c_str(), &requestedWidth, &requestedHeight);
        const float ratio = static_cast<float>(requestedHeight) / static_cast<float>(requestedWidth);

        const int width = static_cast<int>(static_cast<float>(height) / ratio);

        const SDL_Rect destRect{static_cast<int>(point.x), static_cast<int>(point.y), width, height};

        SDL_Texture* texture = RenderTextToTexture(m_sdlRenderer, font, text, r, g, b);

        SDL_RenderCopy(m_sdlRenderer, texture, nullptr, &destRect);
        SDL_SetRenderTarget(m_sdlRenderer, nullptr);

        SDL_DestroyTexture(texture);
    }

    void Renderer::DrawMainMenu(const MainMenu& mainMenu)
    {
        SDLTextureBuffer buffer;

        SDL_LockTexture(m_streamingFrameTexture, nullptr, &buffer.pixels, &buffer.pitch);

        for (int frameY = 0; frameY < static_cast<int>(mainMenu.m_player.m_camera.viewportHeight); frameY++)
        {
            for (int frameX = 0; frameX < static_cast<int>(mainMenu.m_player.m_camera.viewportWidth); frameX++)
            {
                SetPixel(buffer, MAIN_MENU_BACKGROUND, {frameX, frameY});
            }
        }

        const int titleTextX = static_cast<int>(mainMenu.m_player.m_camera.viewportWidth / 4);
        const int titleTextY = static_cast<int>(mainMenu.m_player.m_camera.viewportHeight / 16);
        const int titleTextWidth = static_cast<int>(mainMenu.m_player.m_camera.viewportWidth / 2);

        const int versionTextX = static_cast<int>(3 * (mainMenu.m_player.m_camera.viewportWidth / 8));
        const int versionTextY = static_cast<int>(5 * (mainMenu.m_player.m_camera.viewportHeight / 16));
        const int versionTextWidth = static_cast<int>(mainMenu.m_player.m_camera.viewportWidth / 4);

        const int startTextX = static_cast<int>(mainMenu.m_startButton.m_pos.x - mainMenu.m_startButton.m_width / 4);
        const int startTextY = static_cast<int>(mainMenu.m_startButton.m_pos.y - mainMenu.m_startButton.m_height / 2);

        SDL_UnlockTexture(m_streamingFrameTexture);
        SDL_SetRenderTarget(m_sdlRenderer, m_renderFrameTexture);
        SDL_RenderCopy(m_sdlRenderer, m_streamingFrameTexture, nullptr, nullptr);

        // UI draw calls

        DrawButton(mainMenu.m_startButton);
        DrawTextStr("MiniFPS", mainMenu.m_font, {static_cast<float>(titleTextX), static_cast<float>(titleTextY)},
                    titleTextWidth);
        DrawTextStr(mainMenu.m_settings.version, mainMenu.m_font,
                    {static_cast<float>(versionTextX), static_cast<float>(versionTextY)}, versionTextWidth);

        // TODO: Scale this properly with resolution, it doesn't stay with button atm
        DrawTextStrH("Start", mainMenu.m_font, {static_cast<float>(startTextX), static_cast<float>(startTextY)},
                     mainMenu.m_startButton.m_height);

        SDL_SetRenderTarget(m_sdlRenderer, nullptr);
        SDL_RenderCopy(m_sdlRenderer, m_renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(m_sdlRenderer);
    }

    void Renderer::DrawEnemies(const Player& player, std::vector<Enemy>& enemies, SDLTextureBuffer buffer)
    {
        std::vector<std::pair<float, Enemy>> enemyDistances;

        const Camera& cam = player.m_camera;

        for (Enemy& enemy: enemies)
        {
            if (enemy.IsVisible())
            {
                enemyDistances.emplace_back(player.m_camera.pos.Distance(enemy.m_pos), enemy);
            }
        }

        std::sort(enemyDistances.begin(), enemyDistances.end(), CompareEnemyDistancePair);

        for (auto& enemyDistance: enemyDistances)
        {
            const Vec2 enemyPos = enemyDistance.second.m_pos - cam.pos;
            const Texture& texture = GetTexBuffer(enemyDistance.second.m_textureId);

            const float invDet = 1.0f / (cam.plane.x * cam.direction.y - cam.direction.x * cam.plane.y);

            const Vec2 transform = {
                invDet * ((cam.direction.y * enemyPos.x) - (cam.direction.x * enemyPos.y)) * -1.0f,
                invDet * ((-cam.plane.y * enemyPos.x) + (cam.plane.x * enemyPos.y))
            };

            const int enemyScreenX = static_cast<int>((cam.viewportWidth / 2) * (1 + transform.x / transform.y));

            const int enemyHeight = std::min(2000, std::abs(static_cast<int>(cam.viewportHeight / (transform.y))));
            int drawStartY = -enemyHeight / 2 + cam.viewportHeight / 2;
            if (drawStartY < 0)
            { drawStartY = 0; }
            int drawEndY = enemyHeight / 2 + cam.viewportHeight / 2;
            if (drawEndY >= cam.viewportHeight)
            { drawEndY = cam.viewportHeight - 1; }

            const int enemyWidth = std::min(2000, std::abs(static_cast<int>(cam.viewportHeight / (transform.y))));
            int drawStartX = (-enemyWidth / 2) + enemyScreenX;
            if (drawStartX < 0)
            { drawStartX = 0; }
            int drawEndX = (enemyWidth / 2) + enemyScreenX;
            if (drawEndX >= cam.viewportWidth)
            { drawEndX = cam.viewportWidth - 1; }

            for (int stripe = drawStartX; stripe < drawEndX; stripe++)
            {
                const int texX = static_cast<int>(256 * (stripe - (-enemyWidth / 2 + enemyScreenX)) * texture.size /
                                                  enemyWidth) / 256;
                if (transform.y > 0 && transform.y < m_zBuffer[stripe])
                {
                    for (int y = drawStartY; y < drawEndY; y++)
                    {
                        const int d = (y) * 256 - cam.viewportHeight * 128 + enemyHeight * 128;
                        const int texY = ((d * texture.size) / enemyHeight) / 256;
                        const Color pixel = texture.buffer[texY][texX];
                        if ((pixel.argb & TRANSPARENCY_MASK) != 0)
                        {
                            SetPixel(buffer, pixel, {stripe, y});
                        }
                    }
                }
            }
        }
    }

    void Renderer::DrawGame(const Player& player, std::vector<Enemy>& enemies, const Font& font)
    {
        SDLTextureBuffer buffer;
        SDL_LockTexture(m_streamingFrameTexture, nullptr, &buffer.pixels, &buffer.pitch);

        DrawCeiling(player.m_camera, buffer);
        DrawFloor(player.m_camera, buffer);

        // Cast rays
        for (int ray = 0; ray < player.m_camera.viewportWidth; ray++)
        {
            RaycastResult result = CastRay(ray, player);
            m_zBuffer[ray] = result.adjustedDistance;

            Vec2 intersection;
            if (result.collided)
            {
                intersection = player.m_camera.pos;
                intersection += result.direction * result.distance;
            }

            const Texture texture = GetTexBuffer(result.id);
            const int texX = GetTexX(player.m_camera.pos, intersection, result.sideDistance, result.deltaDistance,
                                     result.direction,
                                     texture.size);

            DrawTexturedColumn(
                texture,
                player.m_camera,
                buffer,
                result.adjustedDistance,
                intersection,
                ray,
                texX);
        }

        DrawEnemies(player, enemies, buffer);

        const int weaponTextureSize = player.m_camera.viewportWidth / 4;
        CopyTextureToFrameTexture(buffer, player.m_weaponTexture, {player.m_camera.viewportWidth / 2,
                                                                   player.m_camera.viewportHeight -
                                                                   (weaponTextureSize / 2)}, weaponTextureSize,
                                  weaponTextureSize);

        SDL_UnlockTexture(m_streamingFrameTexture);

        SDL_SetRenderTarget(m_sdlRenderer, m_renderFrameTexture);
        SDL_RenderCopy(m_sdlRenderer, m_streamingFrameTexture, nullptr, nullptr);

        // TODO: Scale this with frame
        // UI draw here

        string healthDisplay =
            "HP:" + std::to_string(player.m_currentHealth) + "/" + std::to_string(Player::MAX_HEALTH);
        DrawTextStrH(healthDisplay, font, {25, 25}, 25, 255, 255, 255);

        string ammoDisplay;
        if (player.m_reloading)
        {
            ammoDisplay = "AMMO:RELOADING...";
        }
        else
        {
            ammoDisplay = "AMMO:" + std::to_string(player.m_currentAmmo) + "/" + std::to_string(Player::MAG_SIZE);
        }
        DrawTextStrH(ammoDisplay, font, {25, 50}, 25, 255, 255, 255);

        SDL_SetRenderTarget(m_sdlRenderer, nullptr);
        SDL_RenderCopy(m_sdlRenderer, m_renderFrameTexture, nullptr, nullptr);
        SDL_RenderPresent(m_sdlRenderer);
    }

    bool Renderer::CompareEnemyDistancePair(const std::pair<float, Enemy>& pair1, const std::pair<float, Enemy>& pair2)
    {
        return pair1.first > pair2.first;
    }

    RaycastResult Renderer::CastRay(int column, const Player& player)
    {
        RaycastResult result;

        const float rayScreenPos = (
            2.0f * static_cast<float>(column) / static_cast<float>((player.m_camera.viewportWidth)) - 1.0f);
        const float rayAngle = atan2f(player.m_camera.direction.y, player.m_camera.direction.x) +
                               atanf(rayScreenPos * tanf(player.m_camera.horizontalFieldOfView / 2));

        result.direction = {
            // Need to multiply by -1 to avoid flipped rendering
            player.m_camera.direction.x + player.m_camera.plane.x * rayScreenPos * -1.0f,
            player.m_camera.direction.y + player.m_camera.plane.y * rayScreenPos * -1.0f
        };

        result.direction.Normalize();

        const Vec2 deltaDistance = {std::abs(1.0f / result.direction.x), std::abs(1.0f / result.direction.y)};
        Vec2Int mapCheck(player.m_camera.pos);

        Vec2 sideDistance;
        Vec2Int step;

        if (result.direction.x < 0)
        {
            step.x = -1;
            sideDistance.x = (player.m_camera.pos.x - static_cast<float>(mapCheck.x)) * deltaDistance.x;
        }
        else
        {
            step.x = 1;
            sideDistance.x = (static_cast<float>(mapCheck.x + 1) - player.m_camera.pos.x) * deltaDistance.x;
        }

        if (result.direction.y < 0)
        {
            step.y = -1;
            sideDistance.y = ((player.m_camera.pos.y - static_cast<float>(mapCheck.y)) * deltaDistance.y);
        }
        else
        {
            step.y = 1;
            sideDistance.y = (static_cast<float>(mapCheck.y + 1) - player.m_camera.pos.y) * deltaDistance.y;
        }

        int side = 0;

        while (!result.collided && result.distance < player.m_camera.maxRenderDistance)
        {
            if (sideDistance.x < sideDistance.y)
            {
                result.distance = sideDistance.x;
                sideDistance.x += deltaDistance.x;
                mapCheck.x += step.x;
                side = 0;

            }
            else
            {
                result.distance = sideDistance.y;
                sideDistance.y += deltaDistance.y;
                mapCheck.y += step.y;
                side = 1;
            }

            if (player.m_level->IsPositionValid({static_cast<float>(mapCheck.x), static_cast<float>(mapCheck.y)}))
            {
                result.id = player.m_level->Get({mapCheck.x, mapCheck.y});
                if (result.id != 0)
                {
                    result.collided = true;
                }
            }
        }

        if (side == 0)
        { result.distance = sideDistance.x - deltaDistance.x; }
        else
        { result.distance = sideDistance.y - deltaDistance.y; }

        if (!result.collided)
        { result.distance = 1000.0f; }

        const float adjustedDistance =
            result.distance * cosf(rayAngle - atan2f(player.m_camera.direction.y, player.m_camera.direction.x));
        result.adjustedDistance = adjustedDistance;

        result.deltaDistance = deltaDistance;
        result.sideDistance = sideDistance;

        return result;
    }
}