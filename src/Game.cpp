#include "Game.h"

MiniFPS::Game::Game()
{
    if (!InitSDL())
    {
        string err = "Failed to initialize SDL: ";
        err += SDL_GetError();

        LogHandler::LogError(err.c_str());
    }
    else
    {
        LogHandler::Log("Initialized SDL");
    }

    if (!InitSDLImage())
    {
        LogHandler::LogError("Failed to initialize SDL_image");
    }
    else
    {
        LogHandler::Log("Initialized SDL_image");
    }

    if (!InitSDLMixer())
    {
        LogHandler::LogError("Failed to initialize SDL_mixer");
    }
    else
    {
        LogHandler::Log("Initialized SDL_mixer");
    }

    if (!InitSDLTTF())
    {
        LogHandler::LogError("Failed to initialize SDL_ttf");
    }
    else
    {
        LogHandler::Log("Initialized SDL_ttf");
    }

    m_settings = Settings::LoadSettings(GetSDLAssetsFolderPath(), "settings.json");

    // Load in audio files
    AudioHandler::GetInstance(GetSDLAssetsFolderPath() + "audio/", m_settings);

    // Load in font files
    m_fontManager = FontManager(m_settings);

    LoadTextures();

    if (!InitWindow(&m_window, m_settings.screenWidth, m_settings.screenHeight))
    {
        LogHandler::LogError("Window could not be initialized");
    }
    else
    {
        LogHandler::Log("Window initialized");
    }

    if (!InitRenderer(m_window, &m_sdlRenderer, m_settings.vSync))
    {
        LogHandler::LogError("Renderer could not be initialized");
    }
    else
    {
        LogHandler::Log("Renderer initialized");
    }

    const float RENDER_SCALE = 0.5f;

    m_cameraResolution =
    {
        static_cast<int>(m_settings.screenWidth * RENDER_SCALE),
        static_cast<int>(m_settings.screenHeight * RENDER_SCALE)
    };

    m_renderer = Renderer(m_sdlRenderer, m_settings, m_cameraResolution);
    m_mainMenu = MainMenu(m_settings, m_fontManager.m_fonts[0], m_cameraResolution);

    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void MiniFPS::Game::Update()
{
    LogHandler::Update();

    m_oldTime = m_curTime;
    m_curTime = static_cast<float>(SDL_GetTicks64());
    m_frameDelta = GetFrameTime(m_oldTime, m_curTime);

    m_updateEnemyDestinationTimer += m_frameDelta;

    // LogHandler::Log(GetFramesPerSecond(m_frameDelta).c_str());

    if (m_mainMenu.m_player.InMainMenu() && !m_mainMenu.m_player.GameHasEnded())
    {
        m_renderer.DrawMainMenu(m_mainMenu);
        m_mainMenu.Update({m_settings.screenWidth, m_settings.screenHeight}, m_cameraResolution);
    }
    else if (!m_gamePlayer.GameHasEnded() && m_gamePlayer.m_currentHealth > 0)
    {
        if (!m_gameSetup)
        {
            SetupGame();
            m_gameSetup = true;
            LogHandler::Log("Game setup");
        }

        bool updateEnemyDestination = (m_updateEnemyDestinationTimer >= UPDATE_ENEMY_DESTINATION_TIME);

        for (Enemy& enemy : m_enemies)
        {
            if (enemy.IsEnabled())
            {
                if (updateEnemyDestination)
                {
                    enemy.SetDestination(&m_level, m_gamePlayer.m_camera.pos);
                }
                enemy.Update(m_frameDelta, m_gamePlayer);
            }
        }

        if (updateEnemyDestination)
        {
            m_updateEnemyDestinationTimer = 0.0f;
        }

        m_gamePlayer.Update(m_frameDelta, m_settings.speedModifier, m_settings.rotationModifier);

        if (m_gamePlayer.m_inputState.leftMouseButtonPressed)
        {
            if (m_gamePlayer.Shoot(m_enemies, m_renderer.m_zBuffer[m_gamePlayer.m_camera.width / 2]))
            {
                // audio.PlayEffect("testEffect");
            }
        }

        if (m_gamePlayer.m_inputState.rightMouseButtonPressed)
        {
            m_gamePlayer.Reload();
        }

        m_renderer.DrawGame(m_gamePlayer, m_enemies, m_fontManager.m_fonts[0]);
    }
    else
    {
        m_renderer.DrawGameOver(m_gamePlayer, m_mainMenu.m_font);
        m_gamePlayer.UpdateInputState();
    }
}

bool MiniFPS::Game::IsRunning()
{
    return !m_gamePlayer.GameHasEnded();
}

MiniFPS::Game::~Game()
{
    FreeResources(m_renderer, AudioHandler::GetInstance(), m_fontManager);
    ShutdownSDL();
    ShutdownSDLImage();
    ShutdownSDLMixer();
    ShutdownSDLTTF();
    Quit(m_window, m_sdlRenderer);
}

void MiniFPS::Game::LoadTextures()
{
    const std::vector<string> spriteFileNames = GetFilesInDirectory(GetSDLAssetsFolderPath() + "sprites/");

    for (const auto& file: spriteFileNames)
    {
        const string name = file.substr(0, file.size() - 4);

        const Texture newTexture(name, GetSDLAssetsFolderPath() + "sprites/" + file);
        m_textureNameToTextureMap[name] = newTexture;
    }
}

void MiniFPS::Game::SetupGame()
{
    AudioHandler& audioHandler = AudioHandler::GetInstance();

    m_level = Level(GetSDLAssetsFolderPath() + m_settings.levelPath);

    for (const auto& pair: m_textureNameToTextureMap)
    {
        for (const auto& x: m_level.m_textureIdMap)
        {
            if (x.second == pair.first)
            {
                m_textureMap[x.first] = pair.second;
            }
        }
    }

    m_textureMap[-1] = m_textureNameToTextureMap["fallback"];

    for (const auto& pair: m_level.m_enemySpawnLocations)
    {
        m_enemies.emplace_back(pair.second, pair.first);
    }

    m_gamePlayer = Player(&m_level, m_settings, m_cameraResolution);
    m_gamePlayer.m_baseWeaponTexture = m_textureNameToTextureMap["PlayerWeapon-1"];
    m_gamePlayer.m_muzzleFlashWeaponTexture = m_textureNameToTextureMap["PlayerWeapon-2"];

    m_renderer.SetTextureMap(m_textureMap);

    // Disable movement of cursor in game
    SDL_SetRelativeMouseMode(SDL_TRUE);

    audioHandler.PlayTrack("DrumLoop1", -1);
}
