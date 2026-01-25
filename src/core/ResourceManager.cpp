#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "json.hpp"

#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

using json = nlohmann::json;

static const json empty_json = json::object();

namespace dryout {

ResourceManager *ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {
    std::cout << "Initializing resource manager..." << std::endl;

    std::filesystem::path root = std::filesystem::current_path().parent_path() / "res";

    std::cout << "Loading textures..." << std::endl;
    loadTexture(root / "textures" / "atlas", "ui_atlas", ui_atlas, ui_atlas_texture);
    loadTexture(root / "textures" / "atlas", "tilesets_atlas", tileset_atlas,
                tileset_atlas_texture);
    loadTexture(root / "textures" / "atlas", "sprites_atlas", sprite_atlas, sprite_atlas_texture);
    std::cout << "Textures loaded." << std::endl;

    std::cout << "Loading shaders..." << std::endl;
    loadShader(root / "shaders", "frame_shader", frame_vertex_shader, frame_fragment_shader,
               frame_shader);
    std::cout << "Shaders loaded." << std::endl;

    std::cout << "Resource manager initialized." << std::endl;
}

ResourceManager::~ResourceManager() {
    // TODO: Free all resources
}

ResourceManager *ResourceManager::getInstance() {
    if (instance == nullptr) {
        instance = new ResourceManager();
    }
    return instance;
}

void ResourceManager::loadTexture(const std::filesystem::path &path,
                                  const std::string &texture_name, json &j, Texture &texture) {
    std::cout << "Loading texture " << (path / (texture_name + ".png")).string() << "..."
              << std::endl;

    SDL_Surface *surface = IMG_Load((path / (texture_name + ".png")).string().c_str());
    if (!surface) {
        std::cerr << "Failed to load texture atlas! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    texture = Texture(surface);
    SDL_FreeSurface(surface);

    std::ifstream json_file(path / (texture_name + ".json"));
    if (!json_file.is_open()) {
        std::cerr << "Error: Failed to open file " << (path / (texture_name + ".json")).string()
                  << std::endl;
        return;
    }
    json_file >> j;
    json_file.close();

    std::cout << "Texture " << (path / (texture_name + ".png")).string() << " loaded." << std::endl;
}

void ResourceManager::loadShader(const std::filesystem::path &path, const std::string &shader_name,
                                 std::string &vert, std::string &frag, Shader &shader) {
    std::cout << "Loading shader " << shader_name << "..." << std::endl;

    auto parseShaderSource = [](const std::filesystem::path &path, std::string &source) -> void {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file " << path.string() << std::endl;
            return;
        }
        source =
            std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    };

    parseShaderSource(path / (shader_name + ".vert"), vert);
    parseShaderSource(path / (shader_name + ".frag"), frag);

    shader = std::move(Shader(vert, frag));

    std::cout << "Shader " << shader_name << " loaded." << std::endl;
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
