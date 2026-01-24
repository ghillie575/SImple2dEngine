#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace engine
{

    class Transform
    {
    public:
        Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), float rotation = 0.0f, glm::vec2 scale = glm::vec2(1.0f, 1.0f)) : position(position), rotation(rotation), scale(scale), zIndex(0) {}
        glm::mat4 getModelMatrix() const;
        void setPosition(const glm::vec2 &pos) { position = pos; }
        glm::vec2 getPosition() const { return position; }
        void setRotation(float rot) { rotation = rot; }
        float getRotation() const { return rotation; }
        void setScale(const glm::vec2 &scl) { scale = scl; }
        glm::vec2 getScale() const { return scale; }
        void setZIndex(int z) { zIndex = z; }
        int getZIndex() const { return zIndex; }

    private:
        glm::vec2 position;
        float rotation;
        glm::vec2 scale;
        int zIndex;
    };
} // namespace engine