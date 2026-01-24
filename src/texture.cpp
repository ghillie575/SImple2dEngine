#include <texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <logger.hpp>
using namespace engine;
Texture::Texture(const std::string &path) : textureID(0), path(path)
{
}
Texture::~Texture()
{
    free();
}
void Texture::create()
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    log("Loading texture: " + path);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        error("Failed to load texture: " + path);
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}
void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}
void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::free()
{
    if (textureID != 0)
    {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}