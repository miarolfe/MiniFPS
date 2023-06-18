#pragma once

#include <string>
#include <map>

#include <SDL_mixer.h>

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
        std::map<std::string, Effect> effects;
        std::map<std::string, Track> tracks;
    public:
        explicit Audio(const std::string& audioFolderPath);

        /**
         * Play an effect (sound file a few seconds in length at most).
         * @param name The name of the effect.
         * @param loops How many times to loop it (-1 for infinitely looping).
         * @return Whether the effect could be successfully played.
         */
        bool PlayEffect(const std::string& name, int loops=0);

        /**
         * Play a track (sound file longer than a few seconds).
         * @param name The name of the track.
         * @param loops How many times to loop it (-1 for infinitely looping).
         * @return Whether the track could be successfully played.
         */
        bool PlayTrack(const std::string& name, int loops=0);

        /**
         * Pause all audio channels.
         */
        static void Pause();

        /**
         * Resume all audio channels.
         */
        static void Resume();
    };
}

