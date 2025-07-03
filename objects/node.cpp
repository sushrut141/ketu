#include "node.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

namespace ketu::objects
{
    constexpr double MOVEMENT_STEP = 0.005;
    constexpr int EXTRA_NODES_FETCHED = 100;
    constexpr double NODE_CLEARANCE_AREA = 0.3;


    namespace
    {
        ketu::telemetry::Position getPositionDiff(const ketu::communication::MessageType& message_type)
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
            return position;
        }

        ketu::telemetry::Position getAlternatePosition(const ketu::communication::MessageType& message_type)
        {
            ketu::telemetry::Position position;
            switch (message_type)
            {
            case ketu::communication::MessageType::MOVE_X_POSITIVE:
                position = ketu::telemetry::Position::from(0.0, MOVEMENT_STEP, 0.0);
                break;
            case ketu::communication::MessageType::MOVE_X_NEGATIVE:
                position = ketu::telemetry::Position::from(0.0, -MOVEMENT_STEP, 0.0);
                break;
            case ketu::communication::MessageType::MOVE_Y_POSITIVE:
                position = ketu::telemetry::Position::from(-MOVEMENT_STEP, 0.0, 0.0);
                break;
            case ketu::communication::MessageType::MOVE_Y_NEGATIVE:
                position = ketu::telemetry::Position::from(MOVEMENT_STEP, 0.0, 0.0);
                break;
            case ketu::communication::MessageType::MOVE_Z_POSITIVE:
                position = ketu::telemetry::Position::from(0.0, MOVEMENT_STEP, 0.0);
                break;
            case ketu::communication::MessageType::MOVE_Z_NEGATIVE:
                position = ketu::telemetry::Position::from(0.0, -MOVEMENT_STEP, -0.0);
                break;
            case ketu::communication::MessageType::UNSPECIFIED:
            case ketu::communication::MessageType::STOP:
            default:
                break;
            }
            return position;
        }

    } // namespace

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


    void Node::onNodeMove_(const ketu::communication::MessageType& messageType)
    {
        auto position = getPositionDiff(messageType);
        auto nearestNeighbors = sensing_client_->getKNearestNeighbors(getId(), EXTRA_NODES_FETCHED);
        bool isPositionOpen = true;
        for (const auto& idDistancePair : nearestNeighbors)
        {
            auto diff = idDistancePair.second - position;
            double magnitude = diff.magnitude();
            if (magnitude < NODE_CLEARANCE_AREA)
            {
                std::cout << "Encountered node " << idDistancePair.first << " in path" << std::endl;
                isPositionOpen = false;
                break;
            }
        }
        if (!isPositionOpen)
        {
            std::cout << "Moving to random alternate posistion" << std::endl;
            position = getAlternatePosition(messageType);
        }

        onNodeUpdated_(node_id_, position);
    }

    void Node::onNodeAnneal_()
    {
        auto neighbors = formationCoordinator_->getLocalNeighbors(getId());
        // Assign local neighbors if not assigned.
        if (neighbors.empty())
        {
            std::cout << "Annealing node " << getId() << " without neighbors" << std::endl;
            auto nearestNodes = sensing_client_->getKNearestNeighbors(getId(), EXTRA_NODES_FETCHED);
            std::vector<std::string> neighborIds;
            for (auto it = nearestNodes.begin(); it != nearestNodes.end(); ++it)
            {
                neighborIds.push_back(it->first);
            }
            formationCoordinator_->setLocalNeighbors(getId(), neighborIds);
            // Not all nearest nodes will get assigned slots so fetch it again
            // and only align nodes that are actually assigned.
            const auto& assignedNeighbors = formationCoordinator_->getLocalNeighbors(getId());
            const auto neighborIdDistances = sensing_client_->getDistanceToNodes(getId(), assignedNeighbors);

            auto neighborMessages = formationCoordinator_->align(getId(), neighborIdDistances);
            for (const auto& neighbor : neighborMessages)
            {
                communication_client_->sendMessage(neighbor.first, neighbor.second);
            }
        }
        else
        {
            std::cout << "Annealing node " << getId() << " with all neighbors" << std::endl;
            for (const auto& neighbor : neighbors)
            {
                std::cout << " Neighbor " << neighbor << std::endl;
            }
            auto nearestNodes = sensing_client_->getDistanceToNodes(getId(), neighbors);
            auto neighborMessages = formationCoordinator_->align(getId(), nearestNodes);
            for (const auto& neighborMessage : neighborMessages)
            {
                communication_client_->sendMessage(neighborMessage.first, neighborMessage.second);
            }
            // Propagate annealing to all neighbors.
            for (const auto& neighbor : neighbors)
            {
                if (!formationCoordinator_->isNodeLocallyFormed(neighbor))
                {
                    std::cout << "Propagating annealing to node " << neighbor << std::endl;
                    communication_client_->sendMessage(neighbor, ketu::communication::MessageType::ANNEAL);
                }
            }
        }
    }


} // namespace ketu::objects
