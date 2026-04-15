#include "Graphics.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace dryout {

Graphics *Graphics::instance = nullptr;

Graphics *Graphics::getInstance() {
    if (!instance) {
        instance = new Graphics();
    }
    return instance;
}

SDL_Window *Graphics::getWindow() {
    return window;
}

void Graphics::swapWindow() {
    SDL_GL_SwapWindow(window);
}

glm::ivec2 Graphics::getWindowSize() {
    int width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return glm::ivec2(width, height);
}

Graphics::Graphics() : window(nullptr), context(nullptr) {
    init();
}

Graphics::~Graphics() {
    if (context) {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void Graphics::init() {
    std::cout << "Initializing graphics..." << std::endl;

    std::cout << "Initializing SDL..." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not be initialized! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    std::cout << "SDL initialized." << std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    std::cout << "Initializing window..." << std::endl;
    window = SDL_CreateWindow("Dryout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
                                  SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    std::cout << "Window created." << std::endl;

    std::cout << "Initializing OpenGL Context..." << std::endl;
    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError()
                  << std::endl;
        return;
    }
    std::cout << "OpenGL context created." << std::endl;

    std::cout << "Initializing glad..." << std::endl;
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize glad!" << std::endl;
        return;
    }
    std::cout << "glad initialized." << std::endl;

    std::cout << "Initializing SDL_image..." << std::endl;
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image could not be initialized! SDL_Error: " << IMG_GetError()
                  << std::endl;
        return;
    }
    std::cout << "SDL_image initialized." << std::endl;

    std::cout << "Graphics initialized." << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

} // namespace dryout
