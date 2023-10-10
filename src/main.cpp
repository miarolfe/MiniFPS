#define SDL_MAIN_HANDLED

#include "Game.h"

using namespace MiniFPS;

int main()
{
    Game game;
    while (game.IsRunning())
    {
        game.Update();
    }

    return 0;
}