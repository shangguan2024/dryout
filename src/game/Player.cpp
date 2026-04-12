#include "Player.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include "CameraManager.hpp"
#include "GameMap.hpp"

#include <glm/glm.hpp>

namespace dryout {

static constexpr float s_default_player_velocity = 100.0f;
static constexpr glm::vec2 s_player_size(10.0f);
static constexpr float s_default_interaction_radius = 32.0f;

Player::Player(const std::string &name)
    : position(0.0f), velocity(s_default_player_velocity), direction(0.0f),
      interaction_radius(s_default_interaction_radius) {
    sprite = ResourceManager::getInstance()->getSprite(TextureType::SPRITE, name, s_player_size);
    init();
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

void Player::init() {
    InputManager *input_manager = InputManager::getInstance();
    input_manager->registerMouseCallback(
        InputType::DOWN, MouseButton::LEFT,
        [this](const glm::vec2 &screen_pos) { interact(screen_pos); });
}

void Player::interact(const glm::vec2 &screen_pos) {
    Camera *camera = CameraManager::getInstance()->getActiveCamera();
    if (camera == nullptr) {
        return;
    }

    glm::mat2x3 ray = camera->getRay(screen_pos);
    auto &near_point = ray[0];
    auto &far_point = ray[1];
    float t = -near_point.z / (far_point.z - near_point.z);
    glm::vec2 world_pos = near_point + t * (far_point - near_point);

    if (glm::distance(position, world_pos) > interaction_radius) {
        return;
    }

    // TODO: get rid of test method
    g_map->test(world_pos);
}

} // namespace dryout
