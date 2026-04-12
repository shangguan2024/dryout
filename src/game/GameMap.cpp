#include "GameMap.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "Tile.hpp"
#include "CameraManager.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace dryout {

GameMap::GameMap(int width, int height)
    : width(width), height(height), map_center(glm::vec2(width, height) * g_tile_size / 2.0f),
      tiles(height, std::vector<Tile>(width, Tile(TileType::SAND))) {}

GameMap::~GameMap() {}

void GameMap::render(const glm::vec2 &center) const {
    // test
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tiles[x][y].render(tileWorldPos(x, y));
        }
    }
}

void GameMap::test(const glm::vec2 &world_pos) {
    glm::ivec2 tile_index = tileIndex(world_pos);
    tiles[tile_index.x][tile_index.y].setType(TileType::WET_SAND);
}

glm::vec2 GameMap::tileWorldPos(int x, int y) const {
    return glm::vec2(x * g_tile_size, y * g_tile_size) - map_center;
}

glm::ivec2 GameMap::tileIndex(const glm::vec2 &world_pos) const {
    glm::ivec2 index(glm::round((world_pos.x + map_center.x) / g_tile_size),
                     glm::round((world_pos.y + map_center.y) / g_tile_size));
    index = glm::clamp(index, glm::ivec2(0), glm::ivec2(width - 1, height - 1));
    return index;
}

glm::ivec2 GameMap::locateTile(const Camera *camera, const glm::vec2 &screen_pos) const {
    glm::mat2x3 ray = camera->getRay(screen_pos);
    auto &near_point = ray[0];
    auto &far_point = ray[1];
    float t = -near_point.z / (far_point.z - near_point.z);
    glm::vec2 world_pos = near_point + t * (far_point - near_point);
    return tileIndex(world_pos);
}

} // namespace dryout
