#include "Sprite.hpp"
#include "Renderer.hpp"

namespace dryout {

Sprite::Sprite(std::shared_ptr<Texture> texture, const glm::vec2 &tex_coord,
               const glm::vec2 &tex_size, const glm::vec2 &size)
    : texture(texture), tex_coord(tex_coord), tex_size(tex_size), size(size) {}

Sprite::~Sprite() {}

void Sprite::render(const glm::vec2 &position, float z, float scale, const glm::vec4 &color,
                    float rotation) {
    Renderer::drawQuad({position - size * scale / 2.0f, z}, size * scale, color, texture, tex_coord,
                       tex_size);
}

} // namespace dryout
