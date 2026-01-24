#include <vao.hpp>
using namespace engine;
void VAO::create()
{
    glGenVertexArrays(1, &ID);
}
void VAO::bind()
{
    glBindVertexArray(ID);
}
void VAO::free()
{
    glDeleteVertexArrays(1, &ID);
}
void engine::clearVAO()
{
    glBindVertexArray(0);
}