#include <demo_scene.hpp>
#include <logger.hpp>
#include <window.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <network/network.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
using namespace engine;
std::mutex objectQueueMutex;
std::queue<std::function<void()>> pendingObjectCreations;
NetworkClient *client;
std::string currentPlayerName;
std::vector<Object *> objects;
std::vector<float> vertices = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};
std::vector<unsigned int> indices = {
    0, 1, 3,
    1, 2, 3};
Scene::Scene() : windowWidth(800), windowHeight(600)
{
}
Scene::~Scene()
{
    for (auto obj : objects)
    {
        delete obj;
    }
}
glm::vec4 randomColor()
{
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return glm::vec4(r, g, b, 1.0f);
}
void OnObjectCreate(const ObjectCreatePacket &pkt)
{
    log("Received object create packet for object: " + pkt.name);
    std::lock_guard<std::mutex> lock(objectQueueMutex);
    pendingObjectCreations.push([pkt]()
                                {
                                    Object *obj = new Object(pkt.shader, pkt.name, vertices, indices);
                                    obj->addVertexAttribute({3});
                                    obj->transform->setPosition(pkt.position);
                                    obj->transform->setScale(pkt.size);
                                    obj->transform->setRotation(pkt.rotation);
                                    obj->color = randomColor();
                                    obj->setAnchorPoint(NONE);
                                    objects.push_back(obj);
                                    obj->intialize(); });
}
void OnObjectDestroy(const ObjectDestroyPacket &pkt)
{
    log("Received object destroy packet for object: " + pkt.name);
    std::lock_guard<std::mutex> lock(objectQueueMutex);
    pendingObjectCreations.push([pkt]()
                                {
                                    auto it = std::find_if(objects.begin(), objects.end(), [&pkt](Object *obj)
                                                           { return obj->getObjectId() == pkt.name; });
                                    if (it != objects.end())
                                    {
                                        delete *it;
                                        objects.erase(it);
                                    } });
}
void OnPositionUpdate(const NetPositionPacket &pkt)
{
    for (auto obj : objects)
    {
        if (obj->getObjectId() == pkt.objectName)
        {
            obj->transform->setPosition(pkt.position);
            obj->transform->setRotation(pkt.rotation);
            break;
        }
    }
}
void OnPlayerCreate(const PlayerCreatePacket &pkt, bool alreadyCreated)
{
    log("Received player create packet for player: " + pkt.name);
    log("Current player name: " + currentPlayerName);
    if (!alreadyCreated)
    {
        std::lock_guard<std::mutex> lock(objectQueueMutex);
        pendingObjectCreations.push([pkt]()
                                    {
                                    Object *obj = new Object(pkt.shader, pkt.name, vertices, indices);
                                    obj->addVertexAttribute({3});
                                    obj->transform->setPosition(pkt.position);
                                    obj->transform->setScale(pkt.size);
                                    obj->transform->setRotation(pkt.rotation);
                                    obj->color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green for players
                                    obj->setAnchorPoint(NONE);
                                    objects.push_back(obj);
                                    obj->intialize(); });
        currentPlayerName = pkt.name;
    }
    else
    {
        if (pkt.name == currentPlayerName)
        {
            return;
        }
        std::lock_guard<std::mutex> lock(objectQueueMutex);
        pendingObjectCreations.push([pkt]()
                                    {
                                    Object *obj = new Object(pkt.shader, pkt.name, vertices, indices);
                                    obj->addVertexAttribute({3});
                                    obj->transform->setPosition(pkt.position);
                                    obj->transform->setScale(pkt.size);
                                    obj->transform->setRotation(pkt.rotation);
                                    obj->color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue for another players
                                    obj->setAnchorPoint(NONE);
                                    objects.push_back(obj);
                                    obj->intialize(); });
    }
}
void updateClient()
{
    while (getActiveWindow() && !getActiveWindow()->shouldClose())
    {
        client->update();
    }
}
void Scene::initialize()
{
    log("Connecting to server...");
    client = new NetworkClient("localhost", 7680);
    client->onObjectCreate(OnObjectCreate);
    client->onObjectDestroy(OnObjectDestroy);
    client->onPositionUpdate(OnPositionUpdate);
    client->setPlayerCreateCallback(OnPlayerCreate);
    try
    {
        client->connect();
    }
    catch (const std::exception &e)
    {
        log(std::string("Failed to connect to server: ") + e.what());
        return;
    }
    std::thread clientThread(updateClient);
    clientThread.detach();
    log("Connected to server.");
}
void Scene::render()
{
    std::lock_guard<std::mutex> lock(objectQueueMutex);
    while (!pendingObjectCreations.empty())
    {
        pendingObjectCreations.front()();
        pendingObjectCreations.pop();
    }
    for (auto obj : objects)
    {
        obj->render();
    }
}
void Scene::onWindowResize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}
void Scene::onMouseClick(double x, double y)
{
}
void Scene::onMouseMove(double x, double y)
{
}
void Scene::onMouseRelease()
{
}
void Scene::onKeyPress(Key key)
{
    if (client)
    {
        client->sendKeyEvent(static_cast<uint8_t>(key));
    }
}