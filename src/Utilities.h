#pragma once

#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <dirent.h>

namespace MiniFPS {
    const float PI_180 = (1.0 / 180.0) * M_PI;

    /**
     * Initialize SDL's video subsystem.
     * @return Whether the initialization was successful.
     */
    bool InitializeSDL();

    /**
     * Create a window and an SDL_Renderer object and initialize them with the provided parameters.
     * @param window A pointer to the pointer to initialize the window with.
     * @param renderer A pointer to the pointer to initialize the renderer with.
     * @param screenWidth The starting width of the new window.
     * @param screenHeight The starting height of the new window.
     * @param vSync Whether to enable VSync.
     * @return Whether the initialization was successful.
     */
    bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, size_t screenWidth,
                                     size_t screenHeight, bool vSync);

    /**
     * Initialize SDL_Image's PNG subsystem.
     * @return Whether the initialization was successful.
     */
    bool InitializeSDLImage();

    /**
     * Initialize SDL_TTF.
     * @return Whether the initialization was successful.
     */
    bool InitializeSDLTTF();

    /**
     * Shutdown all subsystems and destroy window & renderer.
     * @param window The window.
     * @param renderer The SDL_Renderer.
     */
    void Quit(SDL_Window* window, SDL_Renderer* renderer);

    /**
     * Get the current frames per second (FPS).
     * @param oldTime The time at the start of the frame in milliseconds.
     * @param curTime The time at the end of the frame in milliseconds.
     * @return The current FPS as a string.
     */
    std::string GetFramesPerSecond(float oldTime, float curTime);

    /**
     * Get the time elapsed during the frame.
     * @param oldTime The time at the start of the frame in milliseconds.
     * @param curTime The time at the end of the frame in milliseconds.
     * @return The time elapsed during the frame in seconds.
     */
    float GetFrameTime(float oldTime, float curTime);

    /**
     * Gets the platform-appropriate path to the assets folder.
     * @return The path to the assets folder.
     */
    std::string GetSDLAssetsFolderPath();

    /**
     * Load a texture from a file into a frame buffer.
     * @param buffer The pointer to the frame buffer.
     * @param size The length of a side of the texture.
     * @param assetsFolderPath The path to the assets folder.
     * @param textureFilePath The path to the texture file within the assets folder.
     * @return Whether the texture was successfully loaded into the frame buffer.
     */
    bool LoadTextureToBuffer(uint32_t*** buffer, size_t &size, std::string assetsFolderPath, std::string textureFilePath);

    /**
     *
     * @param fontPath
     * @param pointSize
     * @return
     */
    TTF_Font* LoadFont(const std::string &fontPath, int pointSize);

    /**
     * Clear the contents of a file.
     * @param fileName The path to the file.
     */
    void ClearFile(std::string fileName);

    /**
     * Write a string to a file on a new line.
     * @param fileName The path to the file.
     * @param line The string to write to the file.
     */
    void WriteLineToFile(std::string fileName, std::string line);

    /**
     * Get the paths to all files in a directory.
     * @param directoryPath The path to the directory
     * @return A list of paths to all files in the directory.
     */
    std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath);
}