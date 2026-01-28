#pragma once
#include <engine.hpp>
#include <vao.hpp>
#include <vbo.hpp>
#include <ebo.hpp>
#include <shader.hpp>
#include <shader_manager.hpp>
#include <resource_manager.hpp>
#include <texture.hpp>
#include <transform.hpp>
#include <functional>
#include <glm/glm.hpp>
#include <vector>
#include <string>
namespace engine
{
 
    struct VertexAttribute
    {
        int size;
    };
    class Object
    {
    public:
        Object(const std::string &shaderName, const std::string &objectId, const std::vector<float> &vertexData)
            : shaderName(shaderName), objectId(objectId), vao(), vbo(new VBO(vertexData)), shader(nullptr), vertexData(vertexData), transform(new Transform())
        {
        }
        Object(const std::string &shaderName, const std::string &objectId, const std::vector<float> &vertexData, const std::vector<unsigned int> &indices)
            : shaderName(shaderName), objectId(objectId), vao(), vbo(new VBO(vertexData)), ebo(new EBO(indices)), shader(nullptr), vertexData(vertexData), indices(indices), transform(new Transform())
        {
            hasEBO = true;
        }
        ~Object()
        {
            if (vbo)
            {
                engine::getGlobalResourceManager()->removeVBO(vbo);
                delete vbo;
            }
            if (hasEBO && ebo)
            {
                engine::getGlobalResourceManager()->removeEBO(ebo);
                delete ebo;
            }
            engine::getGlobalResourceManager()->removeVAO(&vao);
            delete transform;
        }
        VAO &getVAO()
        {
            return vao;
        }
        VBO *getVBO()
        {
            return vbo;
        }
        Shader *getShader()
        {
            return shader;
        }
        std::string getObjectId() const
        {
            return objectId;
        }
        void setTexture(std::string texturePath);
        void intialize();
        void render();
        void setAnchorPoint(AnchorPoint point)
        {
            anchorPoint = point;
        }
        AnchorPoint getAnchorPoint() const
        {
            return anchorPoint;
        }
        void addVertexAttribute(const VertexAttribute &attribute)
        {
            vertexAttributes.push_back(attribute);
        }
        Transform *transform;
        glm::vec4 color;
        
    // Click handling
    void setOnClickCallback(std::function<void()> callback) { onClickCallback = callback; }
    bool isPointInside(double x, double y) const;
    void onClick();

    private:
        std::string shaderName;
        std::string objectId;
        VAO vao;
        VBO *vbo;
        EBO *ebo;
        Texture *texture;
        bool hasEBO = false;
        bool hasTexture = false;
        Shader *shader;
        std::vector<float> vertexData;
        std::vector<unsigned int> indices;
        std::vector<VertexAttribute> vertexAttributes;
        AnchorPoint anchorPoint = TOP_LEFT;
        std::function<void()> onClickCallback;
    };
} // namespace engine