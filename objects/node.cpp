#include "node.h"

#include <algorithm>

namespace ketu::objects
{
    constexpr double MOVEMENT_STEP = 0.005;

    Node::Node(const std::string& nodeId, ketu::sensing::SensingClient* sensing_client,
               ketu::communication::CommunicationClient* communication_client,
               ketu::formation::FormationCoordinator* formationCoordinator) :
        Communicable(), node_id_(nodeId), sensing_client_(sensing_client), communication_client_(communication_client),
        formationCoordinator_(formationCoordinator),
        onNodeUpdated_([](std::string nodeId, ketu::telemetry::Position pos) {})
    {
        communication_client_->registerNode(nodeId, this);
    }

    const std::string& Node::getId() const { return this->node_id_; }

    void Node::setOnNodeUpdated(const std::function<void(std::string, ketu::telemetry::Position)>& callback)
    {
        onNodeUpdated_ = callback;
    }

    void Node::onMessage(const ketu::communication::MessageType& message_type)
    {
        switch (message_type)
        {
        case ketu::communication::MessageType::ANNEAL:
            onNodeAnneal_();
            break;
        case ketu::communication::MessageType::MOVE_X_POSITIVE:
        case ketu::communication::MessageType::MOVE_X_NEGATIVE:
        case ketu::communication::MessageType::MOVE_Y_POSITIVE:
        case ketu::communication::MessageType::MOVE_Y_NEGATIVE:
        case ketu::communication::MessageType::MOVE_Z_POSITIVE:
        case ketu::communication::MessageType::MOVE_Z_NEGATIVE:
            onNodeMove_(message_type);
            break;
        case ketu::communication::MessageType::STOP:
        case ketu::communication::MessageType::UNSPECIFIED:
        default:
            break;
        }
    }


    void Node::onNodeMove_(const ketu::communication::MessageType& message_type)
    {
        ketu::telemetry::Position position;
        switch (message_type)
        {
        case ketu::communication::MessageType::MOVE_X_POSITIVE:
            position = ketu::telemetry::Position::from(MOVEMENT_STEP, 0.0, 0.0);
            break;
        case ketu::communication::MessageType::MOVE_X_NEGATIVE:
            position = ketu::telemetry::Position::from(-MOVEMENT_STEP, 0.0, 0.0);
            break;
        case ketu::communication::MessageType::MOVE_Y_POSITIVE:
            position = ketu::telemetry::Position::from(0.0, MOVEMENT_STEP, 0.0);
            break;
        case ketu::communication::MessageType::MOVE_Y_NEGATIVE:
            position = ketu::telemetry::Position::from(0.0, -MOVEMENT_STEP, 0.0);
            break;
        case ketu::communication::MessageType::MOVE_Z_POSITIVE:
            position = ketu::telemetry::Position::from(0.0, 0.0, MOVEMENT_STEP);
            break;
        case ketu::communication::MessageType::MOVE_Z_NEGATIVE:
            position = ketu::telemetry::Position::from(0.0, 0.0, -MOVEMENT_STEP);
            break;
        case ketu::communication::MessageType::UNSPECIFIED:
        case ketu::communication::MessageType::STOP:
        default:
            break;
        }
        onNodeUpdated_(node_id_, position);
    }

    void Node::onNodeAnneal_()
    {
        auto neighbors = formationCoordinator_->getLocalNeighbors(getId());
        // Assign local neighbors if not assigned.
        if (neighbors.empty())
        {
            auto nearestNodes =
                sensing_client_->getKNearestNeighbors(getId(), formationCoordinator_->maxConnectivity());
            // Remove nodes already part of some other node's formation.
            for (auto it = nearestNodes.begin(); it != nearestNodes.end();)
            {
                if (formationCoordinator_->isNodeFrozen(it->first))
                {
                    it = nearestNodes.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            std::vector<std::string> neighborIds;
            for (auto it = nearestNodes.begin(); it != nearestNodes.end(); ++it)
            {
                neighborIds.push_back(it->first);
            }
            formationCoordinator_->setLocalNeighbors(getId(), neighborIds);

            auto neighborMessages = formationCoordinator_->align(getId(), nearestNodes);
            for (const auto& neighbor : neighborMessages)
            {
                communication_client_->sendMessage(neighbor.first, neighbor.second);
            }
        }
        else
        {
            auto nearestNodes =
                sensing_client_->getKNearestNeighbors(getId(), formationCoordinator_->maxConnectivity());
            // Remove nodes already part of some other node's formation.
            for (auto it = nearestNodes.begin(); it != nearestNodes.end();)
            {
                bool isNodeNotAssignedNeighbor =
                    std::find(neighbors.begin(), neighbors.end(), it->first) == neighbors.end();
                // Ignore nearby nodes that aren't assigned to this local formation.
                if (formationCoordinator_->isNodeFrozen(it->first) || isNodeNotAssignedNeighbor)
                {
                    it = nearestNodes.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            std::vector<std::string> neighborIds;
            for (auto it = nearestNodes.begin(); it != nearestNodes.end(); ++it)
            {
                neighborIds.push_back(it->first);
            }

            auto neighborMessages = formationCoordinator_->align(getId(), nearestNodes);
            for (const auto& neighborMessage : neighborMessages)
            {
                communication_client_->sendMessage(neighborMessage.first, neighborMessage.second);
            }
        }
    }


} // namespace ketu::objects
