#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>
#include <unordered_map>

namespace dryout {

enum class KeyCode : unsigned int {
    UNKNOWN = 0,
    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    O,
    P,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    Z,
    X,
    C,
    V,
    B,
    N,
    M,
    SPACE,
    ESC,
    ENTER,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum class MouseButton : unsigned int {
    UNKNOWN = 0,
    LEFT,
    RIGHT,
    MIDDLE,
};

class InputManager {
  public:
    static InputManager *getInstance();

    bool handleEvent(SDL_Event &event);

    bool isKeyDown(KeyCode key);
    bool isMouseButtonDown(MouseButton button);

  private:
    InputManager();
    ~InputManager();

    InputManager(const InputManager &) = delete;
    InputManager &operator=(const InputManager &) = delete;
    InputManager(InputManager &&) = delete;
    InputManager &operator=(InputManager &&) = delete;

    static InputManager *instance;

    std::unordered_map<KeyCode, bool> key_states;
    std::unordered_map<MouseButton, bool> mouse_states;
};

} // namespace dryout

#endif // INPUT_MANAGER_HPP
