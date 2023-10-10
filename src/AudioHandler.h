#pragma once

#include "Common.h"
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
        std::map<string, Effect> m_effects;
        std::map<string, Track> m_tracks;
        float m_effectVolume;
        float m_musicVolume;

    public:
        AudioHandler();

        explicit AudioHandler(const string& audioFolderPath, const Settings& settings);

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

