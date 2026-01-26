#include "Texture.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace dryout {

Texture::Texture() : texture_id(0), width(0), height(0), channels(0) {}

Texture::~Texture() {
    if (texture_id) {
        glDeleteTextures(1, &texture_id);
    }
}

Texture::Texture(const SDL_Surface *surface) : texture_id(0), width(0), height(0), channels(0) {
    if (!surface) {
        std::cerr << "SDL_Surface is null!" << std::endl;
        return;
    }

    width = surface->w;
    height = surface->h;
    channels = surface->format->BytesPerPixel;

    GLenum format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                 surface->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture &&other) noexcept
    : texture_id(other.texture_id), width(other.width), height(other.height),
      channels(other.channels) {
    other.texture_id = 0;
    other.width = 0;
    other.height = 0;
    other.channels = 0;
}

Texture &Texture::operator=(Texture &&other) noexcept {
    if (this != &other) {
        if (texture_id) {
            glDeleteTextures(1, &texture_id);
        }
        texture_id = other.texture_id;
        width = other.width;
        height = other.height;
        channels = other.channels;

        other.texture_id = 0;
        other.width = 0;
        other.height = 0;
        other.channels = 0;
    }
    return *this;
}

void Texture::bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

int Texture::getChannels() const {
    return channels;
}

GLuint Texture::getTextureID() const {
    return texture_id;
}

bool Texture::operator==(const Texture &other) const {
    return texture_id == other.texture_id;
}

} // namespace dryout
