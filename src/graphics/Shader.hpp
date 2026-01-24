#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>

namespace dryout {

class Shader {
  public:
    Shader();
    ~Shader();

    Shader(const std::string &vertex_source, const std::string &fragment_source);

    void bind() const;
    void unbind() const;

  private:
    GLuint compileShader(GLenum type, const std::string &shader);

    GLuint shader_program_id;
};

} // namespace dryout

#endif // SHADER_HPP
