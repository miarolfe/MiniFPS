#include "Audio.h"
#include "Utilities.h"

namespace MiniFPS {
    Effect::Effect() : chunk(nullptr) {}

    Effect::Effect(Mix_Chunk* chunk) : chunk(chunk) {}

    Track::Track() : music(nullptr) {}

    Track::Track(Mix_Music* music) : music(music) {}

    Audio::Audio() = default;

    Audio::Audio(const string& audioFolderPath, const Settings& settings) {
        SetEffectVolume(settings.effectVolume);
        SetMusicVolume(settings.musicVolume);

        const std::vector<std::string> folders = GetFoldersInDirectory(audioFolderPath);

        for (const std::string& folder : folders) {
            if (folder == "effects") {
                std::vector<string> const effectFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const string& file : effectFiles) {
                    const string effectFilePath = audioFolderPath + folder + "/" + file;
                    Mix_Chunk* effect = Mix_LoadWAV((effectFilePath).c_str());
                    if (effect) {
                        const std::string name = file.substr(0, file.size()-4);
                        effects[name] = Effect(effect);
                    } else {
                        std::cerr << "Effect file " << file << " could not be loaded" << std::endl;
                    }
                }
            } else if (folder == "tracks") {
                std::vector<std::string> const trackFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const std::string& file : trackFiles) {
                    const std::string trackFilePath = audioFolderPath + folder + "/" + file;
                    Mix_Music* track = Mix_LoadMUS((trackFilePath).c_str());
                    if (track) {
                        const std::string name = file.substr(0, file.size()-4);
                        tracks[name] = Track(track);
                    } else {
                        if (file != ".DS_Store") { // macOS-specific hidden file
                            std::cerr << "Track file " << file << " could not be loaded" << std::endl;
                        }
                    }
                }
            } else {
                std::cerr << "Folder " << folder << " is not a valid folder for audio files" << std::endl;
            }
        }
    }

    bool Audio::PlayEffect(const std::string& name, int loops) {
        if (effects.count(name) > 0) {
            const int rc = Mix_PlayChannel(-1, effects[name].chunk, loops);
            if (rc != -1) {
                return true;
            }
        }

        return false;
    }

    bool Audio::PlayTrack(const std::string& name, int loops) {
        if (tracks.count(name) > 0) {
            const int rc = Mix_PlayMusic(tracks[name].music, loops);
            if (rc != -1) {
                return true;
            }
        }

        return false;
    }

    void Audio::SetEffectVolume(float volume) {
        if (volume < 0) volume = 0;
        if (volume > 1) volume = 1;

        this->effectVolume = volume;

        Mix_MasterVolume(static_cast<int>(static_cast<float>(SDL_MIX_MAXVOLUME) * this->effectVolume));
    }

    void Audio::SetMusicVolume(float volume) {
        if (volume < 0) volume = 0;
        if (volume > 1) volume = 1;

        this->musicVolume = volume;

        Mix_VolumeMusic(static_cast<int>(static_cast<float>(SDL_MIX_MAXVOLUME) * this->musicVolume));
    }

    void Audio::Pause() {
        for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
            if (Mix_Paused(i) == 0) {
                Mix_Pause(i);
            }
        }
    }

    void Audio::Resume() {
        for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
            if (Mix_Paused(i) == 0) {
                Mix_Resume(i);
            }
        }
    }

    void Audio::FreeEffects() {
        for (auto iter = effects.begin(); iter != effects.end(); iter++) {
            Mix_FreeChunk(iter->second.chunk);
        }
    }

    void Audio::FreeTracks() {
        for (auto iter = tracks.begin(); iter != tracks.end(); iter++) {
            Mix_FreeMusic(iter->second.music);
        }
    }
}