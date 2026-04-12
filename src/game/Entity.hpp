#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Sprite.hpp"

#include <glm/glm.hpp>

namespace dryout {

constexpr glm::vec2 g_default_entity_size(10.0f);

class Entity {
  public:
    Entity(const std::string &name = "player", const glm::vec2 &size = g_default_entity_size);
    ~Entity();

    void render(const glm::vec2 &position) const;

  protected:
    std::unique_ptr<Sprite> sprite;
    glm::vec2 size;
};

} // namespace dryout

#endif // ENTITY_HPP
