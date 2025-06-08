#include "node.h"

namespace ketu::objects
{

    Node::Node(
        const std::string& node_id,
        const ketu::sensing::SensingClient* sensing_client,
        const ketu::communication::CommunicationClient* communication_client)
            : node_id_(node_id),
                sensing_client_(sensing_client),
                communication_client_(communication_client)
    {
    }

    const std::string& Node::getId()
    {
        return this->node_id_;
    }


} // namespace ketu::objects
