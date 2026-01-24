#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <input.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
namespace engine
{
    class Window
    {
    public:
        Input input;
        float deltaTime;
        float time;
        Window(int width, int height, const char *title);
        ~Window();
        void setFramebufferSizeCallback(std::function<void(int, int)> callback);
        void setLoopCallback(std::function<void()> callback);
        void setInitCallback(std::function<void()> callback);
        void loop();
        void updateProjectionMatrix();
        GLFWwindow *getGLFWwindow() const;
        bool shouldClose() const;
        void close();
        void setWindowSize(int width, int height){
            glfwSetWindowSize(window_, width, height);
            setWidth(width);
            setHeight(height);
            updateProjectionMatrix();
        }
        int getWidth() const
        {
            return width_;
        }
        int getHeight() const
        {
            return height_;
        }
        glm::mat4 getProjectionMatrix() const
        {
            return projectionMatrix_;
        }
        
        void setWidth(int width)
        {
            width_ = width;
        }
        void setHeight(int height)
        {
            height_ = height;
        }
        
    private:
        GLFWwindow *window_;
        std::function<void(int, int)> framebufferSizeCallback_;
        std::function<void()> loopCallback_;
        std::function<void()> initCallback_;
        glm::mat4 projectionMatrix_;
        int width_;
        int height_;
    };
    void setActiveWindow(Window *window);
    Window *getActiveWindow();
} // namespace engine