#include <input.hpp>
#include <window.hpp>
namespace engine
{
    bool Input::isKeyPressed(Key key)
    {
        GLFWwindow *window = getActiveWindow()->getGLFWwindow();
        int state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    void Input::windowInput(GLFWwindow *window)
    {
        if (keyPressedCallback_)
        {
            for (int k = KEY_UNKNOWN; k <= KEY_MENU; ++k)
            {
                Key key = static_cast<Key>(k);
                if (isKeyPressed(key))
                {
                    keyPressedCallback_(key);
                }
            }
        }
    }
    void Input::setKeyPressedCallback(std::function<void(Key)> callback)
    {
        keyPressedCallback_ = callback;
    }
} // namespace engine