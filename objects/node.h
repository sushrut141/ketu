#ifndef NODE_H
#define NODE_H

#include <functional>
#include <string>

#include "communication/communication_client.h"
#include "communication/interfaces.h"
#include "formation/formation_coordinator.h"
#include "sensing/sensing_client.h"


namespace ketu::objects
{

    class Node : public ketu::communication::Communicable
    {
    public:
        explicit Node(const std::string& nodeId, ketu::sensing::SensingClient* sensing_client,
                      ketu::communication::CommunicationClient* communication_client,
                      ketu::formation::FormationCoordinator* formationCoordinator);

        // Returns the unique id of the node.
        const std::string& getId() const;

        // Invokes the callback with diff in the position of the node wrt to North.
        void setOnNodeUpdated(const std::function<void(std::string, ketu::telemetry::Position)>& callback);

        // Commands the node to carry out a particular action.
        void onMessage(const ketu::communication::MessageType& message_type) override;

    private:
        void onNodeMove_(const ketu::communication::MessageType& message_type);

        void onNodeAnneal_();

        const std::string node_id_;
        ketu::sensing::SensingClient* sensing_client_;
        ketu::communication::CommunicationClient* communication_client_;
        ketu::formation::FormationCoordinator* formationCoordinator_;

        std::function<void(std::string, ketu::telemetry::Position)> onNodeUpdated_;
    };

} // namespace ketu::objects

#endif // NODE_H
