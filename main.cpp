#include <iostream>
#include <window.hpp>
#include <input.hpp>
#include <network_scene.hpp>

using namespace engine;

// Global scene instance
Scene *g_scene = nullptr;

void onKeyPressed(Key key)
{
    if (key == KEY_ESCAPE)
    {
        getActiveWindow()->close();
    }
    g_scene->onKeyPress(key);

}

void onWindowInit()
{
}

void onWindowLoop()
{
    if (g_scene)
    {
        g_scene->render();
    }
}

void onWindowResize(int width, int height)
{
    if (g_scene)
    {
        g_scene->onWindowResize(width, height);
    }
}
void onMouseClick(double x, double y)
{
    if (g_scene)
    {
        g_scene->onMouseClick(x, y);
    }
}

void onMouseRelease(double x, double y)
{
    if (g_scene)
    {
        g_scene->onMouseRelease();
    }
}
int main(int, char **)
{
    std::cout << "Starting 2D Engine\n";

    Window window(800, 800, "2D Engine");

    // Set up callbacks
    window.input.setKeyPressedCallback(onKeyPressed);
    window.setInitCallback(onWindowInit);
    window.setLoopCallback(onWindowLoop);
    window.setFramebufferSizeCallback(onWindowResize);
    window.input.setMouseClickCallback(onMouseClick);
    window.input.setMouseReleaseCallback(onMouseRelease);
    g_scene = new Scene();
    g_scene->initialize();
    // Run window loop
    window.loop();

    // Cleanup
    if (g_scene)
    {
        delete g_scene;
        g_scene = nullptr;
    }

    return 0;
}
