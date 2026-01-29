#include <physics/physics.hpp>
#include <glm/glm.hpp>
#include <network/network.hpp>
#include <logger.hpp>
#include <thread>
#include <iostream>
#include <chrono>
using namespace engine;
PhysicsWorld physicsWorld;
StaticBody *groundBody;
StaticBody *leftWallBody;
StaticBody *rightWallBody;
StaticBody *topWallBody;
StaticBody *leftGateBody;
StaticBody *rightGateBody;
RigidBody *dynamicBody;
NetworkServer *server;
int scoreA = 0;
int scoreB = 0;
enum Key
{
    KEY_UNKNOWN = -1,
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,
    KEY_EQUAL = 61,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_BRACKET = 91,
    KEY_BACKSLASH = 92,
    KEY_RIGHT_BRACKET = 93,
    KEY_GRAVE_ACCENT = 96,
    KEY_WORLD_1 = 161,
    KEY_WORLD_2 = 162,
    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,
    KEY_F13 = 302,
    KEY_F14 = 303,
    KEY_F15 = 304,
    KEY_F16 = 305,
    KEY_F17 = 306,
    KEY_F18 = 307,
    KEY_F19 = 308,
    KEY_F20 = 309,
    KEY_F21 = 310,
    KEY_F22 = 311,
    KEY_F23 = 312,
    KEY_F24 = 313,
    KEY_F25 = 314,
    KEY_KP_0 = 320,
    KEY_KP_1 = 321,
    KEY_KP_2 = 322,
    KEY_KP_3 = 323,
    KEY_KP_4 = 324,
    KEY_KP_5 = 325,
    KEY_KP_6 = 326,
    KEY_KP_7 = 327,
    KEY_KP_8 = 328,
    KEY_KP_9 = 329,
    KEY_KP_DECIMAL = 330,
    KEY_KP_DIVIDE = 331,
    KEY_KP_MULTIPLY = 332,
    KEY_KP_SUBTRACT = 333,
    KEY_KP_ADD = 334,
    KEY_KP_ENTER = 335,
    KEY_KP_EQUAL = 336,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_MENU = 348,
};
struct PlayerInfo
{
    std::string name;
    RigidBody *body;
};
std::vector<PlayerInfo> players;
void updateServer()
{
    while (true)
    {
        server->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
void OnClientLeave(ServerClient *client)
{
    log("Client left: " + client->name);
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        if (it->name == client->name)
        {
            physicsWorld.removeRigidBody(it->body);
            players.erase(it);
            break;
        }
    }
}
void afterClientLeave(std::string name)
{
    server->sendObjectDestroy(name);
}
void onClientJoin(ServerClient *client)
{
    log("Client joined: " + client->name);

    server->sendObjectCreate({"box1", {0, 0}, {20, 20}, 0, "base"}, client);
    server->sendObjectCreate({"ground", {400, 0}, {5000, 50}, 0, "base"}, client);
    server->sendObjectCreate({"leftWall", {0, 400}, {50, 5000}, 0, "base"}, client);
    server->sendObjectCreate({"rightWall", {800, 400}, {50, 5000}, 0, "base"}, client);
    server->sendObjectCreate({"topWall", {400, 800}, {5000, 50}, 0, "base"}, client);
    server->sendObjectCreate({"leftGate", {60, 400}, {20, 100}, 0, "base"}, client);
    server->sendObjectCreate({"rightGate", {740, 400}, {20, 100}, 0, "base"}, client);
    server->sendScoreUpdate({scoreA, scoreB}, client);
    PlayerCreatePacket playerPacket;
    playerPacket.name = client->name;
    playerPacket.position = {400, 400};
    playerPacket.size = {50, 50};
    playerPacket.rotation = 0;
    playerPacket.shader = "base";
    server->sendPlayerCreate(playerPacket, client);
    server->sendPlayerCreate(playerPacket, nullptr);
    RigidBody *playerBody = new RigidBody(physicsWorld.getSpace(), glm::vec2(400, 400),
                                          0.3f, 50, 50, client->name, 0);
    playerBody->disableRotation();
    physicsWorld.addRigidBody(playerBody);
    players.push_back({client->name, playerBody});
    for (const auto &obj : players)
    {
        if (obj.name != client->name)
        {
            PlayerCreatePacket pkt;
            pkt.name = obj.name;
            pkt.position = obj.body->getPosition();
            pkt.size = {50, 50};
            pkt.rotation = 0;
            pkt.shader = "base";

            server->sendPlayerCreate(pkt, client);
        }
    }
}
void onKeyEvent(ServerClient *client, uint8_t key)
{
    for (const auto &player : players)
    {
        if (player.name == client->name)
        {
            if (key == KEY_W)
            {
                player.body->applyForce(glm::vec2(0, 200));
            }
            else if (key == KEY_S)
            {
                player.body->applyForce(glm::vec2(0, -200));
            }
            else if (key == KEY_A)
            {
                player.body->applyForce(glm::vec2(-200, 0));
            }
            else if (key == KEY_D)
            {
                player.body->applyForce(glm::vec2(200, 0));
            }
        }
    }
}
void OnCollision(void *a, void *b)
{
    std::string name1;
    std::string name2;
    RigidBody *bodyA = static_cast<RigidBody *>(static_cast<Body *>(a));
    RigidBody *bodyB = static_cast<RigidBody *>(static_cast<Body *>(b));
    if (!bodyA)
    {
        bodyA = nullptr;
        StaticBody *staticA = dynamic_cast<StaticBody *>(static_cast<Body *>(a));
        name1 = staticA->name;
    }
    else
    {
        name1 = bodyA->name;
    }
    if (!bodyB)
    {
        bodyB = nullptr;
        StaticBody *staticB = dynamic_cast<StaticBody *>(static_cast<Body *>(b));
        name2 = staticB->name;
    }
    else
    {
        name2 = bodyB->name;
    }
    if (name1 == "leftGate" || name2 == "leftGate")
    {
        if (name1 == "box1" || name2 == "box1")
        {
            dynamicBody->setPosition(glm::vec2(400, 400));
            dynamicBody->resetVelocity();
            scoreA++;
            log("Score A: " + std::to_string(scoreA) + " | Score B: " + std::to_string(scoreB));
            ScoreUpdatePacket pkt;
            pkt.scoreA = scoreA;
            pkt.scoreB = scoreB;
            server->sendScoreUpdate(pkt);
        }
    }
    if (name1 == "rightGate" || name2 == "rightGate")
    {
        if (name1 == "box1" || name2 == "box1")
        {
            dynamicBody->setPosition(glm::vec2(400, 400));
            dynamicBody->resetVelocity();
            scoreB++;
            log("Score A: " + std::to_string(scoreA) + " | Score B: " + std::to_string(scoreB));
            ScoreUpdatePacket pkt;
            pkt.scoreA = scoreA;
            pkt.scoreB = scoreB;
            server->sendScoreUpdate(pkt);
        }
    }
}
int main(int, char **)
{

    log("Initializing Server Physics World");
    physicsWorld = PhysicsWorld();
    physicsWorld.init(0);

    groundBody = new StaticBody(physicsWorld.getSpace(),
                                5000, 50, glm::vec2(400, 0), "ground", 100);
    physicsWorld.addStaticBody(groundBody);
    leftWallBody = new StaticBody(physicsWorld.getSpace(),
                                  50, 5000, glm::vec2(0, 400), "leftWall", 100);
    physicsWorld.addStaticBody(leftWallBody);
    rightWallBody = new StaticBody(physicsWorld.getSpace(),
                                   50, 5000, glm::vec2(800, 400), "rightWall", 100);
    physicsWorld.addStaticBody(rightWallBody);
    topWallBody = new StaticBody(physicsWorld.getSpace(),
                                 5000, 50, glm::vec2(400, 800), "topWall", 100);
    physicsWorld.addStaticBody(topWallBody);
    dynamicBody = new RigidBody(physicsWorld.getSpace(), glm::vec2(200, 300),
                                0.3f, 20, 20, "box1", 100);
    physicsWorld.addRigidBody(dynamicBody);
    leftGateBody = new StaticBody(physicsWorld.getSpace(),
                                  20, 100, glm::vec2(60, 400), "leftGate");
    physicsWorld.addStaticBody(leftGateBody);
    rightGateBody = new StaticBody(physicsWorld.getSpace(),
                                   20, 100, glm::vec2(740, 400), "rightGate");
    physicsWorld.addStaticBody(rightGateBody);
    physicsWorld.setCollisionCallback(OnCollision);
    log("Starting Server");
    server = new NetworkServer(7680);
    server->setOnClientJoinCallback(onClientJoin);
    server->setOnClientLeaveCallback(OnClientLeave);
    server->setAfterClientLeave(afterClientLeave);
    server->setKeyEventCallback(onKeyEvent);
    std::thread serverThread(updateServer);
    log("Server started successfully.");
    log("Entering main loop.");
    for (;;)
    {
        physicsWorld.step(1.0f / 60.0f);
        server->sendPositionUpdate({dynamicBody->getPosition(), static_cast<float>(dynamicBody->getRotation() * 180.0f / M_PI), "box1"});
        for (const auto &player : players)
        {
            server->sendPositionUpdate({player.body->getPosition(), static_cast<float>(player.body->getRotation() * 180.0f / M_PI), player.name});
        }
        enet_host_flush(server->server);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
