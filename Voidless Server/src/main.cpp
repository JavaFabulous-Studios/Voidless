#include <enet/enet.h>
#include <iostream>
#include <unordered_map>
#include <cstring>

// Must match the client's Network.hpp packet definition
enum class PacketType : uint8_t {
    POSITION = 1
};

struct PlayerPosPacket {
    PacketType type;
    float x, y, z;
};

// Broadcast packet for other players (includes the sender's ID)
struct BroadcastPosPacket {
    PacketType type;
    uint32_t playerId;
    float x, y, z;
};

int main(int argc, char** argv) {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== Voidless Server started on port 1234 ===" << std::endl;

    // Map from peer pointer to player ID
    std::unordered_map<ENetPeer*, uint32_t> playerIds;
    uint32_t nextPlayerId = 1;

    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    uint32_t id = nextPlayerId++;
                    playerIds[event.peer] = id;
                    std::cout << "[+] Player " << id << " connected from "
                              << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE: {
                    if (event.packet->dataLength == sizeof(PlayerPosPacket)) {
                        PlayerPosPacket pos;
                        std::memcpy(&pos, event.packet->data, sizeof(PlayerPosPacket));

                        if (pos.type == PacketType::POSITION) {
                            uint32_t senderId = playerIds[event.peer];

                            // Build broadcast packet
                            BroadcastPosPacket broadcast;
                            broadcast.type = PacketType::POSITION;
                            broadcast.playerId = senderId;
                            broadcast.x = pos.x;
                            broadcast.y = pos.y;
                            broadcast.z = pos.z;

                            // Send to all OTHER connected players
                            for (auto& [peer, id] : playerIds) {
                                if (peer != event.peer) {
                                    ENetPacket* pkt = enet_packet_create(
                                        &broadcast, sizeof(BroadcastPosPacket),
                                        ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
                                    enet_peer_send(peer, 0, pkt);
                                }
                            }

                            std::cout << "[P" << senderId << "] Pos: ("
                                      << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
                        }
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }

                case ENET_EVENT_TYPE_DISCONNECT: {
                    uint32_t id = playerIds[event.peer];
                    std::cout << "[-] Player " << id << " disconnected." << std::endl;
                    playerIds.erase(event.peer);
                    break;
                }

                default:
                    break;
            }
        }
    }

    enet_host_destroy(server);
    return 0;
}
