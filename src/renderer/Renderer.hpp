#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Shader.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <array>

namespace dryout {

class Renderer {
  public:
    static void init(std::shared_ptr<Shader> shader);
    static void shutdown();

    static void drawQuad();

  private:
    static constexpr int maximum_texture_slots = 80;
    static int texture_slots_count;
    static std::array<int, maximum_texture_slots> texture_slots;
};

} // namespace dryout

#endif // RENDERER_HPP
