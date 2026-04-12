#include "Entity.hpp"
#include "ResourceManager.hpp"

#include <glm/glm.hpp>

namespace dryout {

Entity::Entity(const std::string &name, const glm::vec2 &size) : size(size) {
    sprite = ResourceManager::getInstance()->getSprite(TextureType::SPRITE, name, size);
}

Entity::~Entity() {}

void Entity::render(const glm::vec2 &position) const {
    sprite->render(position + glm::vec2(0.0f, size.y / 2.0f), 0.1f);
}

} // namespace dryout
