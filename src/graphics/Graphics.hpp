#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>

namespace dryout {

class Graphics {
  public:
    static Graphics *getInstance();

  private:
    Graphics();
    ~Graphics();

    Graphics(const Graphics &) = delete;
    Graphics &operator=(const Graphics &) = delete;
    Graphics(Graphics &&) = delete;
    Graphics &operator=(Graphics &&) = delete;

    void init();

    static Graphics *instance;

    SDL_Window *window;
    SDL_GLContext context;
};

} // namespace dryout

#endif // GRAPHICS_HPP
