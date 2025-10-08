#include "Game.hpp"

int main(void)
{
    Game game;
    game.Initialize();
    game.Run();
    game.Uninitialize();

    return 0;
}
