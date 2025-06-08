#include "node.h"

namespace ketu::objects
{

    Node::Node(const ketu::sensing::SensingClient* sensing_client)
    {
        this->sensing_client_ = sensing_client;
    }

} // namespace ketu::objects
