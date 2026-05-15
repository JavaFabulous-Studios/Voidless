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

            ENetEvent event;
            if (enet_host_service(s_Client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
                std::cout << "Connected to server at " << address << ":" << port << std::endl;
                s_Connected = true;
                return true;
            } else {
                enet_peer_reset(s_Peer);
                std::cerr << "Connection to server failed!" << std::endl;
                return false;
            }
        }

        static bool IsConnected() { return s_Connected; }

    private:
        static inline ENetHost* s_Client = nullptr;
        static inline ENetPeer* s_Peer = nullptr;
        static inline bool s_Connected = false;
    };

}
