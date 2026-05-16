#pragma once

#include <enet/enet.h>
#include <string>
#include <iostream>

namespace Voidless {

    class Network {
    public:
        static bool Init() {
            if (enet_initialize() != 0) {
                std::cerr << "Failed to initialize ENet!" << std::endl;
                return false;
            }
            return true;
        }

        static void Shutdown() {
            if (s_Client) {
                enet_host_destroy(s_Client);
            }
            enet_deinitialize();
        }

        static bool Connect(const std::string& address, int port) {
            s_Client = enet_host_create(nullptr, 1, 2, 0, 0);
            if (s_Client == nullptr) {
                std::cerr << "Failed to create ENet client host!" << std::endl;
                return false;
            }

            ENetAddress enetAddress;
            enet_address_set_host(&enetAddress, address.c_str());
            enetAddress.port = port;

            s_Peer = enet_host_connect(s_Client, &enetAddress, 2, 0);
            if (s_Peer == nullptr) {
                std::cerr << "No available peers for initiating an ENet connection!" << std::endl;
                return false;
            }

            std::cout << "Connecting to laptop at " << address << "... (Attempting for 10 seconds)" << std::endl;

            ENetEvent event;
            // Increased to 10 seconds for Wi-Fi stability
            if (enet_host_service(s_Client, &event, 10000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
                std::cout << "SUCCESS! Connected to server at " << address << ":" << port << std::endl;
                s_Connected = true;
                return true;
            } else {
                enet_peer_reset(s_Peer);
                std::cerr << "FAILED! Laptop at " << address << " did not answer." << std::endl;
                s_Connected = false;
                return false;
            }
        }

        enum class PacketType : uint8_t {
            POSITION = 1
        };

        struct PlayerPosPacket {
            PacketType type = PacketType::POSITION;
            float x, y, z;
        };

        static void SendPosition(float x, float y, float z) {
            if (!s_Connected || !s_Peer) return;

            PlayerPosPacket packet;
            packet.x = x;
            packet.y = y;
            packet.z = z;

            ENetPacket* enetPacket = enet_packet_create(&packet, sizeof(PlayerPosPacket), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
            enet_peer_send(s_Peer, 0, enetPacket);
        }

        static void Update() {
            if (!s_Client) return;
            ENetEvent event;
            while (enet_host_service(s_Client, &event, 0) > 0) {
                if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                    std::cout << "Disconnected from server." << std::endl;
                    s_Connected = false;
                }
                enet_packet_destroy(event.packet);
            }
        }

        static bool IsConnected() { return s_Connected; }

    private:
        static inline ENetHost* s_Client = nullptr;
        static inline ENetPeer* s_Peer = nullptr;
        static inline bool s_Connected = false;
    };

}
