#include <window.hpp>
#include <logger.hpp>
#include <engine.hpp>
#include <shader_manager.hpp>
namespace engine
{
    Window::Window(int width, int height, const char *title)
    {
        if (!engine::isOpenGLInitialized())
        {
            initializeOpenGL();
            if (!engine::isOpenGLInitialized())
            {
                error("Could not initialize OpenGL before creating window");
                window_ = nullptr;
                return;
            }
        }
        log("Creating GLFW window: " + std::string(title));
        GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL)
        {
            error("Failed to create GLFW window");
            window_ = nullptr;
            terminateOpenGL();
            return;
        }
        window_ = window;
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *win, int w, int h)
                                       {
                                          Window *window = getActiveWindow();
                                          if (window && window->framebufferSizeCallback_)
                                          {
                                              window->framebufferSizeCallback_(w, h);
                                          } });
        log("Initializing GLAD");
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            error("Failed to initialize GLAD");
            terminateOpenGL();
            return;
        }
        setActiveWindow(this);
        log("Loading shaders");
        shader_manager::loadAllShaders();
    }
    Window::~Window()
    {
        log("Destroying GLFW window");
        if (window_)
        {
            glfwDestroyWindow(window_);
        }
        terminateOpenGL();
    }
    void Window::setFramebufferSizeCallback(std::function<void(int, int)> callback)
    {
        framebufferSizeCallback_ = callback;
    }
    void Window::setLoopCallback(std::function<void()> callback)
    {
        loopCallback_ = callback;
    }
    void Window::setInitCallback(std::function<void()> callback)
    {
        initCallback_ = callback;
    }
    void Window::close()
    {
        if (window_)
        {
            glfwSetWindowShouldClose(window_, true);
        }
    }
    void Window::loop()
    {
        log("Starting window loop");
        if (initCallback_)
        {
            initCallback_();
        }
        while (!shouldClose())
        {
            input.windowInput(window_);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            if (loopCallback_)
            {
                loopCallback_();
            }
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
    }
    GLFWwindow *Window::getGLFWwindow() const
    {
        return window_;
    }
    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(window_);
    }
    static Window *activeWindow = nullptr;
    void setActiveWindow(Window *window)
    {
        activeWindow = window;
    }
    Window *getActiveWindow()
    {
        return activeWindow;
    }
} // namespace engine