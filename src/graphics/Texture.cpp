#include "Texture.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace dryout {

Texture::Texture() {}

Texture::~Texture() {}

Texture::Texture(const SDL_Surface *surface) : texture_id(0) {
    if (!surface) {
        std::cerr << "SDL_Surface is null!" << std::endl;
        return;
    }

    width = surface->w;
    height = surface->h;
    channels = surface->format->BytesPerPixel;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(GLuint slot) const {
    // todo
}

void Texture::unbind() const {
    // todo
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
