#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include <shader.hpp>
namespace engine
{
    struct Character
    {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        unsigned int Advance;
    };

    class TextBox
    {
    public:
        TextBox(const std::string &shaderName, const std::string &text, std::string font, float scale, glm::vec2 position) : text(text), scale(scale), position(position), font(font) {
                                                                                                                                           };
        void setText(const std::string &text);
        void render();
        void initialize();
        void RenderText(Shader& shader, std::string text,float scale, glm::vec3 color);
        glm::vec2 position;
    private:
        Shader* shader;
        unsigned int VAO, VBO;
        std::string text;
        std::string font;
        float scale;
        bool initialized = false;
        std::map<GLchar, Character> characters;
    };
}