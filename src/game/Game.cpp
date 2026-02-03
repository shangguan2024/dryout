#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
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
    ResourceManager *resource_manager = ResourceManager::getInstance();

    std::shared_ptr<Texture> texture = resource_manager->getTexture(TextureType::SPRITE_ATLAS);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer::drawQuad(glm::vec2(0.0f), glm::vec2(1.0f), glm::vec4(1.0f), glm::vec2(0.0f),
                           glm::vec2(1.0f), texture, glm::vec4(1.0f));
        Renderer::drawQuad(glm::vec2(0.0f), glm::vec2(-1.0f), glm::vec4(1.0f), glm::vec2(0.0f),
                           glm::vec2(1.0f), texture, glm::vec4(1.0f));
        Renderer::endScene();

        graphics->swapWindow();

        SDL_Delay(16);
    }
}

Game::Game() {
    init();
}

Game::~Game() {
    // TODO: Clean up resources
    Renderer::shutdown();
}

void Game::init() {
    Graphics::getInstance();
    ResourceManager *resource_manager = ResourceManager::getInstance();
    Renderer::init();
}

} // namespace dryout
