#include "Sprite.hpp"
#include "Renderer.hpp"

namespace dryout {

Sprite::Sprite(std::shared_ptr<Texture> texture, const glm::vec2 &tex_coord,
               const glm::vec2 &tex_size, const glm::vec2 &size, RenderType render_type)
    : texture(texture), tex_coord(tex_coord), tex_size(tex_size), size(size),
      render_type(render_type) {}

Sprite::~Sprite() {}

void Sprite::render(const glm::vec2 &position, float scale, const glm::vec4 &color) {
    Renderer::drawQuad({position - size * scale / 2.0f, 0.0f}, size * scale, render_type, color,
                       texture, tex_coord, tex_size);
}

} // namespace dryout
