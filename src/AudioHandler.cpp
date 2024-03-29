#include "AudioHandler.h"
#include "Utilities.h"

namespace MiniFPS
{
    AudioHandler::AudioHandler() = default;

    AudioHandler::AudioHandler(const string& audioFolderPath, const Settings& settings)
    {
        SetEffectVolume(settings.effectVolume);
        SetMusicVolume(settings.musicVolume);

        const std::vector<string> folders = GetFoldersInDirectory(audioFolderPath);

        for (const string& folder: folders)
        {
            if (folder == "effects")
            {
                std::vector<string> const effectFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const string& file: effectFiles)
                {
                    string effectFilePath = audioFolderPath;
                    effectFilePath += folder;
                    effectFilePath += "/";
                    effectFilePath += file;

                    Mix_Chunk* effect = Mix_LoadWAV((effectFilePath).c_str());
                    if (effect)
                    {
                        const string name = file.substr(0, file.size() - 4);
                        m_effects[name] = {effect};
                    }
                    else
                    {
                        if (file != ".DS_Store") // macOS-specific hidden file
                        {
                            string err;
                            err += "Effect file ";
                            err += file;
                            err += " could not be loaded";
                            LogHandler::LogError(err.c_str());
                        }
                    }
                }
            }
            else if (folder == "tracks")
            {
                std::vector<string> const trackFiles = GetFilesInDirectory(audioFolderPath + folder);

                for (const string& file: trackFiles)
                {
                    string trackFilePath = audioFolderPath;
                    trackFilePath += folder;
                    trackFilePath += "/";
                    trackFilePath += file;

                    Mix_Music* track = Mix_LoadMUS((trackFilePath).c_str());
                    if (track)
                    {
                        const string name = file.substr(0, file.size() - 4);
                        m_tracks[name] = {track};
                    }
                    else
                    {
                        if (file != ".DS_Store") // macOS-specific hidden file
                        {
                            string err;
                            err += "Track file ";
                            err += file;
                            err += " could not be loaded";
                            LogHandler::LogError(err.c_str());
                        }
                    }
                }
            }
            else
            {
                string err;
                err += "Folder ";
                err += folder;
                err += " is not a valid folder for audio files";
                LogHandler::LogError(err.c_str());
            }
        }
    }

    AudioHandler& AudioHandler::GetInstance(const string& audioFolderPath, const Settings& settings)
    {
        if (s_instance == nullptr)
        {
            if (!s_initialized)
            {
                s_instance = new AudioHandler(audioFolderPath, settings);
                s_initialized = true;
            }
            else
            {
                LogHandler::LogError("Attempting to reinitialize LogHandler");
            }
        }

        return *s_instance;
    }

    bool AudioHandler::PlayEffect(const string& name, int loops)
    {
        if (m_effects.count(name) > 0)
        {
            const int rc = Mix_PlayChannel(-1, m_effects[name].chunk, loops);
            if (rc != -1)
            {
                return true;
            }
        }

        return false;
    }

    bool AudioHandler::PlayTrack(const string& name, int loops)
    {
        if (m_tracks.count(name) > 0)
        {
            const int rc = Mix_PlayMusic(m_tracks[name].music, loops);
            if (rc != -1)
            {
                return true;
            }
        }

        return false;
    }

    void AudioHandler::SetEffectVolume(float volume)
    {
        if (volume < 0)
        { volume = 0; }
        if (volume > 1)
        { volume = 1; }

        this->m_effectVolume = volume;

        Mix_MasterVolume(static_cast<int>(static_cast<float>(SDL_MIX_MAXVOLUME) * this->m_effectVolume));
    }

    void AudioHandler::SetMusicVolume(float volume)
    {
        if (volume < 0)
        { volume = 0; }
        if (volume > 1)
        { volume = 1; }

        this->m_musicVolume = volume;

        Mix_VolumeMusic(static_cast<int>(static_cast<float>(SDL_MIX_MAXVOLUME) * this->m_musicVolume));
    }

    void AudioHandler::Pause()
    {
        for (int i = 0; i < Mix_AllocateChannels(-1); i++)
        {
            if (Mix_Paused(i) == 0)
            {
                Mix_Pause(i);
            }
        }
    }

    void AudioHandler::Resume()
    {
        for (int i = 0; i < Mix_AllocateChannels(-1); i++)
        {
            if (Mix_Paused(i) == 0)
            {
                Mix_Resume(i);
            }
        }
    }

    void AudioHandler::FreeEffects()
    {
        for (auto i = m_effects.begin(); i != m_effects.end(); i++)
        {
            Mix_FreeChunk(i->second.chunk);
        }
    }

    void AudioHandler::FreeTracks()
    {
        for (auto i = m_tracks.begin(); i != m_tracks.end(); i++)
        {
            Mix_FreeMusic(i->second.music);
        }
    }

    AudioHandler* AudioHandler::s_instance = nullptr;
    bool AudioHandler::s_initialized = false;
}