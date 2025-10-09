#include "Game.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game game;

void UpdateDrawFrame(void)
{
    game.DoFrame();
}

int main(void)
{
    game.Initialize();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    //SetTargetFPS(60);
    SetExitKey(KEY_Q);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif
    game.Uninitialize();

    return 0;
}
