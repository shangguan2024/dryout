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

InputManager *InputManager::getInstance() {
    if (instance == nullptr) {
        instance = new InputManager();
    }
    return instance;
}

bool InputManager::handleEvent(SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        key_states[sdlKeyToEnum(event.key.keysym.scancode)] = true;
        break;
    case SDL_KEYUP:
        key_states[sdlKeyToEnum(event.key.keysym.scancode)] = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouse_states[sdlMouseButtonToEnum(event.button.button)] = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mouse_states[sdlMouseButtonToEnum(event.button.button)] = false;
        break;
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

InputManager::InputManager() {
    // TODO
}

InputManager::~InputManager() {
    // TODO
}

} // namespace dryout
