#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <input.hpp>
#include <functional>
namespace engine
{
    class Window
    {
    public:
        Input input;
        Window(int width, int height, const char *title);
        ~Window();
        void setFramebufferSizeCallback(std::function<void(int, int)> callback);
        void setLoopCallback(std::function<void()> callback);
        void setInitCallback(std::function<void()> callback);
        void loop();

        GLFWwindow *getGLFWwindow() const;
        bool shouldClose() const;
        void close();

    private:
        GLFWwindow *window_;
        std::function<void(int, int)> framebufferSizeCallback_;
        std::function<void()> loopCallback_;
        std::function<void()> initCallback_;
    };
    void setActiveWindow(Window *window);
    Window *getActiveWindow();
} // namespace engine