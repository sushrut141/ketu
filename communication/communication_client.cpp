#include "communication_client.h"

#include "../objects/node.h"

namespace ketu::communication
{
    CommunicationClient::CommunicationClient(const ketu::world::World* world) { this->world_ = world; }

    void CommunicationClient::registerNode(const std::string& nodeId, Communicable* node)
    {
        nodes_.insert(std::make_pair(nodeId, node));
    }

    bool CommunicationClient::sendMessage(const std::string& nodeId, const MessageType& messageType)
    {
        auto iter = nodes_.find(nodeId);
        if (iter != nodes_.end())
        {
            Communicable* communicable = iter->second;
            communicable->onMessage(messageType);
            return true;
        }
        return false;
    }


} // namespace ketu::communication
