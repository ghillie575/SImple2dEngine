#pragma once
#include <glad/glad.h>
#include <vao.hpp>
#include <vbo.hpp>
#include <ebo.hpp>
#include <shader.hpp>
namespace engine
{
    class ResourceManager
    {
    public:
        void deleteAll();
        void registerVBO(VBO *vbo);
        void registerVAO(VAO *vao);
        void registerEBO(EBO *ebo);
        void removeVBO(VBO *vbo);
        void removeVAO(VAO *vao);
        void removeEBO(EBO *ebo);

    private:
        std::vector<VBO *> vbos;
        std::vector<VAO *> vaos;
        std::vector<EBO *> ebos;
    };
   ResourceManager *getGlobalResourceManager();
}