#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "json.hpp"

#include <string>

using json = nlohmann::json;

namespace dryout {

enum class TextureType : unsigned int {
    NONE = 0,
    UI_ATLAS,
    TILESET_ATLAS,
    SPRITE_ATLAS,
};

class ResourceManager {
  public:
    static ResourceManager *getInstance();

    unsigned int getTextureId(TextureType type);
    const json &getTextureFrame(TextureType type, const std::string &texture_name) const;

  private:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    void loadTextures(const std::string &path, json &j, unsigned int &texture_id);

    static ResourceManager *instance;
    json ui_atlas, tileset_atlas, sprite_atlas;
    unsigned int ui_atlas_texture_id, tileset_atlas_texture_id, sprite_atlas_texture_id;
};

} // namespace dryout

#endif // RESOURCE_MANAGER_HPP
