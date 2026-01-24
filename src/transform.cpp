#include <transform.hpp>
#include <window.hpp>
using namespace engine;
glm::mat4 Transform::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 center = glm::vec2(getActiveWindow()->getWidth() / 2.0f, getActiveWindow()->getHeight() / 2.0f);
    model = glm::translate(model, glm::vec3(center + position, static_cast<float>(zIndex)));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    return model;
}