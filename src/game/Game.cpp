#include "Game.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "Graphics.hpp"
#include "Sprite.hpp"
#include "GameMap.hpp"
#include "Player.hpp"
#include "CameraManager.hpp"

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
    CameraManager *camera_manager = CameraManager::getInstance();

    int frame_rate = 60;
    float velocity = 100.0f;
    float delta = 1.0f / frame_rate;
    glm::vec2 position = glm::vec2(0.0f);
    Camera *camera = new Camera();
    camera_manager->registerCamera(camera, true);
    Player player;
    g_map = new GameMap(63, 63);

    auto fillRenderContext = [&]() {
        auto &context = Renderer::context;
        context.polar_angle = camera->getPolarAngle();
        context.view_projection_matrix = camera->getViewProjectionMatrix();
    };

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
                camera->setAspect(aspect);
            } else if (input_manager->handleEvent(event)) {
                continue;
            } else {
                // std::cout << "Unknown event type: " << event.type << std::endl;
            }
        }

        glm::ivec2 screen_pos = input_manager->getMousePosition();
        glm::mat2x3 ray = camera->getRay(screen_pos);
        auto &near_point = ray[0];
        auto &far_point = ray[1];
        float t = -near_point.z / (far_point.z - near_point.z);
        glm::vec2 world_pos = near_point + t * (far_point - near_point);
        g_map->test(world_pos, 2);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->setEye(glm::vec3(position + glm::vec2(0.0f, -100.0f * coef), 100.0f * coef));
        camera->setCenter(glm::vec3(position, 0.0f));

        fillRenderContext();
        Renderer::beginScene();
        player.render();
        g_map->render(position);
        Renderer::endScene();

        if (input_manager->isKeyDown(KeyCode::ESC)) {
            running = false;
        }
        if (input_manager->isKeyDown(KeyCode::Z)) {
            coef *= 0.95f;
        }
        if (input_manager->isKeyDown(KeyCode::X)) {
            coef /= 0.95f;
        }
        coef = std::clamp(coef, g_near / 100.0f * 32.0f, g_far / 100.0f / 4.0f);
        player.update(delta);
        position = player.getPosition();

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
