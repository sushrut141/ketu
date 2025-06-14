#ifndef NODE_H
#define NODE_H

#include <string>
#include <functional>

#include "../communication/communication_client.h"
#include "../communication/types.h"
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
        const std::string& getId() const;

        // Invokes the callback with diff in the position of the node wrt to North.
        void setOnNodeUpdated(const std::function<void(ketu::telemetry::Position)>& callback);

        // Commands the node to carry out a particular action.
        void onMessage(const ketu::communication::MessageType& message_type);

    private:

        void onNodeMove_(const ketu::communication::MessageType& message_type);

        void onNodeAnneal_();

        const std::string node_id_;
        const ketu::sensing::SensingClient* sensing_client_;
        const ketu::communication::CommunicationClient* communication_client_;

        std::function<void(ketu::telemetry::Position)> onNodeUpdated_;
    };

} // namespace ketu::objects

#endif // NODE_H
