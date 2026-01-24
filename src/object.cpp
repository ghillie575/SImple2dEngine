#include <object.hpp>
#include <shader_manager.hpp>
#include <resource_manager.hpp>
#include <window.hpp>
#include <random>
using namespace engine;
void Object::intialize()
{
    shader = engine::shader_manager::getShader(shaderName);
    // Initialize VAO, VBO, and EBO
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
        vbo->addVertexAttributePointer(i, vertexAttributes[i].size, GL_FLOAT, GL_FALSE, totalSize, reinterpret_cast<void *>(static_cast<intptr_t>(offset)));
        offset += vertexAttributes[i].size * sizeof(float);
    }
    vbo->unbind();
    engine::getGlobalResourceManager()->registerVAO(&vao);
    engine::getGlobalResourceManager()->registerVBO(vbo);
    if (hasEBO && ebo)
    {
        ebo->create();
        ebo->bind();
        engine::getGlobalResourceManager()->registerEBO(ebo);
    }
    clearVAO();
}
void Object::setTexture(std::string texturePath)
{
    texture = new Texture(texturePath);
    texture->create();
    hasTexture = true;
}
void Object::render()
{
    if (shader)
    {
        shader->use();
        shader->setMat4("transform", transform->getModelMatrix());
    }
    if (hasTexture && texture)
    {
        texture->bind();
    }
    if (hasEBO && ebo)
    {
        vao.bind();
        ebo->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        ebo->unbind();
        return;
    }
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.size() / vertexAttributes.size()));
}