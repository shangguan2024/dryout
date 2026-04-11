#include "Renderer.hpp"
#include "Shader.hpp"

#include <glad/glad.h>
#include <memory>
#include <iostream>
#include <string>

namespace dryout {

struct QuadVertex {
    QuadVertex() {}
    QuadVertex(const glm::vec3 &position, const glm::vec4 &color, const glm::vec2 &tex_coord,
               float tex_index)
        : position(position), color(color), tex_coord(tex_coord), tex_index(tex_index) {}
    QuadVertex(float x, float y, float z, float r, float g, float b, float a, float s, float t,
               float tex_index)
        : position(x, y, z), color(r, g, b, a), tex_coord(s, t), tex_index(tex_index) {}
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float tex_index;
};

static constexpr int s_max_quad_count = 100'000;
static constexpr int s_max_vertex_count = s_max_quad_count * 4;
static constexpr int s_max_index_count = s_max_quad_count * 6;

static QuadVertex s_quad_vertices[s_max_vertex_count];
static unsigned int s_quad_indices[s_max_index_count];
static int s_quad_count = 0;
static int s_vertex_count = 0;
static int s_index_count = 0;

static constexpr int s_max_tex_slot_count = 32;
static int s_tex_slot_count;
static GLuint s_tex_slots[s_max_tex_slot_count];

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
static GLuint s_vbo;
static GLuint s_ebo;
static GLuint s_vao;

void Renderer::init() {
    std::cout << "Initializing renderer..." << std::endl;

    // Get the maximum number of texture units supported by the GPU
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_tex_slot_count);
    if (s_tex_slot_count > s_max_tex_slot_count) {
        s_tex_slot_count = s_max_tex_slot_count;
    }
    resetTextureSlots();

    // Enable blending for transparent textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the shader to be used by the renderer
    s_shader = std::make_shared<Shader>(s_default_vertex_shader, s_default_fragment_shader);

    // Initialize shader
    s_shader->bind();

    // Set default ViewProjectionMatrix (Identity)
    s_shader->setMat4("u_ViewProjectionMatrix", glm::mat4(1.0f));

    // Set the texture slots in the shader
    int samplers[32];
    for (int i = 0; i < 32; i++) {
        samplers[i] = i;
    }
    s_shader->setIntArray("u_Textures", samplers, 32);

    // Create and bind the vertex buffer object (VBO)
    glGenBuffers(1, &s_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_quad_vertices), s_quad_vertices, GL_DYNAMIC_DRAW);

    // Create and bind the element buffer object (EBO)
    glGenBuffers(1, &s_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_quad_indices), s_quad_indices, GL_DYNAMIC_DRAW);

    // Create and bind the vertex array object (VAO)
    glGenVertexArrays(1, &s_vao);
    glBindVertexArray(s_vao);
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

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

    std::cout << "Renderer initialized." << std::endl;
}

void Renderer::shutdown() {
    if (s_vao) {
        glDeleteVertexArrays(1, &s_vao);
    }
    if (s_vbo) {
        glDeleteBuffers(1, &s_vbo);
    }
    if (s_ebo) {
        glDeleteBuffers(1, &s_ebo);
    }
    s_vao = 0;
    s_vbo = 0;
    s_ebo = 0;
    s_quad_count = 0;
    s_vertex_count = 0;
    s_index_count = 0;
    resetTextureSlots();
}

void Renderer::setShader(const std::shared_ptr<Shader> &shader) {
    s_shader = shader;
}

void Renderer::beginScene(const glm::mat4 &view_projection_matrix) {
    s_quad_count = 0;
    s_vertex_count = 0;
    s_index_count = 0;
    s_shader->bind();
    s_shader->setMat4("u_ViewProjectionMatrix", view_projection_matrix);
}

void Renderer::endScene() {
    flush();
}

void Renderer::flush() {
    if (s_quad_count == 0) {
        return;
    }

    s_shader->bind();
    glBindVertexArray(s_vao);

    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, s_vertex_count * sizeof(QuadVertex), s_quad_vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, s_index_count * sizeof(unsigned int),
                    s_quad_indices);

    glDrawElements(GL_TRIANGLES, s_index_count, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    s_quad_count = 0;
    s_vertex_count = 0;
    s_index_count = 0;
}

void Renderer::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color,
                        const std::shared_ptr<Texture> &texture, const glm::vec2 &texture_coords,
                        const glm::vec2 &texture_size) {
    if (s_quad_count >= s_max_quad_count) {
        flush();
    }

    // Get the texture ID and bind it to a texture slot
    GLuint texture_id = texture->getTextureID();
    int texture_slot = getTextureSlot(texture_id);
    if (texture_slot == -1) {
        texture_slot = bindTextureSlot(texture_id);
    }
    if (texture_slot == -1) {
        flush();
        resetTextureSlots();
        texture_slot = bindTextureSlot(texture_id);
    }

    // Add the quad vertices to the buffer
    const auto &p = position;
    const auto &s = size;
    const auto &tc = texture_coords;
    const auto &ts = texture_size;
    s_quad_vertices[s_vertex_count++] =
        QuadVertex({p.x + s.x, p.y, p.z}, color, {tc.x + ts.x, tc.y + ts.y}, texture_slot);
    s_quad_vertices[s_vertex_count++] = QuadVertex(p, color, {tc.x, tc.y + ts.y}, texture_slot);
    s_quad_vertices[s_vertex_count++] = QuadVertex({p.x, p.y + s.y, p.z}, color, tc, texture_slot);
    s_quad_vertices[s_vertex_count++] =
        QuadVertex({p.x + s.x, p.y + s.y, p.z}, color, {tc.x + ts.x, tc.y}, texture_slot);

    // Add the quad indices to the buffer
    s_quad_indices[s_index_count++] = s_vertex_count - 4;
    s_quad_indices[s_index_count++] = s_vertex_count - 3;
    s_quad_indices[s_index_count++] = s_vertex_count - 2;
    s_quad_indices[s_index_count++] = s_vertex_count - 2;
    s_quad_indices[s_index_count++] = s_vertex_count - 1;
    s_quad_indices[s_index_count++] = s_vertex_count - 4;

    s_quad_count++;
}

void Renderer::resetTextureSlots() {
    for (int i = 0; i < s_tex_slot_count; i++) {
        if (s_tex_slots[i] != -1) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        s_tex_slots[i] = -1;
    }
}

int Renderer::getTextureSlot(GLuint texture_id) {
    for (int i = 0; i < s_tex_slot_count; i++) {
        if (s_tex_slots[i] == texture_id) {
            return i;
        }
    }
    return -1;
}

int Renderer::getTextureSlot(std::shared_ptr<Texture> texture) {
    GLuint texture_id = texture->getTextureID();
    for (int i = 0; i < s_tex_slot_count; i++) {
        if (s_tex_slots[i] == texture_id) {
            return i;
        }
    }
    return -1;
}

int Renderer::bindTextureSlot(GLuint texture_id) {
    for (int i = 0; i < s_tex_slot_count; i++) {
        if (s_tex_slots[i] == -1) {
            s_tex_slots[i] = texture_id;
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            return i;
        }
    }
    return -1;
}

int Renderer::bindTextureSlot(std::shared_ptr<Texture> texture) {
    GLuint texture_id = texture->getTextureID();
    for (int i = 0; i < s_tex_slot_count; i++) {
        if (s_tex_slots[i] == -1) {
            s_tex_slots[i] = texture_id;
            texture->bind(i);
            return i;
        }
    }
    return -1;
}

} // namespace dryout
