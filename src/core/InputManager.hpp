#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <stack>
#include <functional>

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

enum class InputType : unsigned int {
    UNKNOWN = 0,
    UP,
    DOWN,
};

class InputManager {
  public:
    static InputManager *getInstance();

    bool handleEvent(SDL_Event &event);

    bool isKeyDown(KeyCode key);
    bool isMouseButtonDown(MouseButton button);

    glm::vec2 getMousePosition();

    void pushKeyCallback();
    void pushMouseCallback();

    void popKeyCallback();
    void popMouseCallback();

    void registerKeyCallback(InputType type, KeyCode key, std::function<void()> callback);
    void registerMouseCallback(InputType type, MouseButton button,
                               std::function<void(glm::vec2)> callback);

    void unregisterKeyCallback(InputType type, KeyCode key);
    void unregisterMouseCallback(InputType type, MouseButton button);

  private:
    InputManager();
    ~InputManager();

    InputManager(const InputManager &) = delete;
    InputManager &operator=(const InputManager &) = delete;
    InputManager(InputManager &&) = delete;
    InputManager &operator=(InputManager &&) = delete;

    void keyCallback(InputType type, KeyCode key);
    void mouseCallback(InputType type, MouseButton button, glm::vec2 position);

    static InputManager *instance;

    std::unordered_map<KeyCode, bool> key_states;
    std::unordered_map<MouseButton, bool> mouse_states;

    std::stack<std::unordered_map<KeyCode, std::function<void()>>> key_down_callbacks,
        key_up_callbacks;
    std::stack<std::unordered_map<MouseButton, std::function<void(glm::vec2)>>>
        mouse_down_callbacks, mouse_up_callbacks; // param: mouse cursor position under window space
};

} // namespace dryout

#endif // INPUT_MANAGER_HPP
