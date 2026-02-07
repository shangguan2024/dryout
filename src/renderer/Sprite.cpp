#include "Sprite.hpp"
#include "Renderer.hpp"

namespace dryout {

Sprite::Sprite(std::shared_ptr<Texture> texture, const glm::vec2 &tex_coord,
               const glm::vec2 &tex_size, const glm::vec2 &size)
    : texture(texture), tex_coord(tex_coord), tex_size(tex_size), size(size) {}

Sprite::~Sprite() {}

void Sprite::render(const glm::vec2 &position, const glm::vec4 &color, const glm::vec2 &scale,
                    float rotation) {
    // todo : rotation
    Renderer::drawQuad(position, size * scale, color, texture, tex_coord, tex_size);
}

} // namespace dryout
