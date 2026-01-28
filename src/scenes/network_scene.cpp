#include <network_scene.hpp>
#include <logger.hpp>
#include <window.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <network/network.hpp>
#include <textbox.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
using namespace engine;
bool connected = false;
std::mutex objectQueueMutex;
std::queue<std::function<void()>> pendingObjectCreations;
NetworkClient *client;
std::string currentPlayerName;
std::map<std::string, Object *> objectMap;
std::map<std::string, TextBox *> playerNames;
TextBox *infoBox;
std::vector<float> vertices = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};
std::vector<unsigned int> indices = {
    0, 1, 3,
    1, 2, 3};
Scene::Scene() : windowWidth(800), windowHeight(800)
{
}
Scene::~Scene()
{
    for (auto obj : objectMap)
    {
        delete obj.second;
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
                                    objectMap[pkt.name] = obj;
                                    obj->intialize(); });
}
void OnObjectDestroy(const ObjectDestroyPacket &pkt)
{
    log("Received object destroy packet for object: " + pkt.name);
    std::lock_guard<std::mutex> lock(objectQueueMutex);
    pendingObjectCreations.push([pkt]()
                                {
                                    auto it = objectMap.find(pkt.name);
                                    if (it != objectMap.end())
                                    {
                                        Object *obj = it->second;
                                        objectMap.erase(it);
                                        delete obj;
                                    } });
}
void OnPositionUpdate(const NetPositionPacket &pkt)
{
    for (auto obj : objectMap)
    {
        if (obj.second->getObjectId() == pkt.objectName)
        {
            obj.second->transform->setPosition(pkt.position);
            obj.second->transform->setRotation(pkt.rotation);
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
                                    objectMap[pkt.name] = obj;
                                    obj->intialize(); 
                                TextBox *nameTag = new TextBox("text", pkt.name, "base.ttf", 0.3f, glm::vec2(400.0f, 400.0f));
        nameTag->initialize();
        playerNames[pkt.name] = nameTag; });
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
                                    objectMap[pkt.name] = obj;
                                    obj->intialize(); 
                                TextBox *nameTag = new TextBox("text", pkt.name, "base.ttf", 0.3f, glm::vec2(400.0f, 400.0f));
        nameTag->initialize();
        playerNames[pkt.name] = nameTag; });
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
    infoBox = new TextBox("text", "Connecting to server...", "base.ttf", 0.5f, glm::vec2(10.0f, windowHeight - 50.0f));
    infoBox->initialize();
    log("Connecting to server...");
    client = new NetworkClient("localhost", 7680);
    client->onObjectCreate(OnObjectCreate);
    client->onObjectDestroy(OnObjectDestroy);
    client->onPositionUpdate(OnPositionUpdate);
    client->setPlayerCreateCallback(OnPlayerCreate);
    std::thread([this]()
                {
        try
        {
            client->connect();
            log("Connected to server.");
            infoBox->setText("Connected to server.");
            connected = true;
            updateClient();
        }
        catch (const std::exception &e)
        {
            log(std::string("Failed to connect to server: ") + e.what());
            infoBox->setText("Failed to connect to server: " + std::string(e.what()));
        } })
        .detach();
}
void Scene::render()
{
    std::lock_guard<std::mutex> lock(objectQueueMutex);
    while (!pendingObjectCreations.empty())
    {
        pendingObjectCreations.front()();
        pendingObjectCreations.pop();
    }
    for (auto obj : objectMap)
    {

        obj.second->render();
        auto it = playerNames.find(obj.first);
        if (it != playerNames.end() && it->second)
        {
            it->second->position = glm::vec2(obj.second->transform->getPosition().x, obj.second->transform->getPosition().y + 1.0f);
            it->second->render();
        }
    }
    if (!connected)
    {
        infoBox->render();
    }
    else
    {
        infoBox->setText("FPS: " + std::to_string(static_cast<int>(1.0 / getActiveWindow()->deltaTime)));
        infoBox->render();
    }
}
void Scene::onWindowResize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    infoBox->position = glm::vec2(10.0f, windowHeight - 50.0f);
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