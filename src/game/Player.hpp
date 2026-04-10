#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Sprite.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace dryout {

class Player {
  public:
    Player(); // TODO: Player(...)
    ~Player();

    glm::vec2 getPosition() const;

    void update(float delta);
    void render() const;

  private:
    glm::vec2 position;
    float velocity;
    glm::vec2 direction;

    std::unique_ptr<Sprite> sprite;
};

} // namespace dryout

#endif // PLAYER_HPP
