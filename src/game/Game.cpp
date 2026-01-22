#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Graphics.hpp"

namespace dryout {

Game *Game::getInstance() {
    if (instance == nullptr) {
        instance = new Game();
    }
    return instance;
}

void Game::run() {}

Game::Game() {
    init();
}

Game::~Game() {}

void Game::init() {
    Graphics *graphics = Graphics::getInstance();
    ResourceManager *resourceManager = ResourceManager::getInstance();
}

} // namespace dryout
