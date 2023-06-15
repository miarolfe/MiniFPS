#pragma once

#include <string>
#include <vector>

#include <SDL_mixer.h>

namespace MiniFPS {
    class Audio {
    public:
        explicit Audio(const std::string& audioFolderPath);
        std::vector<Mix_Chunk*> effects;
        std::vector<Mix_Music*> tracks;
    };
}

