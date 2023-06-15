#pragma once

#include <string>
#include <vector>

#include <SDL_mixer.h>

namespace MiniFPS {
    struct Effect {
        Effect(const std::string& name, Mix_Chunk* chunk);
        std::string name;
        Mix_Chunk* chunk;
    };

    struct Track {
        Track(const std::string& name, Mix_Music* music);
        std::string name;
        Mix_Music* music;
    };

    class Audio {
    public:
        explicit Audio(const std::string& audioFolderPath);
        std::vector<Effect> effects;
        std::vector<Track> tracks;
    };
}

