#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace dryout {

class Shader {
  public:
    Shader();
    ~Shader();

    Shader(const std::string &vertex_source, const std::string &fragment_source);

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    Shader(Shader &&) noexcept;
    Shader &operator=(Shader &&) noexcept;

    void bind() const;
    void unbind() const;

    // Setters for uniform variables, to be called after binding the shader
    void setInt(const std::string &name, int value) const;
    void setIntArray(const std::string &name, const int *values, int count) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;

  private:
    GLuint compileShader(GLenum type, const std::string &shader);

    GLint getUniformLocation(const std::string &name) const;

    GLuint shader_program_id;
};

} // namespace dryout

#endif // SHADER_HPP
