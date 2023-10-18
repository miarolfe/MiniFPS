#include "Settings.h"

namespace MiniFPS
{
    Settings::Settings()
    {
        screenWidth = 0;
        screenHeight = 0;
        renderDistance = 0;
        vSync = false;
        fieldOfView = 0;
        speedModifier = 0;
        rotationModifier = 0;
        playerStartPos = {0.0, 0.0};
        effectVolume = 0;
        musicVolume = 0;
    }

    Settings::Settings(const string& version, int screenWidth, int screenHeight,
                       int renderDistance, bool vSync,
                       float fieldOfView, float speedModifier, float rotationModifier, const Vec2& playerStartPos,
                       float effectVolume, float musicVolume,
                       const string& levelPath, const std::map<string, string>& fontPaths)
    {
        this->version = version;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->renderDistance = renderDistance;
        this->fieldOfView = fieldOfView;
        this->speedModifier = speedModifier;
        this->rotationModifier = rotationModifier;
        this->playerStartPos = playerStartPos;
        this->effectVolume = effectVolume;
        this->musicVolume = musicVolume;
        this->levelPath = levelPath;
        this->vSync = vSync;

        for (const auto& font: fontPaths)
        {
            this->fontPaths.emplace_back(font);
        }
    }

    Settings Settings::LoadSettings(const string& assetsFilePath, const string& settingsFilePath)
    {
        std::ifstream f(assetsFilePath + settingsFilePath);

        if (!f.is_open())
        {
            // TODO
        }

        json settingsAsJson = json::parse(f);
        int major, minor, patch;
        string version;
        int screenWidth, screenHeight, renderDistance;
        float fieldOfView, speedModifier, rotationModifier,
            playerStartAngle, effectVolume, musicVolume;
        Vec2 playerStartPos;
        string levelPath;
        std::map<string, string> texturePaths;
        std::map<string, string> fontPaths;
        bool vSync;

        major = settingsAsJson["game"]["version"]["major"];
        minor = settingsAsJson["game"]["version"]["minor"];
        patch = settingsAsJson["game"]["version"]["patch"];

        std::stringstream versionStringStream;
        versionStringStream << "v" << major << "." << minor << "." << patch;
        version = versionStringStream.str();

        screenWidth = settingsAsJson["graphics"]["screenWidth"];
        screenHeight = settingsAsJson["graphics"]["screenHeight"];
        renderDistance = settingsAsJson["rendering"]["renderDistance"];
        fieldOfView = settingsAsJson["graphics"]["horizontalFieldOfView"];
        vSync = settingsAsJson["graphics"]["vSync"];
        speedModifier = settingsAsJson["player"]["speedModifier"];
        rotationModifier = settingsAsJson["player"]["rotationModifier"];
        playerStartPos = {settingsAsJson["player"]["startX"], settingsAsJson["player"]["startY"]};
        effectVolume = settingsAsJson["audio"]["effectVolume"];
        musicVolume = settingsAsJson["audio"]["musicVolume"];
        levelPath = settingsAsJson["files"]["levelPath"];
        fontPaths = settingsAsJson["files"]["fontPaths"];

        Settings settings(version, screenWidth, screenHeight, renderDistance, vSync, fieldOfView,
                          speedModifier,
                          rotationModifier, playerStartPos,
                          effectVolume, musicVolume, levelPath, fontPaths);

        return settings;
    }
}