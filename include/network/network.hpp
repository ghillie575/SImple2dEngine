#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <enet/enet.h>
#include <mutex>

enum class PacketType : uint8_t
{
    PositionPacket = 1,
    JoinPacket = 2,
    KeyPacket = 3,
    LeavePacket = 4,
    ObjectTexturedPacket = 5,
    ObjectCreatePacket = 6,
    ObjectDestroyPacket = 7,
    PlayerCreatePacket = 8,
    ScoreUpdatePacket = 9
};

struct NetPositionPacket
{
    glm::vec2 position;
    float rotation;
    std::string objectName;
};
struct PlayerCreatePacket
{
    std::string name;
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    std::string shader;
};

struct NetKeyPacket
{
    uint8_t key;
};
struct ScoreUpdatePacket
{
    int scoreA;
    int scoreB;
};

struct NetJoinPacket
{
    std::string name;
};

struct NetLeavePacket
{
    std::string name;
};

struct ObjectCreatePacket
{
    std::string name;
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    std::string shader;
};

struct ObjectDestroyPacket
{
    std::string name;
};

struct ObjectTexturedPacket
{
    ObjectCreatePacket base;
    std::string texturePath;
};

struct PacketWriter
{
    std::vector<uint8_t> data;

    template <typename T>
    void write(const T &value)
    {
        size_t offset = data.size();
        data.resize(offset + sizeof(T));
        std::memcpy(data.data() + offset, &value, sizeof(T));
    }

    void writeString(const std::string &str)
    {
        uint16_t len = static_cast<uint16_t>(str.size());
        write(len);
        data.insert(data.end(), str.begin(), str.end());
    }
};

struct PacketReader
{
    const uint8_t *data;
    size_t offset = 0;

    PacketReader(const uint8_t *d) : data(d) {}

    template <typename T>
    T read()
    {
        T value;
        std::memcpy(&value, data + offset, sizeof(T));
        offset += sizeof(T);
        return value;
    }

    std::string readString()
    {
        uint16_t len = read<uint16_t>();
        std::string s(reinterpret_cast<const char *>(data + offset), len);
        offset += len;
        return s;
    }
};

class ServerClient
{
public:
    ServerClient(ENetPeer *p) : peer(p) {}
    ~ServerClient() = default;
    ENetPeer *peer;
    std::string name;
    void sendPacket(const std::vector<uint8_t> &data)
    {
        ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }
};

class NetworkServer
{
public:
    NetworkServer(uint16_t port);
    ~NetworkServer();
    void sendObjectCreate(const ObjectCreatePacket &obj, ServerClient *target = nullptr);
    void sendObjectDestroy(const std::string &name);
    void sendObjectTextured(const ObjectTexturedPacket &obj);
    void sendPositionUpdate(const NetPositionPacket &posPkt);
    void sendPlayerCreate(const PlayerCreatePacket &playerPkt, ServerClient *target = nullptr);
    void broadcastPacket(const std::vector<uint8_t> &data, ENetPeer *excludedPeer = nullptr);
    void update();
    void setOnClientJoinCallback(void (*callback)(ServerClient *))
    {
        onClientJoinCallback = callback;
    }
    void setKeyEventCallback(void (*callback)(ServerClient *, uint8_t))
    {
        keyEventCallback = callback;
    }
    void setOnClientLeaveCallback(void (*callback)(ServerClient *))
    {
        onClientLeaveCallback = callback;
    }
    void setAfterClientLeave(void (*callback)(std::string))
    {
        afterClientLeaveCallback = callback;
    }
    void sendScoreUpdate(const ScoreUpdatePacket &scorePkt, ServerClient *target = nullptr);
    ENetHost *server;
    std::recursive_mutex enetMutex; // Use recursive_mutex

private:
    std::vector<ServerClient> clients;
    void (*keyEventCallback)(ServerClient *, uint8_t);
    void (*onClientJoinCallback)(ServerClient *);
    void (*onClientLeaveCallback)(ServerClient *);
    void (*afterClientLeaveCallback)(std::string);
};

class NetworkClient
{
public:
    NetworkClient(const std::string &address, uint16_t port);
    ~NetworkClient();
    void update();
    void onObjectCreate(void (*callback)(const ObjectCreatePacket &))
    {
        objectCreateCallback = callback;
    }
    void onObjectDestroy(void (*callback)(const ObjectDestroyPacket &))
    {
        objectDestroyCallback = callback;
    }
    void onPositionUpdate(void (*callback)(const NetPositionPacket &))
    {
        positionUpdateCallback = callback;
    }
    void setPlayerCreateCallback(void (*callback)(const PlayerCreatePacket &, bool))
    {
        playerCreateCallback = callback;
    }
    void setScoreUpdateCallback(void (*callback)(const ScoreUpdatePacket &))
    {
        scoreUpdateCallback = callback;
    }
    void connect();
    void sendKeyEvent(uint8_t key);

    std::recursive_mutex enetMutex;

private:
    bool playerCreated = false;
    ENetHost *client;
    ENetPeer *peer;
    ENetAddress *addr;
    void (*objectCreateCallback)(const ObjectCreatePacket &);
    void (*objectDestroyCallback)(const ObjectDestroyPacket &);
    void (*positionUpdateCallback)(const NetPositionPacket &);
    void (*playerCreateCallback)(const PlayerCreatePacket &, bool);
    void (*scoreUpdateCallback)(const ScoreUpdatePacket &);
};