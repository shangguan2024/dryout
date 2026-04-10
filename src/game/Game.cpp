#include "Game.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "Graphics.hpp"
#include "Sprite.hpp"
#include "GameMap.hpp"

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
    InputManager *input_manager = InputManager::getInstance();

    int frame_rate = 60;
    float velocity = 100.0f;
    float delta = 1.0f / frame_rate;
    glm::vec2 position = glm::vec2(0.0f);
    glm::mat4 view_matrix = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
    std::shared_ptr<Texture> texture = resource_manager->getTexture(TextureType::SPRITE_ATLAS);
    auto sprite =
        resource_manager->getSprite(TextureType::SPRITE_ATLAS, "player", glm::vec2(10.0f));
    GameMap game_map(63, 63);

    bool running = true;
    SDL_Event event;

    float coef = 1.0;
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
                projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
            } else if (input_manager->handleEvent(event)) {
                continue;
            } else {
                std::cout << "Unknown event type: " << event.type << std::endl;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_matrix = glm::lookAt(glm::vec3(position, 100.0f * coef), glm::vec3(position, 0.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view_projection_matrix = projection_matrix * view_matrix;

        Renderer::beginScene(view_projection_matrix);
        sprite->render(position, 0.1f);
        game_map.render(position);
        Renderer::endScene();

        if (input_manager->isKeyDown(KeyCode::W)) {
            coef *= 0.95f;
            std::cout << "W key pressed, coef: " << coef << std::endl;
        }
        if (input_manager->isKeyDown(KeyCode::S)) {
            coef /= 0.95f;
            std::cout << "S key pressed, coef: " << coef << std::endl;
        }
        coef = std::clamp(coef, 0.1f, 10.0f);
        if (input_manager->isKeyDown(KeyCode::LEFT)) {
            position.x -= velocity * delta;
        }
        if (input_manager->isKeyDown(KeyCode::RIGHT)) {
            position.x += velocity * delta;
        }
        if (input_manager->isKeyDown(KeyCode::UP)) {
            position.y += velocity * delta;
        }
        if (input_manager->isKeyDown(KeyCode::DOWN)) {
            position.y -= velocity * delta;
        }
        if (input_manager->isKeyDown(KeyCode::ESC)) {
            running = false;
        }

        graphics->swapWindow();

        SDL_Delay(int(1000.0f / frame_rate));
        delta = 1.0f / frame_rate;
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
