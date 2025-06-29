#include "node.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

namespace ketu::objects
{
    constexpr double MOVEMENT_STEP = 0.005;
    constexpr int EXTRA_NODES_FETCHED = 100;
    constexpr double NODE_CLEARANCE_AREA = 0.5;


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
        for (const auto& idDistancePair: nearestNeighbors)
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
        bool nodesInFormation = formationCoordinator_->isNodeLocallyFormed(getId());
        // Assign local neighbors if not assigned.
        if (neighbors.empty())
        {
            std::cout << "Annealing node " << getId() << " without neighbors" << std::endl;
            auto nearestNodes =
                sensing_client_->getKNearestNeighbors(getId(), EXTRA_NODES_FETCHED);
            // Remove nodes already part of some other node's formation.
            for (auto it = nearestNodes.begin(); it != nearestNodes.end();)
            {
                if (formationCoordinator_->isNodeAssigned(it->first))
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
        else if (nodesInFormation)
        {
            std::cout << "All neighbors formed for node " << getId() << std::endl;
            for (const auto& neighbor : neighbors)
            {
                if (!formationCoordinator_->isNodeLocallyFormed(neighbor))
                {
                    std::cout << "Propagating annealing to node " << neighbor << std::endl;
                    communication_client_->sendMessage(neighbor, ketu::communication::MessageType::ANNEAL);
                    return;
                }
            }
        }
        else if (neighbors.size() == formationCoordinator_->getMaxNeighborCount(getId()))
        {
            std::cout << "Annealing node " << getId() << " with all neighbors" << std::endl;
            auto nearestNodes = sensing_client_->getDistanceToNodes(getId(), neighbors);
            std::unordered_map<std::string, ketu::telemetry::Position> frozenNeighbors;
            for (auto it = nearestNodes.begin(); it != nearestNodes.end();)
            {

                if (formationCoordinator_->isNodeInPosition(getId(), it->first))
                {
                    std::cout << "Node " << it->first << " already in position" << std::endl;
                    frozenNeighbors.insert({it->first, it->second});
                    it = nearestNodes.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            auto neighborMessages = formationCoordinator_->align(getId(), nearestNodes);
            for (const auto& neighborMessage : neighborMessages)
            {
                communication_client_->sendMessage(neighborMessage.first, neighborMessage.second);
            }
        }
        else
        {
            std::cout << "Annealing node " << getId() << " with some neighbors" << std::endl;
            // Use heuristic to fetch n nearby nodes, we cannot fetch all nodes since that
            // would be unrealistic in a real sensor.
            auto nearestNodes = sensing_client_->getKNearestNeighbors(getId(), EXTRA_NODES_FETCHED);
            std::unordered_map<std::string, ketu::telemetry::Position> availableNeighbors;
            int availableSlots = formationCoordinator_->getMaxNeighborCount(getId()) - neighbors.size();
            std::cout << "Available slots: " <<  availableSlots << " and neighbors: " << neighbors.size() << std::endl;
            std::cout << "Assigning available slots from nearest nodes: " << nearestNodes.size() << std::endl;
            for (auto it = nearestNodes.begin(); it != nearestNodes.end();)
            {
                if (availableSlots == 0)
                {
                    break;
                }
                if (formationCoordinator_->isNodeAssigned(it->first))
                {
                    ++it;
                }
                else
                {
                    neighbors.push_back(it->first);
                    availableNeighbors.insert({it->first, it->second});
                    availableSlots -= 1;
                    ++it;
                }
            }
            std::cout << "Post assignment available slots: " <<  availableSlots << " and neighbors " << neighbors.size() << std::endl;
            // Update neighbors to include new nodes.
            formationCoordinator_->setLocalNeighbors(getId(), neighbors);
            auto neighborMessages = formationCoordinator_->align(getId(), availableNeighbors);
            for (const auto& neighbor : neighborMessages)
            {
                communication_client_->sendMessage(neighbor.first, neighbor.second);
            }
        }
    }


} // namespace ketu::objects
