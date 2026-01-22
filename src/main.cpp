#include <SDL2/SDL.h>
#include "Game.hpp"

int main(int argc, char *argv[]) {
    dryout::Game *game = dryout::Game::getInstance();
    game->run();
    return 0;
}
