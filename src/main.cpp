#define SDL_MAIN_HANDLED

#include "Game.h"

using namespace MiniFPS;

int main()
{
    Game* game = new Game();

    while (game->IsRunning())
    {
        game->Update();
    }

    delete game;

    return 0;
}