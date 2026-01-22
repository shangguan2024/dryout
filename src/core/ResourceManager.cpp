#include "ResourceManager.hpp"
#include "json.hpp"

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

static const json empty_json = json::object();

namespace dryout {

ResourceManager *ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {
    std::cout << "Loading textures..." << std::endl;

    loadTextures("../res/textures/atlas/ui_atlas", ui_atlas, ui_atlas_texture_id);
    loadTextures("../res/textures/atlas/tilesets_atlas", tileset_atlas, tileset_atlas_texture_id);
    loadTextures("../res/textures/atlas/sprites_atlas", sprite_atlas, sprite_atlas_texture_id);

    std::cout << "Textures loaded." << std::endl;
}

ResourceManager::~ResourceManager() {}

ResourceManager *ResourceManager::getInstance() {
    if (instance == nullptr) {
        instance = new ResourceManager();
    }
    return instance;
}

void ResourceManager::loadTextures(const std::string &path, json &j, unsigned int &texture_id) {
    glGenTextures(1, &texture_id);

    std::ifstream json_file(path + ".json");
    if (!json_file.is_open()) {
        std::cerr << "Error: Failed to open file " << path << ".json" << std::endl;
        return;
    }
    json_file >> j;
    json_file.close();
}

unsigned int ResourceManager::getTextureId(TextureType type) {
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

const json &ResourceManager::getTextureFrame(TextureType type,
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
