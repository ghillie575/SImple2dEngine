#include <network/network.hpp>
#include <stdexcept>
#include <algorithm>

NetworkClient::NetworkClient(const std::string &address, uint16_t port)
    : objectCreateCallback(nullptr), objectDestroyCallback(nullptr), positionUpdateCallback(nullptr)
{
    enet_initialize();
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client)
    {
        throw std::runtime_error("Failed to create ENet client host.");
    }
    addr = new ENetAddress();
    enet_address_set_host(addr, address.c_str());
    addr->port = port;
}

NetworkClient::~NetworkClient()
{
    delete addr;
    enet_host_destroy(client);
    enet_deinitialize();
}

void NetworkClient::update()
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0)
    {
        if (event.type == ENET_EVENT_TYPE_RECEIVE)
        {
            PacketReader reader(event.packet->data);
            PacketType pktType = reader.read<PacketType>();
            if (pktType == PacketType::ObjectCreatePacket && objectCreateCallback)
            {
                ObjectCreatePacket obj;
                obj.name = reader.readString();
                obj.position = reader.read<glm::vec2>();
                obj.size = reader.read<glm::vec2>();
                obj.rotation = reader.read<float>();
                obj.shader = reader.readString();
                objectCreateCallback(obj);
            }
            else if (pktType == PacketType::ObjectDestroyPacket && objectDestroyCallback)
            {
                ObjectDestroyPacket obj;
                obj.name = reader.readString();
                objectDestroyCallback(obj);
            }
            else if (pktType == PacketType::PositionPacket && positionUpdateCallback)
            {
                NetPositionPacket posPkt;
                posPkt.position = reader.read<glm::vec2>();
                posPkt.rotation = reader.read<float>();
                posPkt.objectName = reader.readString();
                positionUpdateCallback(posPkt);
            }
            else if (pktType == PacketType::PlayerCreatePacket)
            {
                PlayerCreatePacket playerPkt;
                playerPkt.name = reader.readString();
                playerPkt.position = reader.read<glm::vec2>();
                playerPkt.size = reader.read<glm::vec2>();
                playerPkt.rotation = reader.read<float>();
                playerPkt.shader = reader.readString();
                if (!playerCreated)
                {
                    if (playerCreateCallback)
                    {
                        playerCreateCallback(playerPkt, playerCreated);
                    }
                    playerCreated = true;
                }
                else
                {
                    playerCreateCallback(playerPkt, playerCreated);
                }
            }
            else if (pktType == PacketType::ScoreUpdatePacket && scoreUpdateCallback)
            {
                ScoreUpdatePacket scorePkt;
                scorePkt.scoreA = reader.read<int>();
                scorePkt.scoreB = reader.read<int>();
                scoreUpdateCallback(scorePkt);
            }
            enet_packet_destroy(event.packet);
        }
    }
}

void NetworkClient::sendKeyEvent(uint8_t key)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::KeyPacket);
    writer.write<uint8_t>(key);
    ENetPacket *packet = enet_packet_create(writer.data.data(), writer.data.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);
}

void NetworkClient::connect()
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    peer = enet_host_connect(client, addr, 2, 0);
    if (!peer)
    {
        throw std::runtime_error("No available peers for ENet client.");
    }
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
    }
    else
    {
        enet_peer_reset(peer);
        throw std::runtime_error("Failed to connect to ENet server.");
    }
}

NetworkServer::NetworkServer(uint16_t port)
    : keyEventCallback(nullptr), onClientJoinCallback(nullptr)
{
    enet_initialize();
    clients.reserve(32);
    ENetAddress addr;
    addr.host = ENET_HOST_ANY;
    addr.port = port;
    server = enet_host_create(&addr, 32, 2, 0, 0);
    if (!server)
    {
        throw std::runtime_error("Failed to create ENet server host.");
    }
}

NetworkServer::~NetworkServer()
{
    enet_host_destroy(server);
    enet_deinitialize();
}

