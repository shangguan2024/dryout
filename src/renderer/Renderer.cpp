#include "Renderer.hpp"
#include "Shader.hpp"
#include "Light.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <memory>
#include <iostream>
#include <string>

namespace dryout {

struct QuadVertex {
    QuadVertex() {}
    QuadVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec4 &color,
               const glm::vec2 &tex_coord, int tex_index)
        : position(position), normal(normal), color(color), tex_coord(tex_coord),
          tex_index(tex_index) {}
    QuadVertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b,
               float a, float s, float t, int tex_index)
        : position(x, y, z), normal(nx, ny, nz), color(r, g, b, a), tex_coord(s, t),
          tex_index(tex_index) {}
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
    glm::vec2 tex_coord;
    int tex_index;
};

static constexpr int s_max_tex_slot_count = 32;
static int s_tex_slot_count;
static GLuint s_tex_slots[s_max_tex_slot_count];

static constexpr int s_max_quad_count = 10'0000;
static constexpr int s_max_vertex_count = s_max_quad_count * 4;
static constexpr int s_max_index_count = s_max_quad_count * 6;
static QuadVertex s_quad_vertices[s_max_vertex_count];
static unsigned int s_quad_indices[s_max_index_count];
static int s_quad_count = 0;
static int s_vertex_count = 0;
static int s_index_count = 0;

static constexpr int s_max_light_count = 1000;
static Light s_lights[s_max_light_count];
static int s_light_count = 0;

static std::string s_default_vertex_shader = R"(
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in int a_TexIndex;

layout(location = 0) out vec3 v_FragPos;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec4 v_Color;
layout(location = 3) out vec2 v_TexCoord;
layout(location = 4) flat out int v_TexIndex;

uniform mat4 u_ViewProjectionMatrix;

void main() {
    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
    v_FragPos = a_Position;
    v_Normal = a_Normal;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}
)";
static std::string s_default_fragment_shader = R"(
#version 460 core

layout(location = 0) in vec3 v_FragPos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in vec2 v_TexCoord;
layout(location = 4) flat in int v_TexIndex;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[32];
uniform int u_LightCount;

void main() {
    int dummy = u_LightCount;
    vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);

    if(texColor.a < 0.1) {
        discard;
    }
    o_Color = texColor * v_Color;
}
)";

static std::shared_ptr<Shader> s_shader, s_default_shader;
static GLuint s_vbo;
static GLuint s_ebo;
static GLuint s_vao;
static GLuint s_ssbo;

Renderer::RenderContext Renderer::s_context{};

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
    s_default_shader = std::make_shared<Shader>(s_default_vertex_shader, s_default_fragment_shader);
    s_shader = s_default_shader;

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
    glEnableVertexAttribArray(0); // position
    glEnableVertexAttribArray(1); // normal
    glEnableVertexAttribArray(2); // color
    glEnableVertexAttribArray(3); // tex_coord
    glEnableVertexAttribArray(4); // tex_index
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, normal));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, color));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void *)offsetof(QuadVertex, tex_coord));
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(QuadVertex),
                           (void *)offsetof(QuadVertex, tex_index));

    // Create and bind the shader storage buffer object (SSBO)
    glGenBuffers(1, &s_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, s_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Light) * s_max_light_count, nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, s_ssbo);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

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
    if (shader == nullptr) {
        s_shader = s_default_shader;
    } else {
        s_shader = shader;
    }
    s_shader->bind();
    int samplers[32];
    for (int i = 0; i < 32; i++) {
        samplers[i] = i;
    }
    s_shader->setIntArray("u_Textures", samplers, 32);
}

void Renderer::beginScene() {
    s_quad_count = 0;
    s_vertex_count = 0;
    s_index_count = 0;
    s_shader->bind();
    s_shader->setMat4("u_ViewProjectionMatrix", s_context.view_projection_matrix);
    s_shader->setInt("u_LightCount", s_light_count);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, s_ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Light) * s_light_count, s_lights);
}

void Renderer::endScene() {
    flush();
    s_light_count = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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

void Renderer::drawQuad(const glm::vec3 &position, const glm::vec2 &size, RenderType type,
                        const glm::vec4 &color, const std::shared_ptr<Texture> &texture,
                        const glm::vec2 &tex_coords, const glm::vec2 &tex_size) {
    if (s_quad_count >= s_max_quad_count) {
        flush();
    }

    // Get the texture ID and bind it to a texture slot
    GLuint texture_id = texture->getTextureID();
    int tex_slot = getTextureSlot(texture_id);
    if (tex_slot == -1) {
        tex_slot = bindTextureSlot(texture_id);
    }
    if (tex_slot == -1) {
        flush();
        resetTextureSlots();
        tex_slot = bindTextureSlot(texture_id);
    }

    // Add the quad vertices to the buffer
    const auto &p = position;
    const auto &s = size;
    const auto &tc = tex_coords;
    const auto &ts = tex_size;
    glm::vec2 yz(s.y, 0.0f);
    if (type == RenderType::BILLBOARD) {
        yz = glm::rotate(yz, s_context.polar_angle); // TODO: 3d billboard rotation
    }
    glm::vec3 rd(s.x, 0.0f, 0.0f);
    glm::vec3 ld(0.0f, 0.0f, 0.0f);
    glm::vec3 lu(0.0f, yz);
    glm::vec3 ru(s.x, yz);
    glm::vec3 n = glm::normalize(glm::cross(rd, lu));
    if (type == RenderType::BILLBOARD) {
        // TODO: better solution
        n = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    s_quad_vertices[s_vertex_count++] = QuadVertex(p + rd, n, color, tc + ts, tex_slot);
    s_quad_vertices[s_vertex_count++] = QuadVertex(p + ld, n, color, {tc.x, tc.y + ts.y}, tex_slot);
    s_quad_vertices[s_vertex_count++] = QuadVertex(p + lu, n, color, tc, tex_slot);
    s_quad_vertices[s_vertex_count++] = QuadVertex(p + ru, n, color, {tc.x + ts.x, tc.y}, tex_slot);

    // Add the quad indices to the buffer
    s_quad_indices[s_index_count++] = s_vertex_count - 4;
    s_quad_indices[s_index_count++] = s_vertex_count - 3;
    s_quad_indices[s_index_count++] = s_vertex_count - 2;
    s_quad_indices[s_index_count++] = s_vertex_count - 2;
    s_quad_indices[s_index_count++] = s_vertex_count - 1;
    s_quad_indices[s_index_count++] = s_vertex_count - 4;

    s_quad_count++;
}

void Renderer::putLight(const Light &light) {
    s_lights[s_light_count++] = light;
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
