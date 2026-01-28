#include "Renderer.hpp"
#include "Shader.hpp"

#include <glad/glad.h>
#include <memory>
#include <iostream>
#include <string>

namespace dryout {

struct QuadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float tex_index;
};

static std::string s_default_vertex_shader = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

uniform mat4 u_ViewProjectionMatrix;

void main() {
    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
}
)";
static std::string s_default_fragment_shader = R"(
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main() {
    vec4 texColor = v_Color;

    int index = int(v_TexIndex);
    texColor *= texture(u_Textures[index], v_TexCoord);

    if (texColor.a < 0.1) {
        discard;
    }
    o_Color = texColor;
}
)";

static std::shared_ptr<Shader> s_shader;
static GLuint s_default_texture;
static GLuint s_vbo;
static GLuint s_ebo;
static GLuint s_vao;

void Renderer::init() {
    std::cout << "Initializing renderer..." << std::endl;

    // Enable blending for transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the shader to be used by the renderer
    s_shader = std::make_shared<Shader>(s_default_vertex_shader, s_default_fragment_shader);

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
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 1.0f},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, 1.0f},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, 1.0f},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, 1.0f},
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
