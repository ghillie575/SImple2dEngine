#include <iostream>
#include <window.hpp>
#include <shader.hpp>
#include <vbo.hpp>
#include <vao.hpp>
#include <shader_manager.hpp>
#include <object.hpp>
#include <random>
using namespace engine;

/*float vertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};*/
float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3};
Object *cubeObject;
bool cubeMoving = false;
bool movingBackward = false;
bool tabPressed = false;
void keyCallback(Key key)
{
    if (key == KEY_ESCAPE)
    {
        getActiveWindow()->close();
    }
    if (key == KEY_TAB)
    {
        if (!tabPressed)
        {
            cubeMoving = !cubeMoving;
            log("Cube moving: " + std::string(cubeMoving ? "true" : "false"));
            tabPressed = true;
        }
    }
    else
    {
        tabPressed = false;
    }
}
void initCallback()
{
    // cubeObject = new Object("base-tex", "cube", std::vector<float>(std::begin(vertices), std::end(vertices)), std::vector<unsigned int>(std::begin(indices), std::end(indices)));
    cubeObject = new Object("base", "cube", std::vector<float>(std::begin(vertices), std::end(vertices)), std::vector<unsigned int>(std::begin(indices), std::end(indices)));
    // cubeObject->setTexture("textures/box.jpg");
    cubeObject->transform->setRotation(0.0f);
    cubeObject->transform->setScale(glm::vec2(10.0f, 10.0f));
    cubeObject->transform->setPosition(glm::vec2(100.0f, 30.0f));
    cubeObject->addVertexAttribute({3}); // position
    // cubeObject->addVertexAttribute({2}); // texCoord
    cubeObject->intialize();
}
int randomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
void loopCallback()
{
    if (cubeMoving)
    {
        float newX;
        float newY;
        if (movingBackward)
        {
            newX = cubeObject->transform->getPosition().x - 100 * getActiveWindow()->deltaTime;
        }
        else
        {
            newX = cubeObject->transform->getPosition().x + 100 * getActiveWindow()->deltaTime;
        }
        if (newX + cubeObject->transform->getScale().x / 2 > getActiveWindow()->getWidth() / 2.0f){\
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            movingBackward = true;
        }
        if (newX - cubeObject->transform->getScale().x / 2 < -getActiveWindow()->getWidth() / 2.0f){
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            movingBackward = false;
        }
        newY = cubeObject->transform->getPosition().y - randomInt(-5, 5);
        if(newY + cubeObject->transform->getScale().y / 2 > getActiveWindow()->getHeight() / 2.0f){
            newY = getActiveWindow()->getHeight() / 2.0f - cubeObject->transform->getScale().y / 2;
        }
        if(newY - cubeObject->transform->getScale().y / 2 < -getActiveWindow()->getHeight() / 2.0f){
            newY = -getActiveWindow()->getHeight() / 2.0f + cubeObject->transform->getScale().y / 2;
        }
        cubeObject->transform->setPosition(glm::vec2(newX, newY));
    }
    cubeObject->render();
}
int main(int, char **)
{
    std::cout << "Hello, from 2dengine!\n";
    Window window(512, 512, "2dengine Window");
    window.input.setKeyPressedCallback(keyCallback);
    window.setInitCallback(initCallback);
    window.setLoopCallback(loopCallback);
    window.loop();
    return 0;
}
