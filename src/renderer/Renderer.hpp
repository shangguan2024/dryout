#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace dryout {

class Renderer {
  public:
    static void init();
    static void shutdown();

    static void setShader(const std::shared_ptr<Shader> &shader);

    static void beginScene(const glm::mat4 &view_projection_matrix);
    static void endScene();

    static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color,
                         const std::shared_ptr<Texture> &texture, const glm::vec2 &texture_coords,
                         const glm::vec2 &texture_size); // TODO

  private:
    static void flush();

    static void resetTextureSlots();
    static int getTextureSlot(GLuint texture_id);
    static int getTextureSlot(std::shared_ptr<Texture> texture);
    static int bindTextureSlot(GLuint texture_id);
    static int bindTextureSlot(std::shared_ptr<Texture> texture);
};

} // namespace dryout

#endif // RENDERER_HPP
