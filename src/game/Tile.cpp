#include "Tile.hpp"
#include "Sprite.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"
#include "json.hpp"

using json = nlohmann::json;

namespace dryout {

// TileFlyweight

TileFlyweight *TileFlyweight::instance = nullptr;

TileFlyweight::TileFlyweight() {
    // TODO
    ResourceManager *resource_manager = ResourceManager::getInstance();

    flyweight[static_cast<unsigned int>(TileType::UNKNOWN)] = nullptr;
    flyweight[static_cast<unsigned int>(TileType::SAND)] = resource_manager->getSprite(
        TextureType::TILE, "sand", glm::vec2(g_tile_size), RenderType::NORMAL);
    flyweight[static_cast<unsigned int>(TileType::WET_SAND)] = resource_manager->getSprite(
        TextureType::TILE, "wet_sand", glm::vec2(g_tile_size), RenderType::NORMAL);
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

// Tile

Tile::Tile() : type(TileType::UNKNOWN), attached_entity(nullptr) {}

Tile::Tile(TileType type) : type(type), attached_entity(nullptr) {}

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
    if (attached_entity != nullptr) {
        attached_entity->render(position);
    }
}

void Tile::attachEntity(Entity *entity) {
    attached_entity = entity;
}

void Tile::detachEntity() {
    attached_entity = nullptr;
}

Entity *Tile::getAttachedEntity() const {
    return attached_entity;
}

} // namespace dryout
