#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "Graphics.hpp"
#include "Sprite.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

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

    glm::mat4 view_matrix = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    std::shared_ptr<Texture> texture = resource_manager->getTexture(TextureType::SPRITE_ATLAS);
    Sprite sprite(texture, glm::vec2(0.0f), glm::vec2(0.25f), glm::vec2(10.0f));

    bool running = true;
    SDL_Event event;

    float delta = 1.0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT &&
                       event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int width, height;
                SDL_GL_GetDrawableSize(graphics->getWindow(), &width, &height);
                glViewport(0, 0, width, height);

                float aspect = (float)width / (float)height;
                projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 100.0f * delta),
                                  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // std::cout << glm::to_string(view_matrix) << std::endl;
        glm::mat4 view_projection_matrix = projection_matrix * view_matrix;

        Renderer::beginScene(view_projection_matrix);
        sprite.render(glm::vec2(0.25f));
        Renderer::drawQuad(glm::vec2(0.0f), glm::vec2(-10.0f), glm::vec4(1.0f), texture,
                           glm::vec2(0.0f), glm::vec2(1.0f));
        Renderer::endScene();

        // delta *= 0.999;

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
