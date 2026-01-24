#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "Graphics.hpp"
#include "json.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

static const json empty_json = json::object();

namespace dryout {

ResourceManager *ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {
    std::cout << "Initializing resource manager..." << std::endl;

    std::cout << "Loading textures..." << std::endl;
    loadTexture("../res/textures/atlas/ui_atlas", ui_atlas, ui_atlas_texture_id);
    loadTexture("../res/textures/atlas/tilesets_atlas", tileset_atlas, tileset_atlas_texture_id);
    loadTexture("../res/textures/atlas/sprites_atlas", sprite_atlas, sprite_atlas_texture_id);
    std::cout << "Textures loaded." << std::endl;

    std::cout << "Loading shaders..." << std::endl;
    loadShader("../res/shaders/frame_shader", frame_vertex_shader, frame_fragment_shader,
               frame_shader_id);
    std::cout << "Shaders loaded." << std::endl;

    Renderer::init();

    std::cout << "Resource manager initialized." << std::endl;
}

ResourceManager::~ResourceManager() {}

ResourceManager *ResourceManager::getInstance() {
    if (instance == nullptr) {
        instance = new ResourceManager();
    }
    return instance;
}

void ResourceManager::loadTexture(const std::string &path, json &j, GLuint &texture_id) {
    std::cout << "Loading texture " << path << "..." << std::endl;

    Graphics *graphics = Graphics::getInstance();
    SDL_Surface *surface = IMG_Load((path + ".png").c_str());
    if (!surface) {
        std::cerr << "Failed to load texture atlas! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    texture_id = graphics->loadTextureAtlas(surface);
    SDL_FreeSurface(surface);

    std::ifstream json_file(path + ".json");
    if (!json_file.is_open()) {
        std::cerr << "Error: Failed to open file " << path << ".json" << std::endl;
        return;
    }
    json_file >> j;
    json_file.close();

    std::cout << "Texture " << path << " loaded." << std::endl;
}

void ResourceManager::loadShader(const std::string &path, std::string &vert, std::string &frag,
                                 GLuint &shader_id) {
    std::cout << "Loading shader " << path << "..." << std::endl;

    auto parseShaderSource = [](const std::string &path, std::string &source) -> void {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file " << path << std::endl;
            return;
        }
        source =
            std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    };

    parseShaderSource(path + ".vert", vert);
    parseShaderSource(path + ".frag", frag);

    Graphics *graphics = Graphics::getInstance();
    shader_id = graphics->createShaderProgram(vert, frag);

    std::cout << "Shader " << path << " loaded." << std::endl;
}

GLuint ResourceManager::getTextureId(TextureType type) {
    switch (type) {
    case TextureType::UI_ATLAS:
        return ui_atlas_texture_id;
    case TextureType::TILESET_ATLAS:
        return tileset_atlas_texture_id;
    case TextureType::SPRITE_ATLAS:
        return sprite_atlas_texture_id;
    default:
        std::cerr << "Error: Invalid texture type." << std::endl;
        return 0;
    }
}

const json &ResourceManager::getTextureFrameInfo(TextureType type,
                                                 const std::string &texture_name) const {
    switch (type) {
    case TextureType::UI_ATLAS:
        return ui_atlas["frames"][texture_name + ".png"];
    case TextureType::TILESET_ATLAS:
        return tileset_atlas["frames"][texture_name + ".png"];
    case TextureType::SPRITE_ATLAS:
        return sprite_atlas["frames"][texture_name + ".png"];
    default:
        std::cerr << "Error: Invalid texture type." << std::endl;
        return empty_json;
    }
}

} // namespace dryout
