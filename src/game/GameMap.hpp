#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "Tile.hpp"
#include "CameraManager.hpp"

#include <vector>

namespace dryout {

class GameMap;
extern GameMap *g_map; // test

class GameMap {
  public:
    GameMap(int width, int height);
    ~GameMap();

    void render(const glm::vec2 &center) const;

    void test(const glm::vec2 &world_pos); // To be removed

  private:
    glm::vec2 tileWorldPos(int x, int y) const;
    glm::ivec2 tileIndex(const glm::vec2 &world_pos) const;
    glm::ivec2 locateTile(const Camera *camera, const glm::vec2 &screen_pos) const;

    int width, height;
    glm::vec2 map_center;
    std::vector<std::vector<Tile>> tiles;
};

} // namespace dryout

#endif // GAME_MAP_HPP
