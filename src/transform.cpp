#include <transform.hpp>
#include <window.hpp>
using namespace engine;
glm::vec2 Transform::getRawPosition(AnchorPoint anchorPoint) const
{
    glm::vec2 anchorPointOffset(0.0f, 0.0f);
    int windowWidth = engine::getActiveWindow()->getWidth();
    int windowHeight = engine::getActiveWindow()->getHeight();
    bool hasAnchorPoint = true;
    switch (anchorPoint)
    {
    case TOP_CENTER:
        anchorPointOffset = glm::vec2(windowWidth / 2, windowHeight);
        break;
    case TOP_RIGHT:
        anchorPointOffset = glm::vec2(windowWidth, windowHeight);
        break;
    case TOP_LEFT:
        anchorPointOffset = glm::vec2(0.0f, windowHeight);
        break;
    case CENTER:
        anchorPointOffset = glm::vec2(windowWidth / 2, windowHeight / 2);
        break;
    case BOTTOM_LEFT:
        anchorPointOffset = glm::vec2(0.0f, 0.0f);
        break;
    case BOTTOM_CENTER:
        anchorPointOffset = glm::vec2(windowWidth / 2, 0.0f);
        break;
    case BOTTOM_RIGHT:
        anchorPointOffset = glm::vec2(windowWidth, 0.0f);
        break;
    case CENTER_LEFT:
        anchorPointOffset = glm::vec2(0.0f, windowHeight / 2);
        break;
    case CENTER_RIGHT:
        anchorPointOffset = glm::vec2(windowWidth, windowHeight / 2);
        break;
    case NONE:
        hasAnchorPoint = false;
        break;
    default:
        hasAnchorPoint = false;
        break;
    }
    if (!hasAnchorPoint)
        return position;
    return anchorPointOffset + position;
}
glm::mat4 Transform::getModelMatrix(AnchorPoint anchorPoint) const
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 anchorPointOffset(0.0f, 0.0f);
    int windowWidth = engine::getActiveWindow()->getWidth();
    int windowHeight = engine::getActiveWindow()->getHeight();
    bool hasAnchorPoint = true;
    switch (anchorPoint)
    {
    case TOP_CENTER:
        anchorPointOffset = glm::vec2(windowWidth / 2, windowHeight);
        break;
    case TOP_RIGHT:
        anchorPointOffset = glm::vec2(windowWidth, windowHeight);
        break;
    case TOP_LEFT:
        anchorPointOffset = glm::vec2(0.0f, windowHeight);
        break;
    case CENTER:
        anchorPointOffset = glm::vec2(windowWidth / 2, windowHeight / 2);
        break;
    case BOTTOM_LEFT:
        anchorPointOffset = glm::vec2(0.0f, 0.0f);
        break;
    case BOTTOM_CENTER:
        anchorPointOffset = glm::vec2(windowWidth / 2, 0.0f);
        break;
    case BOTTOM_RIGHT:
        anchorPointOffset = glm::vec2(windowWidth, 0.0f);
        break;
    case CENTER_LEFT:
        anchorPointOffset = glm::vec2(0.0f, windowHeight / 2);
        break;
    case CENTER_RIGHT:
        anchorPointOffset = glm::vec2(windowWidth, windowHeight / 2);
        break;
    case NONE:
        hasAnchorPoint = false;
        break;
    default:
        hasAnchorPoint = false;
        break;
    }
    if (!hasAnchorPoint)
    {
        model = glm::translate(model, glm::vec3(anchorPointOffset + position, static_cast<float>(zIndex)));
    }
    else
    {
        model = glm::translate(model, glm::vec3(position, static_cast<float>(zIndex)));
    }
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    return model;
}