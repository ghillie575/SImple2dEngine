#include <shader_manager.hpp>
#include <filesystem>
#include <vector>
#include <logger.hpp>
namespace engine
{
    namespace shader_manager
    {
        std::vector<Shader *> shaders;
        void loadAllShaders()
        {
            log("Compiling all shaders...");
            std::filesystem::path shaderDir = "shaders/";
            for (const auto &entry : std::filesystem::directory_iterator(shaderDir))
            {
                if (entry.is_regular_file())
                {
                    std::string path = entry.path().string();
                    if (path.size() >= 5 && path.substr(path.size() - 5) == ".vert")
                    {
                        std::string fragmentPath = path.substr(0, path.size() - 5) + ".frag";
                        if (std::filesystem::exists(fragmentPath))
                        {
                            log("Compiling: " + path + " and " + fragmentPath);
                            Shader *shader = new Shader(path.c_str(), fragmentPath.c_str());
                            shader->name = std::filesystem::path(path).stem().string();
                            shaders.push_back(shader);
                        }
                        else
                        {
                            error("Fragment shader not found for vertex shader: " + path);
                        }
                    }
                }
            }
        }

        Shader *getShader(const std::string &name)
        {
            for (auto shader : shaders)
            {
                if (shader->name == name)
                {
                    return shader;
                }
            }
            return nullptr;
        }
        void clearAllShaders()
        {
            for (auto shader : shaders)
            {
                if (shader)
                    delete shader;
            }
            shaders.clear();
        }
    } // namespace shader_manager
} // namespace engine