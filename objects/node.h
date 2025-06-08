#ifndef NODE_H
#define NODE_H

#include <string>

#include "../communication/communication_client.h"
#include "../sensing/sensing_client.h"

namespace ketu::objects
{

    class Node {
    public:
        explicit Node(
            const std::string& node_id,
            const ketu::sensing::SensingClient* sensing_client,
            const ketu::communication::CommunicationClient* communication_client
        );

        // Returns the unique id of the node.
        const std::string& getId();

    private:
        const std::string node_id_;
        const ketu::sensing::SensingClient* sensing_client_;
        const ketu::communication::CommunicationClient* communication_client_;
    };

} // namespace ketu::objects

#endif // NODE_H
