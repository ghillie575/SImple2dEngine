#include <object.hpp>
#include <shader_manager.hpp>
#include <resource_manager.hpp>
using namespace engine;
void Object::intialize()
{
    shader = engine::shader_manager::getShader(shaderName);
    vbo = new VBO(vertexData);
    vao.create();
    vbo->create();
    vao.bind();
    vbo->bind();
    int totalSize = 0;
    for (auto attribute : vertexAttributes)
    {
        totalSize += attribute.size * sizeof(float);
    }
    int offset = 0;
    for (size_t i = 0; i < vertexAttributes.size(); ++i)
    {
        vbo->addVertexAttributePointer(i, vertexAttributes[i].size, GL_FLOAT, GL_FALSE, totalSize, (void *)(offset));
        offset += vertexAttributes[i].size * sizeof(float);
    }
    vbo->unbind();
    engine::getGlobalResourceManager()->registerVAO(&vao);
    engine::getGlobalResourceManager()->registerVBO(vbo);
    clearVAO();
}
void Object::render()
{
    if (shader)
    {
        shader->use();
    }
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}