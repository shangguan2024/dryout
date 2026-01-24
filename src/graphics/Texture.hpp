#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "json.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>

using json = nlohmann::json;

namespace dryout {

class Texture {
  public:
    Texture();
    ~Texture();

    Texture(const SDL_Surface *surface);

    void bind(GLuint slot) const;
    void unbind() const;

    GLuint getTextureID() const;
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;

    bool operator==(const Texture &other) const;

  private:
    GLuint texture_id;
    int width, height, channels;
};

} // namespace dryout

#endif // TEXTURE_HPP
