#include <vbo.hpp>
using namespace engine;
void VBO::create()
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
}
void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::free()
{
    glDeleteBuffers(1, &ID);
}
void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::addVertexAttributePointer(unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}