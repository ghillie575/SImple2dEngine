#include <resource_manager.hpp>
#include <shader_manager.hpp>
#include <logger.hpp>
using namespace engine;
void ResourceManager::registerVAO(VAO *vao)
{
    vaos.push_back(vao);
}
void ResourceManager::registerVBO(VBO *vbo)
{
    vbos.push_back(vbo);
}
void ResourceManager::removeVAO(VAO *vao)
{
    for (auto it = vaos.begin(); it != vaos.end(); ++it)
    {
        if (*it == vao)
        {
            if (*it)
                (*it)->free();
            vaos.erase(it);
            break;
        }
    }
}
void ResourceManager::removeVBO(VBO *vbo)
{
    for (auto it = vbos.begin(); it != vbos.end(); ++it)
    {
        if (*it == vbo)
        {
            if (*it)
                (*it)->free();
            vbos.erase(it);
            break;
        }
    }
}
void ResourceManager::deleteAll()
{
    for (auto vao : vaos)
    {
        if (vao)
            vao->free();
    }
    vaos.clear();
    for (auto vbo : vbos)
    {
        if (vbo)
            vbo->free();
    }
    vbos.clear();
    for (auto ebo : ebos)
    {
        if (ebo)
            ebo->free();
    }
    ebos.clear();
    shader_manager::clearAllShaders();
}
void ResourceManager::registerEBO(EBO *ebo)
{
    ebos.push_back(ebo);
}
void ResourceManager::removeEBO(EBO *ebo)
{
    for (auto it = ebos.begin(); it != ebos.end(); ++it)
    {
        if (*it == ebo)
        {
            if (*it)
                (*it)->free();
            ebos.erase(it);
            break;
        }
    }
}
ResourceManager *globalResourceManager = new ResourceManager();
ResourceManager *engine::getGlobalResourceManager()
{
    return globalResourceManager;
}