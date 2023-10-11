#include "AudioHandler.h"
#include "Utilities.h"

namespace MiniFPS
{
    Effect::Effect() : chunk(nullptr)
    {}

    Effect::Effect(Mix_Chunk* chunk) : chunk(chunk)
    {}

    Track::Track() : music(nullptr)
    {}

    Track::Track(Mix_Music* music) : music(music)
    {}

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
                    const string effectFilePath = audioFolderPath + folder + "/" + file;
                    Mix_Chunk* effect = Mix_LoadWAV((effectFilePath).c_str());
                    if (effect)
                    {
                        const string name = file.substr(0, file.size() - 4);
                        m_effects[name] = Effect(effect);
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
                    const string trackFilePath = audioFolderPath + folder + "/" + file;
                    Mix_Music* track = Mix_LoadMUS((trackFilePath).c_str());
                    if (track)
                    {
                        const string name = file.substr(0, file.size() - 4);
                        m_tracks[name] = Track(track);
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
        for (auto iter = m_effects.begin(); iter != m_effects.end(); iter++)
        {
            Mix_FreeChunk(iter->second.chunk);
        }
    }

    void AudioHandler::FreeTracks()
    {
        for (auto iter = m_tracks.begin(); iter != m_tracks.end(); iter++)
        {
            Mix_FreeMusic(iter->second.music);
        }
    }

    AudioHandler* AudioHandler::s_instance = nullptr;
    bool AudioHandler::s_initialized = false;
}