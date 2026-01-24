#include <iostream>
#include <window.hpp>
#include <shader.hpp>
#include <vbo.hpp>
#include <vao.hpp>
#include <shader_manager.hpp>
#include <object.hpp>
using namespace engine;

float vertices[] = {
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};  
Object *triangleObject;
void keyCallback(Key key)
{
    if (key == KEY_ESCAPE)
    {
        getActiveWindow()->close();
    }
}
void initCallback()
{
    triangleObject = new Object("base", "triangle", std::vector<float>(std::begin(vertices), std::end(vertices)));
    triangleObject->addVertexAttribute({3}); //position
    triangleObject->addVertexAttribute({3}); //color
    triangleObject->intialize();
}
void loopCallback()
{
    triangleObject->render();
}
int main(int, char **)
{
    std::cout << "Hello, from 2dengine!\n";
    Window window(800, 600, "2dengine Window");
    window.input.setKeyPressedCallback(keyCallback);
    window.setInitCallback(initCallback);
    window.setLoopCallback(loopCallback);
    window.loop();
    return 0;
}
