#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "json.hpp"

#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>

using json = nlohmann::json;

static const json s_empty_json = json::object();

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
                                  const std::string &texture_name, json &j,
                                  std::shared_ptr<Texture> &texture) {
    std::cout << "Loading texture " << (path / (texture_name + ".png")).string() << "..."
              << std::endl;

    SDL_Surface *surface = IMG_Load((path / (texture_name + ".png")).string().c_str());
    if (!surface) {
        std::cerr << "Failed to load texture atlas! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    texture = std::make_shared<Texture>(surface);
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
                                 std::string &vert, std::string &frag,
                                 std::shared_ptr<Shader> &shader) {
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

    shader = std::make_shared<Shader>(vert, frag);

    std::cout << "Shader " << shader_name << " loaded." << std::endl;
}

const json &ResourceManager::getTextureFrameInfo(TextureType type,
                                                 const std::string &texture_name) const {
    switch (type) {
    case TextureType::UI_ATLAS:
        return ui_atlas["frames"][texture_name];
    case TextureType::TILESET_ATLAS:
        return tileset_atlas["frames"][texture_name];
    case TextureType::SPRITE_ATLAS:
        return sprite_atlas["frames"][texture_name];
    default:
        std::cerr << "Error: Invalid texture type." << std::endl;
        return s_empty_json;
    }
}

std::shared_ptr<Shader> ResourceManager::getShader(ShaderType type) const {
    switch (type) {
    case ShaderType::BASIC:
        return frame_shader;
    default:
        std::cerr << "Error: Invalid shader type." << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Sprite> ResourceManager::getSprite(TextureType type,
                                                   const std::string &texture_name,
                                                   const glm::vec2 &size) const {
    auto getAtlasSize = [&](TextureType type) -> glm::vec2 const {
        glm::vec2 atlas_size{1.0f, 1.0f};
        switch (type) {
        case TextureType::UI_ATLAS:
            atlas_size.x = ui_atlas["meta"]["size"]["w"];
            atlas_size.y = ui_atlas["meta"]["size"]["h"];
            break;
        case TextureType::TILESET_ATLAS:
            atlas_size.x = tileset_atlas["meta"]["size"]["w"];
            atlas_size.y = tileset_atlas["meta"]["size"]["h"];
            break;
        case TextureType::SPRITE_ATLAS:
            atlas_size.x = sprite_atlas["meta"]["size"]["w"];
            atlas_size.y = sprite_atlas["meta"]["size"]["h"];
            break;
        default:
            std::cerr << "Error: Invalid texture type." << std::endl;
            break;
        }
        return atlas_size;
    };
    static const std::unordered_map<TextureType, glm::vec2> s_atlas_sizes = {
        {TextureType::UI_ATLAS, getAtlasSize(TextureType::UI_ATLAS)},
        {TextureType::TILESET_ATLAS, getAtlasSize(TextureType::TILESET_ATLAS)},
        {TextureType::SPRITE_ATLAS, getAtlasSize(TextureType::SPRITE_ATLAS)},
    };

    const json &info = getTextureFrameInfo(type, texture_name);
    const glm::vec2 &atlas_size = s_atlas_sizes.at(type); // Safe access
    glm::vec2 tex_coord(info["frame"]["x"], info["frame"]["y"]);
    glm::vec2 tex_size(info["frame"]["w"], info["frame"]["h"]);
    tex_coord /= atlas_size;
    tex_size /= atlas_size;

    return std::make_unique<Sprite>(Sprite(getTexture(type), tex_coord, tex_size, size));
}

std::shared_ptr<Texture> ResourceManager::getTexture(TextureType type) const {
    switch (type) {
    case TextureType::UI_ATLAS:
        return ui_atlas_texture;
    case TextureType::TILESET_ATLAS:
        return tileset_atlas_texture;
    case TextureType::SPRITE_ATLAS:
        return sprite_atlas_texture;
    default:
        std::cerr << "Error: Invalid texture type." << std::endl;
        return nullptr;
    }
}

} // namespace dryout
