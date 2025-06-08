#ifndef NODE_H
#define NODE_H

#include "world.h"
#include "../communication/communication_client.h"
#include "../sensing/sensing_client.h"

namespace ketu::objects
{

    class Node {
    public:
        explicit Node(
            const ketu::sensing::SensingClient* sensing_client,
            const ketu::communication::CommunicationClient* communication_client
        );

    private:
        const ketu::sensing::SensingClient* sensing_client_;
        const ketu::communication::CommunicationClient* communication_client_;
    };

} // namespace ketu::objects

#endif // NODE_H
