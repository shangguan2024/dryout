#include "Shader.hpp"

#include <iostream>

namespace dryout {

Shader::Shader() : shader_program_id(0) {}

Shader::~Shader() {}

Shader::Shader(const std::string &vertex_source, const std::string &fragment_source)
    : shader_program_id(0) {
    GLuint vertex_shader_id = compileShader(GL_VERTEX_SHADER, vertex_source);
    if (!vertex_shader_id) {
        std::cerr << "Failed to compile vertex shader!" << std::endl;
        return;
    }

    GLuint fragment_shader_id = compileShader(GL_FRAGMENT_SHADER, fragment_source);
    if (!fragment_shader_id) {
        std::cerr << "Failed to compile fragment shader!" << std::endl;
        return;
    }

    shader_program_id = glCreateProgram();
    if (!shader_program_id) {
        std::cerr << "Failed to create shader program!" << std::endl;
        return;
    }

    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    GLint success;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLint log_length;
        glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &log_length);
        GLchar *info_log = new GLchar[log_length];
        glGetProgramInfoLog(shader_program_id, log_length, nullptr, info_log);
        std::cerr << "Error: Failed to link shader program! Info Log: " << info_log << std::endl;
        delete[] info_log;
        return;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

void Shader::bind() const {}

void Shader::unbind() const {}

GLuint Shader::compileShader(GLenum type, const std::string &shader) {
    GLuint shader_id = glCreateShader(type);
    const char *shader_source = shader.c_str();
    glShaderSource(shader_id, 1, &shader_source, nullptr);
    glCompileShader(shader_id);

    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        GLchar *info_log = new GLchar[log_length];
        glGetShaderInfoLog(shader_id, log_length, nullptr, info_log);
        std::cerr << "Error: Failed to compile shader! Info Log: " << info_log << std::endl;
        delete[] info_log;
        return 0;
    }

    return shader_id;
}

} // namespace dryout
