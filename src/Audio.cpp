#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Audio.h"
#include "Utilities.h"

namespace MiniFPS {
    Effect::Effect() {
        this->chunk = nullptr;
    }

    Effect::Effect(Mix_Chunk* chunk) {
        this->chunk = chunk;
    }

    Track::Track() {
        this->music = nullptr;
    }

    Track::Track(Mix_Music* music) {
        this->music = music;
    }

    Audio::Audio(const std::string& audioFolderPath) {
        std::vector<std::string> folders = GetFoldersInDirectory(audioFolderPath);

        for (const std::string& folder : folders) {
            if (folder == "effects") {
                std::vector<std::string> const effectFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const std::string& file : effectFiles) {
                    Mix_Chunk* effect = Mix_LoadWAV((audioFolderPath + folder + "/" + file).c_str());
                    if (effect) {
                        std::string name = file.substr(0, file.size()-4);
                        effects[name] = Effect(effect);
                    } else {
                        std::cerr << "Effect file " << file << " could not be loaded" << std::endl;
                    }
                }
            }

            if (folder == "tracks") {
                std::vector<std::string> const trackFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const std::string& file : trackFiles) {
                    Mix_Music* track = Mix_LoadMUS((audioFolderPath + folder + "/" + file).c_str());
                    if (track) {
                        std::string name = file.substr(0, file.size()-4);
                        tracks[name] = Track(track);
                    } else {
                        std::cerr << "Track file " << file << " could not be loaded" << std::endl;
                    }
                }
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
}