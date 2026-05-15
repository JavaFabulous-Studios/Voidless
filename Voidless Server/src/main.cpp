#include <enet/enet.h>
#include <iostream>

int main(int argc, char** argv) {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost* server;

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = 1234;

    server = enet_host_create(&address /* the address to bind the server host to */, 
                             32      /* allow up to 32 clients and/or outgoing connections */,
                             2       /* allow up to 2 channels to be used, 0 and 1 */,
                             0       /* assume any amount of incoming bandwidth */,
                             0       /* assume any amount of outgoing bandwidth */);

    if (server == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Voidless Server started on port 1234..." << std::endl;

    ENetEvent event;
    while (true) {
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << "." << std::endl;
                    /* Store any relevant client information here. */
                    event.peer->data = (void*)"Client info";
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "A packet of length " << event.packet->dataLength << " containing " << (char*)event.packet->data << " was received from " << (char*)event.peer->data << " on channel " << event.channelID << "." << std::endl;
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << (char*)event.peer->data << " disconnected." << std::endl;
                    /* Reset the peer's client information. */
                    event.peer->data = nullptr;
                    break;
                default:
                    break;
            }
        }
    }

    enet_host_destroy(server);
    return 0;
}
