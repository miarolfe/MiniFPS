#include <iostream>

#include "Audio.h"
#include "Utilities.h"

namespace MiniFPS {
    Audio::Audio(const std::string& audioFolderPath) {
        std::vector<std::string> folders = GetFoldersInDirectory(audioFolderPath);

        for (const std::string& folder : folders) {
            if (folder == "effects") {
                std::vector<std::string> const effectFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const std::string& file : effectFiles) {
                    Mix_Chunk* effect = Mix_LoadWAV((audioFolderPath + folder + "/" + file).c_str());
                    if (effect) {
                        effects.emplace_back(effect);
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
                        tracks.emplace_back(track);
                    } else {
                        std::cerr << "Track file " << file << " could not be loaded" << std::endl;
                    }
                }
            }
        }
    }
}