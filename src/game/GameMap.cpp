#include "GameMap.hpp"
#include "Tile.hpp"

namespace dryout {

static constexpr float tile_size = 10.0f;

GameMap::GameMap(int width, int height)
    : width(width), height(height), map_center(glm::vec2(width, height) * tile_size / 2.0f) {
    tiles.assign(height, std::vector<Tile>(width, Tile(TileType::SAND)));
}

GameMap::~GameMap() {}

void GameMap::render(glm::vec2 center) const {
    // test
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            tiles[y][x].render(glm::vec2(x * tile_size, y * tile_size) - map_center);
        }
    }
}

} // namespace dryout
