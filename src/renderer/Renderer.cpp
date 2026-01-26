#include "Renderer.hpp"
#include "Shader.hpp"

#include <glad/glad.h>
#include <memory>
#include <iostream>

namespace dryout {

struct QuadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float tex_index;
};

static std::shared_ptr<Shader> s_shader;
static GLuint s_default_texture;
static GLuint s_vbo;
static GLuint s_ebo;
static GLuint s_vao;

int Renderer::texture_slots_count = 0;
std::array<int, Renderer::maximum_texture_slots> Renderer::texture_slots = {-1};

void Renderer::init(std::shared_ptr<Shader> shader) {
    std::cout << "Initializing renderer..." << std::endl;

    // Get the maximum number of texture units supported by the GPU
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &texture_slots_count);
    if (texture_slots_count > maximum_texture_slots) {
        texture_slots_count = maximum_texture_slots;
    }

    // Enable blending for transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the shader to be used by the renderer
    s_shader = shader;

    // Initialize shader
    s_shader->bind();

    // Create 1x1 white texture
    glGenTextures(1, &s_default_texture);
    glBindTexture(GL_TEXTURE_2D, s_default_texture);
    unsigned int white_data = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind white texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_default_texture);
    texture_slots[0] = s_default_texture;
    texture_slots_count = 1;

    // Set default ViewProjectionMatrix (Identity)
    s_shader->setMat4("u_ViewProjectionMatrix", glm::mat4(1.0f));

    // Set the texture slots in the shader
    int samplers[32];
    for (int i = 0; i < 32; i++) {
        samplers[i] = i;
    }
    s_shader->setIntArray("u_Textures", samplers, 32);

    // Create the quad vertices and indices
    QuadVertex vertices[] = {
        // position             color                     tex_coord    tex_index
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 0.0f},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, 0.0f},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 0.0f},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, 0.0f},
    };
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    // Create and bind the vertex buffer object (VBO)
    glGenBuffers(1, &s_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create and bind the element buffer object (EBO)
    glGenBuffers(1, &s_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create and bind the vertex array object (VAO)
    glGenVertexArrays(1, &s_vao);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, tex_coord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, tex_index));
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

    std::cout << "Renderer initialized." << std::endl;
}

void Renderer::shutdown() {
    // todo
}

void Renderer::drawQuad() {
    s_shader->bind();
    glBindVertexArray(s_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace dryout
