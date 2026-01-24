#pragma once
#include <engine.hpp>
#include <vao.hpp>
#include <vbo.hpp>
#include <shader.hpp>
#include <shader_manager.hpp>
#include <resource_manager.hpp>
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
            : shaderName(shaderName), objectId(objectId), vao(), vbo(nullptr), shader(nullptr), vertexData(vertexData)
        {
        }
        ~Object()
        {
            if (vbo)
            {
                engine::getGlobalResourceManager()->removeVBO(vbo);
                delete vbo;
            }
            engine::getGlobalResourceManager()->removeVAO(&vao);
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
        void intialize();
        void render();
        void addVertexAttribute(const VertexAttribute &attribute)
        {
            vertexAttributes.push_back(attribute);
        }

    private:
        std::string shaderName;
        std::string objectId;
        VAO vao;
        VBO *vbo;
        Shader *shader;
        std::vector<float> vertexData;
        std::vector<VertexAttribute> vertexAttributes;
    };
} // namespace engine