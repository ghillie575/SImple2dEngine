#include <input.hpp>
#include <window.hpp>
namespace engine
{
    bool Input::isMouseButtonPressed(int button) const
    {
        GLFWwindow *window = getActiveWindow()->getGLFWwindow();
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    bool Input::isKeyPressed(Key key)
    {
        GLFWwindow *window = getActiveWindow()->getGLFWwindow();
        int state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    void Input::windowInput(GLFWwindow *window)
    {
        // Handle keyboard input
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

        // Handle mouse input
        double newMouseX, newMouseY;
        glfwGetCursorPos(window, &newMouseX, &newMouseY);
        mouseX = newMouseX;
        mouseY = newMouseY;

        bool currentMouseState = isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        // Detect click (transition from not pressed to pressed)
        if (currentMouseState && !lastMouseState && mouseClickCallback_)
        {
            mouseClickCallback_(mouseX, mouseY);
        }
        // Detect release (transition from pressed to not pressed)
        if (!currentMouseState && lastMouseState && mouseReleaseCallback_)
        {
            mouseReleaseCallback_(mouseX, mouseY);
        }
        lastMouseState = currentMouseState;
    }
    void Input::setKeyPressedCallback(std::function<void(Key)> callback)
    {
        keyPressedCallback_ = callback;
    }
    void Input::setMouseClickCallback(std::function<void(double, double)> callback)
    {
        mouseClickCallback_ = callback;
    }

    void Input::setMouseReleaseCallback(std::function<void(double, double)> callback)
    {
        mouseReleaseCallback_ = callback;
    }
} // namespace engine