#ifndef TILE_HPP
#define TILE_HPP

#include "Sprite.hpp"

#include <glm/glm.hpp>
#include <array>
#include <memory>

namespace dryout {

constexpr float g_tile_size = 10.0f;

enum class TileType : unsigned int {
    UNKNOWN = 0,
    SAND,
    WET_SAND,
    COUNT,
};

class TileFlyweight {
  public:
    static TileFlyweight *getInstance();

    void render(TileType type, const glm::vec2 &position) const;

  private:
    TileFlyweight();
    ~TileFlyweight() = default;

    TileFlyweight(const TileFlyweight &) = delete;
    TileFlyweight &operator=(const TileFlyweight &) = delete;
    TileFlyweight(TileFlyweight &&) = delete;
    TileFlyweight &operator=(TileFlyweight &&) = delete;

    static TileFlyweight *instance;
    std::array<std::unique_ptr<Sprite>, static_cast<unsigned int>(TileType::COUNT)> flyweight;
};

class Tile {
  public:
    Tile();
    Tile(TileType type);
    ~Tile();

    TileType getType() const;
    void setType(TileType type);

    void render(const glm::vec2 &position) const;

  private:
    TileType type;
    // Other properties
};

} // namespace dryout

#endif // TILE_HPP
