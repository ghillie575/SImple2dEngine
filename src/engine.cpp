#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <engine.hpp>
#include <logger.hpp>
#include <resource_manager.hpp>
bool isOpenGLInitializedFlag = false;
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
    log("Terminating OpenGL");
    glfwTerminate();
    isOpenGLInitializedFlag = false;
}