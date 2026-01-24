#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "json.hpp"

#include <glad/glad.h>
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

    GLuint getTextureId(TextureType type);
    const json &getTextureFrameInfo(TextureType type, const std::string &texture_name) const;

  private:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    void loadTexture(const std::string &path, json &j, GLuint &texture_id);
    void loadShader(const std::string &path, std::string &vert, std::string &frag,
                    GLuint &shader_id);

    static ResourceManager *instance;
    json ui_atlas, tileset_atlas, sprite_atlas;
    GLuint ui_atlas_texture_id, tileset_atlas_texture_id, sprite_atlas_texture_id;
    std::string frame_vertex_shader, frame_fragment_shader;
    GLuint frame_shader_id;
};

} // namespace dryout

#endif // RESOURCE_MANAGER_HPP
