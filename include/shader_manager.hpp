#pragma once
#include <shader.hpp>
#include <string>
#include <vector>
namespace engine
{
    namespace shader_manager
    {
        void loadAllShaders();
        Shader *getShader(const std::string &name);
        void clearAllShaders();
        std::vector<Shader *> &getAllShaders();
    } // namespace shader_manager
}