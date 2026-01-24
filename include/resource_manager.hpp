#pragma once
#include <glad/glad.h>
#include <vao.hpp>
#include <vbo.hpp>
#include <shader.hpp>
namespace engine
{
    class ResourceManager
    {
    public:
        void deleteAll();
        void registerVBO(VBO *vbo);
        void registerVAO(VAO *vao);
        void removeVBO(VBO *vbo);
        void removeVAO(VAO *vao);

    private:
        std::vector<VBO *> vbos;
        std::vector<VAO *> vaos;
    };
   ResourceManager *getGlobalResourceManager();
}