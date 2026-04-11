#include "InputManager.hpp"

#include <iostream>

namespace dryout {

static KeyCode sdlKeyToEnum(SDL_Scancode key) {
    switch (key) {
    case SDL_SCANCODE_ESCAPE:
        return KeyCode::ESC;
    case SDL_SCANCODE_SPACE:
        return KeyCode::SPACE;
    case SDL_SCANCODE_LEFT:
        return KeyCode::LEFT;
    case SDL_SCANCODE_RIGHT:
        return KeyCode::RIGHT;
    case SDL_SCANCODE_UP:
        return KeyCode::UP;
    case SDL_SCANCODE_DOWN:
        return KeyCode::DOWN;
    case SDL_SCANCODE_RETURN:
        return KeyCode::ENTER;
    case SDL_SCANCODE_Q:
        return KeyCode::Q;
    case SDL_SCANCODE_W:
        return KeyCode::W;
    case SDL_SCANCODE_E:
        return KeyCode::E;
    case SDL_SCANCODE_R:
        return KeyCode::R;
    case SDL_SCANCODE_T:
        return KeyCode::T;
    case SDL_SCANCODE_Y:
        return KeyCode::Y;
    case SDL_SCANCODE_U:
        return KeyCode::U;
    case SDL_SCANCODE_I:
        return KeyCode::I;
    case SDL_SCANCODE_O:
        return KeyCode::O;
    case SDL_SCANCODE_P:
        return KeyCode::P;
    case SDL_SCANCODE_A:
        return KeyCode::A;
    case SDL_SCANCODE_S:
        return KeyCode::S;
    case SDL_SCANCODE_D:
        return KeyCode::D;
    case SDL_SCANCODE_F:
        return KeyCode::F;
    case SDL_SCANCODE_G:
        return KeyCode::G;
    case SDL_SCANCODE_H:
        return KeyCode::H;
    case SDL_SCANCODE_J:
        return KeyCode::J;
    case SDL_SCANCODE_K:
        return KeyCode::K;
    case SDL_SCANCODE_L:
        return KeyCode::L;
    case SDL_SCANCODE_Z:
        return KeyCode::Z;
    case SDL_SCANCODE_X:
        return KeyCode::X;
    case SDL_SCANCODE_C:
        return KeyCode::C;
    case SDL_SCANCODE_V:
        return KeyCode::V;
    case SDL_SCANCODE_B:
        return KeyCode::B;
    case SDL_SCANCODE_N:
        return KeyCode::N;
    case SDL_SCANCODE_M:
        return KeyCode::M;
    default:
        return KeyCode::UNKNOWN;
    }
}

static MouseButton sdlMouseButtonToEnum(Uint8 button) {
    switch (button) {
    case SDL_BUTTON_LEFT:
        return MouseButton::LEFT;
    case SDL_BUTTON_RIGHT:
        return MouseButton::RIGHT;
    case SDL_BUTTON_MIDDLE:
        return MouseButton::MIDDLE;
    default:
        return MouseButton::UNKNOWN;
    }
}

InputManager *InputManager::instance = nullptr;

InputManager::InputManager() {
    pushKeyCallback();
    pushMouseCallback();
}

InputManager::~InputManager() {
    // TODO
}

InputManager *InputManager::getInstance() {
    if (instance == nullptr) {
        instance = new InputManager();
    }
    return instance;
}

bool InputManager::handleEvent(SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN: {
        auto key = sdlKeyToEnum(event.key.keysym.scancode);
        if (!key_states[key]) {
            keyCallback(InputType::DOWN, key);
            key_states[key] = true;
        }
    } break;
    case SDL_KEYUP: {
        auto key = sdlKeyToEnum(event.key.keysym.scancode);
        if (key_states[key]) {
            keyCallback(InputType::UP, key);
            key_states[key] = false;
        }
    } break;
    case SDL_MOUSEBUTTONDOWN: {
        auto button = sdlMouseButtonToEnum(event.button.button);
        if (!mouse_states[button]) {
            mouse_states[button] = true;
            mouseCallback(InputType::DOWN, button, glm::vec2(event.button.x, event.button.y));
        }
    } break;
    case SDL_MOUSEBUTTONUP: {
        auto button = sdlMouseButtonToEnum(event.button.button);
        if (mouse_states[button]) {
            mouse_states[button] = false;
            mouseCallback(InputType::UP, button, glm::vec2(event.button.x, event.button.y));
        }
    } break;
    default:
        return false;
    }
    return true;
}

bool InputManager::isKeyDown(KeyCode key) {
    return key_states[key];
}

bool InputManager::isMouseButtonDown(MouseButton button) {
    return mouse_states[button];
}

glm::vec2 InputManager::getMousePosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return glm::vec2(x, y);
}

void InputManager::pushKeyCallback() {
    key_down_callbacks.push({});
    key_up_callbacks.push({});
}

void InputManager::pushMouseCallback() {
    mouse_down_callbacks.push({});
    mouse_up_callbacks.push({});
}

void InputManager::popKeyCallback() {
    if (key_down_callbacks.empty()) {
        return;
    }
    key_down_callbacks.pop();
    key_up_callbacks.pop();
}

void InputManager::popMouseCallback() {
    if (mouse_down_callbacks.empty()) {
        return;
    }
    mouse_down_callbacks.pop();
    mouse_up_callbacks.pop();
}

void InputManager::registerKeyCallback(InputType type, KeyCode key,
                                       std::function<void()> callback) {
    if (type == InputType::DOWN) {
        key_down_callbacks.top()[key] = callback;
    } else if (type == InputType::UP) {
        key_up_callbacks.top()[key] = callback;
    }
}

void InputManager::registerMouseCallback(InputType type, MouseButton button,
                                         std::function<void(glm::vec2)> callback) {
    if (type == InputType::DOWN) {
        mouse_down_callbacks.top()[button] = callback;
    } else if (type == InputType::UP) {
        mouse_up_callbacks.top()[button] = callback;
    }
}

void InputManager::unregisterKeyCallback(InputType type, KeyCode key) {
    if (type == InputType::DOWN) {
        key_down_callbacks.top().erase(key);
    } else if (type == InputType::UP) {
        key_up_callbacks.top().erase(key);
    }
}

void InputManager::unregisterMouseCallback(InputType type, MouseButton button) {
    if (type == InputType::DOWN) {
        mouse_down_callbacks.top().erase(button);
    } else if (type == InputType::UP) {
        mouse_up_callbacks.top().erase(button);
    }
}

void InputManager::keyCallback(InputType type, KeyCode key) {
    auto &key_callbacks = type == InputType::DOWN ? key_down_callbacks : key_up_callbacks;
    if (key_callbacks.empty()) {
        return;
    }
    auto &callbacks = key_callbacks.top();
    if (callbacks[key] != nullptr) {
        callbacks[key]();
    }
}

void InputManager::mouseCallback(InputType type, MouseButton button, glm::vec2 position) {
    std::cout << "Mouse callback: " << position.x << ", " << position.y << std::endl;
    auto &mouse_callbacks = type == InputType::DOWN ? mouse_down_callbacks : mouse_up_callbacks;
    if (mouse_callbacks.empty()) {
        return;
    }
    auto &callbacks = mouse_callbacks.top();
    if (callbacks[button] != nullptr) {
        callbacks[button](position);
    }
}

} // namespace dryout
