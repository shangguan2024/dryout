#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "Tile.hpp"

#include <vector>

namespace dryout {

class GameMap {
  public:
    GameMap(int width, int height);
    ~GameMap();

    void render(glm::vec2 center) const;

  private:
    int width, height;
    glm::vec2 map_center;
    std::vector<std::vector<Tile>> tiles;
};

} // namespace dryout

#endif // GAME_MAP_HPP
