#include "Game.h"

MiniFPS::Game::Game()
{
    InitSDLSubsystems();

    m_settings = Settings::LoadSettings(GetSDLAssetsFolderPath(), "settings.json");
    m_audio = AudioHandler(GetSDLAssetsFolderPath() + "audio/", m_settings);
    m_fontManager = FontManager(m_settings);

    LoadTextures();

    if (!InitWindow(&m_window, m_settings.screenWidth, m_settings.screenHeight))
    {
        std::cerr << "Window could not be initialized" << std::endl;
    }

    if (!InitRenderer(m_window, &m_sdlRenderer, m_settings.vSync))
    {
        std::cerr << "Renderer could not be initialized" << std::endl;
    }

    m_renderer = Renderer(m_sdlRenderer, m_settings);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    m_mainMenu = MainMenu(m_settings, m_fontManager.m_fonts[0]);
}

void MiniFPS::Game::Update()
{
    if (m_mainMenu.m_player.InMainMenu() && !m_mainMenu.m_player.GameHasEnded())
    {
        m_renderer.DrawMainMenu(m_mainMenu);
        m_mainMenu.Update();
    }
    else if (!m_gamePlayer.GameHasEnded())
    {
        if (!m_gameSetup)
        {
            SetupGame();
            m_gameSetup = true;
        }

        m_oldTime = m_curTime;
        m_curTime = static_cast<float>(SDL_GetTicks64());
        m_frameDelta = GetFrameTime(m_oldTime, m_curTime);

        m_gamePlayer.Update(m_frameDelta, m_settings.speedModifier, m_settings.rotationModifier);

        if (m_gamePlayer.m_inputState.leftMouseButtonPressed)
        {
            if (m_gamePlayer.Shoot(m_enemies, m_renderer.m_zBuffer[m_gamePlayer.m_camera.viewportWidth / 2], m_audio))
            {
                // audio.PlayEffect("testEffect");
            }
        }

        if (m_gamePlayer.m_inputState.rightMouseButtonPressed)
        {
            m_gamePlayer.Reload(m_audio);
        }

        m_renderer.DrawGame(m_gamePlayer, m_enemies, m_fontManager.m_fonts[0]);
    }
}

bool MiniFPS::Game::IsRunning()
{
    return !m_gamePlayer.GameHasEnded();
}

MiniFPS::Game::~Game()
{
    FreeResources(m_renderer, m_audio, m_fontManager);
    ShutdownSDLSubsystems();
    Quit(m_window, m_sdlRenderer);
}

void MiniFPS::Game::LoadTextures()
{
    const std::vector<std::string> spriteFileNames = GetFilesInDirectory(GetSDLAssetsFolderPath() + "sprites/");

    for (const auto &file: spriteFileNames)
    {
        const std::string name = file.substr(0, file.size() - 4);

        const Texture newTexture(name, GetSDLAssetsFolderPath() + "sprites/" + file);
        m_textureNameToTextureMap[name] = newTexture;
    }
}

void MiniFPS::Game::SetupGame()
{
    m_level = Level(GetSDLAssetsFolderPath() + m_settings.levelPath);

    for (const auto &pair: m_textureNameToTextureMap)
    {
        for (const auto &x: m_level.m_textureIdMap)
        {
            if (x.second == pair.first)
            {
                m_textureMap[x.first] = pair.second;
            }
        }
    }

    m_textureMap[-1] = m_textureNameToTextureMap["fallback"];

    for (const auto &pair: m_level.m_enemySpawnLocations)
    {
        m_enemies.emplace_back(pair.second, pair.first);
    }

    m_gamePlayer = Player(&m_level, m_settings);
    m_gamePlayer.m_weaponTexture = m_textureNameToTextureMap["Player_Weapon"];

    m_renderer.SetTextureMap(m_textureMap);

    // Disable movement of cursor in game
    SDL_SetRelativeMouseMode(SDL_TRUE);

    m_audio.PlayTrack("DrumLoop1", -1);
}
