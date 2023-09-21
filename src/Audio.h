#pragma once

#include "Common.h"
#include "Settings.h"

namespace MiniFPS {
    struct Effect {
        Effect();
        explicit Effect(Mix_Chunk* chunk);
        Mix_Chunk* chunk;
    };

    struct Track {
        Track();
        explicit Track(Mix_Music* music);
        Mix_Music* music;
    };

    class Audio {
    private:
        std::map<string, Effect> effects;
        std::map<string, Track> tracks;
        float effectVolume;
        float musicVolume;

    public:
        Audio();
        explicit Audio(const string& audioFolderPath, const Settings& settings);
        bool PlayEffect(const string& name, int loops=0);
        bool PlayTrack(const string& name, int loops=0);
        void SetEffectVolume(float volume);
        void SetMusicVolume(float volume);
        static void Pause();
        static void Resume();
        void FreeEffects();
        void FreeTracks();
    };
}

