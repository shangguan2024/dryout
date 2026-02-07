#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Texture.hpp"

#include <glm/glm.hpp>

namespace dryout {

class Sprite {
  public:
    Sprite(std::shared_ptr<Texture> texture, const glm::vec2 &tex_coord, const glm::vec2 &tex_size,
           const glm::vec2 &size);
    ~Sprite();

    void render(const glm::vec2 &position, const glm::vec4 &color = glm::vec4(1.0f),
                const glm::vec2 &scale = glm::vec2(1.0f), float rotation = 0.0f);

  private:
    std::shared_ptr<Texture> texture;
    glm::vec2 tex_coord, tex_size, size;
};

} // namespace dryout

#endif // SPRITE_HPP
