#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <string>
#include <unordered_map>

#include "world/world.h"
#include "./interfaces.h"

namespace ketu::communication
{
    // Encapsulates communication with other nodes in the world.
    class CommunicationClient
    {
    public:
        explicit CommunicationClient(const ketu::world::World* world);

        void registerNode(const std::string& nodeId, Communicable* node);

        // Send a message to a node. Returns true if the message was sent.
        bool sendMessage(const std::string& nodeId, const MessageType& messageType);

    private:
        const ketu::world::World* world_;
        std::unordered_map<std::string, Communicable*> nodes_;
    };

} // namespace ketu::communication

#endif // COMMUNICATION_H
