#include <iostream>
#include "./Game/Game.h"

int main(int argc, char *args[]) {
    Game game;

    game.Initialize();

    while (game.IsRunning()) {
        game.ProcessInput();
        game.Update();
        game.Render();
    }

    game.Destroy();

    return 0;
}
