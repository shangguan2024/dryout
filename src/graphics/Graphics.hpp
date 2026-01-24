#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <string>

namespace dryout {

class Graphics {
  public:
    static Graphics *getInstance();

    GLuint compileShader(GLenum type, const std::string &shader);
    GLuint createShaderProgram(const std::string &vertex_source,
                               const std::string &fragment_source);

    void swapWindow();

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