void NetworkServer::update()
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    ENetEvent event;
    while (enet_host_service(server, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            clients.emplace_back(event.peer);
            clients.back().name = std::to_string(clients.size());
            if (onClientJoinCallback)
            {
                ServerClient *clientPtr = &clients.back();
                onClientJoinCallback(clientPtr);
            }
            break;

        case ENET_EVENT_TYPE_RECEIVE:
        {
            PacketReader reader(event.packet->data);
            PacketType pktType = reader.read<PacketType>();
            if (pktType == PacketType::KeyPacket)
            {
                uint8_t key = reader.read<uint8_t>();
                for (auto &client : clients)
                {
                    if (client.peer == event.peer && keyEventCallback)
                    {
                        keyEventCallback(&client, key);
                    }
                }
            }
            enet_packet_destroy(event.packet);
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (it->peer == event.peer)
                {

                    onClientLeaveCallback(&(*it));
                    std::string name = it->name;
                    clients.erase(it);
                    afterClientLeaveCallback(name);
                    break;
                }
            }
            break;

        default:
            break;
        }
    }
}

void NetworkServer::sendPositionUpdate(const NetPositionPacket &posPkt)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::PositionPacket);
    writer.write<glm::vec2>(posPkt.position);
    writer.write<float>(posPkt.rotation);
    writer.writeString(posPkt.objectName);

    broadcastPacket(writer.data);
    enet_host_flush(server);
}

void NetworkServer::broadcastPacket(const std::vector<uint8_t> &data, ENetPeer *excludedPeer)
{
    for (auto &client : clients)
    {
        if (client.peer != excludedPeer)
        {
            client.sendPacket(data);
        }
    }
}
void NetworkServer::sendScoreUpdate(const ScoreUpdatePacket &scorePkt, ServerClient *target)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::ScoreUpdatePacket);
    writer.write<int>(scorePkt.scoreA);
    writer.write<int>(scorePkt.scoreB);
    if(target)
    {
        target->sendPacket(writer.data);
        enet_host_flush(server);
        return;
    }
    broadcastPacket(writer.data);
    enet_host_flush(server);
}
void NetworkServer::sendObjectDestroy(const std::string &name)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::ObjectDestroyPacket);
    writer.writeString(name);
    broadcastPacket(writer.data);
    enet_host_flush(server);
}

void NetworkServer::sendObjectTextured(const ObjectTexturedPacket &obj)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::ObjectTexturedPacket);
    writer.writeString(obj.base.name);
    writer.write<glm::vec2>(obj.base.position);
    writer.write<glm::vec2>(obj.base.size);
    writer.write<float>(obj.base.rotation);
    writer.writeString(obj.base.shader);
    writer.writeString(obj.texturePath);
    broadcastPacket(writer.data);
    enet_host_flush(server);
}

void NetworkServer::sendObjectCreate(const ObjectCreatePacket &obj, ServerClient *target)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::ObjectCreatePacket);
    writer.writeString(obj.name);
    writer.write<glm::vec2>(obj.position);
    writer.write<glm::vec2>(obj.size);
    writer.write<float>(obj.rotation);
    writer.writeString(obj.shader);
    std::vector<uint8_t> data = writer.data;

    if (target)
    {
        target->sendPacket(data);
    }
    else
    {
        broadcastPacket(data);
    }
    enet_host_flush(server);
}
void NetworkServer::sendPlayerCreate(const PlayerCreatePacket &playerPkt, ServerClient *target)
{
    std::lock_guard<std::recursive_mutex> lock(enetMutex);
    PacketWriter writer;
    writer.write<PacketType>(PacketType::PlayerCreatePacket);
    writer.writeString(playerPkt.name);
    writer.write<glm::vec2>(playerPkt.position);
    writer.write<glm::vec2>(playerPkt.size);
    writer.write<float>(playerPkt.rotation);
    writer.writeString(playerPkt.shader);
    std::vector<uint8_t> data = writer.data;

    if (target)
    {
        target->sendPacket(data);
    }
    else
    {
        broadcastPacket(data);
    }
    enet_host_flush(server);
}