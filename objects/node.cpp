#include "node.h"

namespace ketu::objects
{

    Node::Node(
        const ketu::sensing::SensingClient* sensing_client,
        const ketu::communication::CommunicationClient* communication_client)
    {
        this->sensing_client_ = sensing_client;
        this->communication_client_ = communication_client;
    }

} // namespace ketu::objects
