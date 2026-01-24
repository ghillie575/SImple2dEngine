#pragma once
#include <shader.hpp>
namespace engine
{
    namespace shader_manager
    {
        void loadAllShaders();
        Shader *getShader(const std::string &name);
        void clearAllShaders();
    } // namespace shader_manager
}