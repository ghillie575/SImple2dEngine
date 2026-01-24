#include <ebo.hpp>
#include <resource_manager.hpp>
#include <glad/glad.h>
namespace engine
{
    EBO::~EBO()
    {
        glDeleteBuffers(1, &eboID);
    }
    void EBO::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    }
    void EBO::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void EBO::free()
    {
        glDeleteBuffers(1, &eboID);
    }
    void EBO::create()
    {
        glGenBuffers(1, &eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }
}