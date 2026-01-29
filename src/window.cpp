#include <window.hpp>
#include <logger.hpp>
#include <engine.hpp>
#include <shader_manager.hpp>
namespace engine
{
    void Window::updateProjectionMatrix()
    {
        projectionMatrix_ = glm::ortho(0.0f, static_cast<float>(width_), 0.0f, static_cast<float>(height_));
    }
    Window::Window(int width, int height, const char *title)
    {
        if (!engine::isOpenGLInitialized())
        {
            initializeOpenGL();
            initializeFreeType();
            if (!engine::isOpenGLInitialized())
            {
                error("Could not initialize OpenGL before creating window");
                window_ = nullptr;
                return;
            }
        }
        log("Creating GLFW window: " + std::string(title));
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 16); // 16x MSAA

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
        glfwSwapInterval(1); // Enable V-Sync
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *win, int w, int h)
                                       {
                                                                                    Window *window = getActiveWindow();
                                                                                    if (window)
                                                                                    {
                                                                                        glViewport(0, 0, w, h);
                                                                                        window->setWidth(w);
                                                                                        window->setHeight(h);
                                                                                        window->updateProjectionMatrix();
                                                                                        if (window->framebufferSizeCallback_)
                                                                                        {
                                                                                            window->framebufferSizeCallback_(w, h);
                                                                                        }
                                                                                    } });
        log("Initializing GLAD");
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            error("Failed to initialize GLAD");
            terminateOpenGL();
            return;
        }
        glViewport(0, 0, width, height);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        width_ = width;
        height_ = height;
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
        updateProjectionMatrix();
        if (initCallback_)
        {
            initCallback_();
        }
        std::vector<Shader *> &shaders = shader_manager::getAllShaders();
        const float FIXED_TIMESTEP = 1.0f / 60.0f; // Physics at 60Hz
        float accumulator = 0.0f;
        float lastTime = glfwGetTime();

        while (!shouldClose())
        {
            float currentTime = glfwGetTime();
            float frameTime = currentTime - lastTime;
            lastTime = currentTime;

            if (frameTime > 0.25f)
                frameTime = 0.25f;

            input.windowInput(window_);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            time = currentTime;
            deltaTime = frameTime;

            for (auto shader : shaders)
            {
                shader->use();
                shader->setFloat("time", time);
                shader->setFloat("deltaTime", deltaTime);
                shader->setMat4("projection", projectionMatrix_);
            }

            if (loopCallback_)
            {
                loopCallback_();
            }

            glfwSwapBuffers(window_);
            glfwPollEvents();

            std::cout << "FPS: " << 1.0f / frameTime << "\r" << std::flush;
            deltaTime = frameTime;
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