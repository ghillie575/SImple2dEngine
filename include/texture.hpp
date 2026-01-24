#pragma once
#include <glad/glad.h>
#include <string>
namespace engine
{
    class Texture
    {
    public:
        Texture(const std::string &path);
        ~Texture();
        void create();
        void bind() const;
        void unbind() const;
        void free();
        unsigned int getID() const
        {
            return textureID;
        }

    private:
        unsigned int textureID;
        std::string path;
    };
} // namespace engine