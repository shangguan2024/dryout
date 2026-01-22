#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Graphics.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace dryout {

Game *Game::instance = nullptr;

Game *Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game();
    }
    return instance;
}

void Game::run() {
    // test
    Graphics *graphics = Graphics::getInstance();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        graphics->swapWindow();
    }
}

Game::Game() {
    init();
}

Game::~Game() {}

void Game::init() {
    Graphics *graphics = Graphics::getInstance();
    ResourceManager *resource_manager = ResourceManager::getInstance();
}

} // namespace dryout
