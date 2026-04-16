#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Texture.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "json.hpp"

#include <glad/glad.h>
#include <string>
#include <filesystem>
#include <memory>

using json = nlohmann::json;

namespace dryout {

enum class TextureType : unsigned int {
    NONE = 0,
    UI,
    TILE,
    SPRITE,
};

enum class ShaderType : unsigned int {
    NONE = 0,
    BASIC,
};

class ResourceManager {
  public:
    static ResourceManager *getInstance();

    std::shared_ptr<Texture> getTexture(TextureType type) const;
    std::shared_ptr<Shader> getShader(ShaderType type) const;

    std::unique_ptr<Sprite> getSprite(TextureType type, const std::string &texture_name,
                                      const glm::vec2 &size, RenderType render_type) const;

  private:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    void loadTexture(const std::filesystem::path &path, const std::string &texture_name, json &j,
                     std::shared_ptr<Texture> &texture);
    void loadShader(const std::filesystem::path &path, const std::string &shader_name,
                    std::string &vert, std::string &frag, std::shared_ptr<Shader> &shader);

    const json &getTextureFrameInfo(TextureType type, const std::string &texture_name) const;

    static ResourceManager *instance;
    json ui_atlas, tileset_atlas, sprite_atlas;
    std::shared_ptr<Texture> ui_atlas_texture, tileset_atlas_texture, sprite_atlas_texture;
    std::string frame_vertex_shader, frame_fragment_shader;
    std::shared_ptr<Shader> basic_shader;
};

} // namespace dryout

#endif // RESOURCE_MANAGER_HPP
