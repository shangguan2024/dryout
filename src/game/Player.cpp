#include "Player.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"

#include <glm/glm.hpp>

namespace dryout {

static constexpr float s_default_player_velocity = 100.0f;
static constexpr glm::vec2 s_player_size(10.0f);

Player::Player() : position(0.0f), velocity(s_default_player_velocity), direction(0.0f) {
    sprite = ResourceManager::getInstance()->getSprite(TextureType::SPRITE_ATLAS, "player",
                                                       s_player_size);
}

Player::~Player() {
    // TODO: Clean up player
}

glm::vec2 Player::getPosition() const {
    return position;
}

void Player::update(float delta) {
    InputManager *input_manager = InputManager::getInstance();

    direction = {};
    if (input_manager->isKeyDown(KeyCode::LEFT) || input_manager->isKeyDown(KeyCode::A)) {
        direction.x -= 1.0f;
    }
    if (input_manager->isKeyDown(KeyCode::RIGHT) || input_manager->isKeyDown(KeyCode::D)) {
        direction.x += 1.0f;
    }
    if (input_manager->isKeyDown(KeyCode::UP) || input_manager->isKeyDown(KeyCode::W)) {
        direction.y += 1.0f;
    }
    if (input_manager->isKeyDown(KeyCode::DOWN) || input_manager->isKeyDown(KeyCode::S)) {
        direction.y -= 1.0f;
    }
    if (direction != glm::vec2(0.0f, 0.0f)) {
        direction = glm::normalize(direction);
        position += direction * velocity * delta;
    }
}

void Player::render() const {
    sprite->render(position, 0.1f);
}

} // namespace dryout
