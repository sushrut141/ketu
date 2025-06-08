#include "communication_client.h"

namespace ketu::communication
{
    CommunicationClient::CommunicationClient(const ketu::world::World* world)
    {
        this->world_ = world;
    }


} // namespace ketu::communication
