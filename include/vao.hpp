#pragma once
#include <glad/glad.h>
namespace engine
{
    void clearVAO();
    class VAO
    {
    public:
        unsigned int ID;
        VAO() {};
        void create();
        void bind();
        void free();
    };
}