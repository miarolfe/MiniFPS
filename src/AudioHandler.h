#pragma once

#include "Common.h"
#include "LogHandler.h"
#include "Settings.h"

namespace MiniFPS
{
    struct Effect
    {
        Effect();

        explicit Effect(Mix_Chunk* chunk);

        Mix_Chunk* chunk;
    };

    struct Track
    {
        Track();

        explicit Track(Mix_Music* music);

        Mix_Music* music;
    };

    class AudioHandler
    {
    private:
        AudioHandler();
        explicit AudioHandler(const string& audioFolderPath, const Settings& settings);

        std::map<string, Effect> m_effects;
        std::map<string, Track> m_tracks;
        float m_effectVolume;
        float m_musicVolume;

        static AudioHandler* s_instance;
        static bool s_initialized;

    public:
        static AudioHandler& GetInstance(const string& audioFolderPath = "", const Settings& settings = Settings());

        AudioHandler(const AudioHandler&) = delete; // No copy
        AudioHandler& operator=(const AudioHandler&) = delete; // No assignment

        bool PlayEffect(const string& name, int loops = 0);
        bool PlayTrack(const string& name, int loops = 0);
        void SetEffectVolume(float volume);
        void SetMusicVolume(float volume);
        static void Pause();
        static void Resume();
        void FreeEffects();
        void FreeTracks();
    };
}

