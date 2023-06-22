#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Audio.h"
#include "Renderer.h"
#include "Settings.h"

namespace MiniFPS {
    const float PI_180 = (1.0 / 180.0) * M_PI;

    /**
     * Initializes all the SDL subsystems (SDL, SDL_image, SDL_mixer, SDL_ttf).
     * @return Whether all the SDL subsystems were initialized properly.
     */
    bool InitializeSDLSubsystems();

    /**
     * Deactivate the SDL subsystems (SDL, SDL_image, SDL_mixer, SDL_ttf).
     */
    void DeactivateSDLSubsystems();

    /**
     * Free all memory allocated for game content.
     * @param renderer The game renderer.
     * @param audio The audio manager.
     * @param fontManager The font manager.
     */
    void FreeResources(Renderer renderer, Audio audio, FontManager fontManager);

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
    bool InitializeWindowAndRenderer(SDL_Window** window, SDL_Renderer** renderer, int screenWidth,
                                     int screenHeight, bool vSync);

    /**
     * Initialize SDL_Image's PNG subsystem.
     * @return Whether the initialization was successful.
     */
    bool InitializeSDLImage();

    /**
     * Initialize SDL_mixer's subsystems.
     * @return Whether the initialization was successful.
     */
    bool InitializeSDLMixer();

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
     * @param The time elapsed between frames in seconds.
     * @return The current FPS as a string.
     */
    std::string GetFramesPerSecond(float frameDelta);

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
     * Clear the contents of a file.
     * @param fileName The path to the file.
     */
    void ClearFile(const std::string& fileName);

    /**
     * Write a string to a file on a new line.
     * @param fileName The path to the file.
     * @param line The string to write to the file.
     */
    void WriteLineToFile(const std::string& fileName, const std::string& line);

    /**
     * Get the paths to all files in a directory.
     * @param directoryPath The path to the directory
     * @return A list of paths to all files in the directory.
     */
    std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath);

    /**
     * Gets the paths to all folders in a directory.
     * @param directoryPath The path to the directory
     * @return A list of paths to all folders in the directory.
     */
    std::vector<std::string> GetFoldersInDirectory(const std::string& directoryPath);

    /**
     * Calculates the cross product of two vectors.
     * @param x1 x component of first vector.
     * @param y1 y component of first vector.
     * @param x2 x component of second vector.
     * @param y2 y component of second vector.
     * @return The cross product of the first and second vectors.
     */
    float CalculateCrossProduct(float x1, float y1, float x2, float y2);

    /**
     * Checks if a point is within a right-angled triangle.
     * @param pointX x coordinate of the point.
     * @param pointY y coordinate of the point.
     * @param x1 x coordinate of first vertex.
     * @param y1 y coordinate of first vertex.
     * @param x2 x coordinate of second vertex.
     * @param y2 y coordinate of second vertex.
     * @param x3 x coordinate of third vertex.
     * @param y3 y coordinate of third vertex.
     * @return Whether the point is inside the right-angled triangle.
     */
    bool IsPointInRightAngledTriangle(float pointX, float pointY, float x1, float y1, float x2, float y2, float x3, float y3);
}