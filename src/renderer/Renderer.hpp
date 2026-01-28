#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace dryout {

class Renderer {
  public:
    static void init();
    static void shutdown();

    static void drawQuad();

  private:
};

} // namespace dryout

#endif // RENDERER_HPP
