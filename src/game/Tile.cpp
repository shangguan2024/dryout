#include "Tile.hpp"
#include "Sprite.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "json.hpp"

using json = nlohmann::json;

namespace dryout {

TileFlyweight *TileFlyweight::instance = nullptr;

TileFlyweight::TileFlyweight() {
    // TODO
    ResourceManager *resource_manager = ResourceManager::getInstance();

    flyweight[static_cast<unsigned int>(TileType::UNKNOWN)] = nullptr;
    flyweight[static_cast<unsigned int>(TileType::SAND)] =
        resource_manager->getSprite(TextureType::TILESET_ATLAS, "sand", glm::vec2(10.0f));
}

TileFlyweight *TileFlyweight::getInstance() {
    if (instance == nullptr) {
        instance = new TileFlyweight();
    }
    return instance;
}

void TileFlyweight::render(TileType type, const glm::vec2 &position) const {
    if (type == TileType::UNKNOWN) {
        return;
    }
    if (flyweight[static_cast<unsigned int>(type)] == nullptr) {
        return;
    }
    flyweight[static_cast<unsigned int>(type)]->render(position);
}

Tile::Tile() : type(TileType::UNKNOWN) {}

Tile::Tile(TileType type) : type(type) {}

Tile::~Tile() {
    // TODO
}

TileType Tile::getType() const {
    return type;
}

void Tile::setType(TileType type) {
    this->type = type;
}

void Tile::render(const glm::vec2 &position) const {
    if (type == TileType::UNKNOWN) {
        return;
    }
    TileFlyweight::getInstance()->render(type, position);
}

} // namespace dryout
