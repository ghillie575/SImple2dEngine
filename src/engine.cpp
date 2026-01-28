#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <engine.hpp>
#include <logger.hpp>
#include <resource_manager.hpp>
FT_Library ftLibrary;
bool isOpenGLInitializedFlag = false;
void engine::initializeFreeType()
{
    log("Initializing FreeType library");
    if (FT_Init_FreeType(&ftLibrary))
    {
        error("Could not initialize FreeType Library");
    }
}
FT_Library &engine::getFreeTypeLibrary()
{
    return ftLibrary;
}
bool engine::isOpenGLInitialized()
{
    return isOpenGLInitializedFlag;
}
void engine::initializeOpenGL(int major, int minor)
{
    log("Initializing OpenGL with version " + std::to_string(major) + "." + std::to_string(minor));
    if(!glfwInit()) {
        error("Failed to initialize GLFW");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    isOpenGLInitializedFlag = true;
}
void engine::terminateOpenGL()
{
    log("Cleaning up resources");
    getGlobalResourceManager()->deleteAll();
    FT_Done_FreeType(ftLibrary);
    log("Terminating OpenGL");
    glfwTerminate();
    isOpenGLInitializedFlag = false;
}