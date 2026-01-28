#include <object.hpp>
#include <shader_manager.hpp>
#include <resource_manager.hpp>
#include <window.hpp>
#include <random>
using namespace engine;
void Object::intialize()
{
    shader = engine::shader_manager::getShader(shaderName);
    if (!shader)
    {
        error("Shader " + shaderName + " not found for object " + objectId);
        return;
    }
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
        shader->setMat4("transform", transform->getModelMatrix(anchorPoint));
        shader->setVec4("color", color);
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
bool Object::isPointInside(double x, double y) const
{
    // Get object bounds in screen space
    glm::vec2 pos = transform->getPosition();
    glm::vec2 scale = transform->getScale();

    // Calculate half-width and half-height
    float halfWidth = scale.x / 2.0f;
    float halfHeight = scale.y / 2.0f;

    // Check if point is within axis-aligned bounding box
    // Note: This is simplified AABB collision, doesn't account for rotation
    float minX = pos.x - halfWidth;
    float maxX = pos.x + halfWidth;
    float minY = pos.y - halfHeight;
    float maxY = pos.y + halfHeight;

    return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

void Object::onClick()
{
    if (onClickCallback)
    {
        onClickCallback();
    }
}