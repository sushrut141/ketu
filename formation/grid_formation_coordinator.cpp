
#include "grid_formation_coordinator.h"

#include "../planning/movement.h"
#include "../telemetry/position.h"

namespace ketu::formation
{
    constexpr double MIN_DISTANCE = 2.0f;

    static ketu::telemetry::Position TOP = ketu::telemetry::Position::from(0.0, 0.0, MIN_DISTANCE);
    static ketu::telemetry::Position BOTTOM = ketu::telemetry::Position::from(0.0, 0.0, -MIN_DISTANCE);
    static ketu::telemetry::Position LEFT = ketu::telemetry::Position::from(-MIN_DISTANCE, 0.0, 0.0);
    static ketu::telemetry::Position RIGHT = ketu::telemetry::Position::from(MIN_DISTANCE, 0.0, 0.0);
    static ketu::telemetry::Position FORWARD = ketu::telemetry::Position::from(0.0, MIN_DISTANCE, 0.0);
    static ketu::telemetry::Position BACKWARD = ketu::telemetry::Position::from(0.0, -MIN_DISTANCE, 0.0);

    static ketu::telemetry::Position* POSITIONS[6] = {&TOP, &BOTTOM, &LEFT, &RIGHT, &FORWARD, &BACKWARD};

    static int REVERSE_POSITION_MAPPING[6] = {1, 0, 3, 2, 5, 4};


    GridFormationCoordinator::GridFormationCoordinator(const ketu::world::World* world) :
        FormationCoordinator(), world_(world)
    {
        connectivity_ = {};
        frozenNodes_ = {};
    }

    int GridFormationCoordinator::maxConnectivity() { return 6; }

    bool GridFormationCoordinator::isNodeLocallyFormed(const std::string& nodeId)
    {
        auto it = frozenNodes_.find(nodeId);
        if (it != frozenNodes_.end())
        {
            for (const auto& neighbors : connectivity_)
            {
                if (frozenNodes_.find(neighbors.first) == frozenNodes_.end())
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    std::vector<std::string> GridFormationCoordinator::getLocalNeighbors(const std::string& nodeId)
    {
        std::vector<std::string> neighbors;
        auto it = connectivity_.find(nodeId);
        if (it != connectivity_.end())
        {
            for (const auto& neighbor : it->second)
            {
                neighbors.push_back(neighbor.first);
            }
        }
        return neighbors;
    }

    void GridFormationCoordinator::setLocalNeighbors(const std::string& nodeId,
                                                     const std::vector<std::string> neighbors)
    {
        connectivity_.insert({nodeId, {}});
        auto it = connectivity_.find(nodeId);
        auto& existingNeighbors = it->second;
        std::unordered_map<std::string, int> positionMapping;

        if (existingNeighbors.size() < maxConnectivity())
        {
            auto diff = maxConnectivity() - existingNeighbors.size();
            int i = existingNeighbors.size();
            int j = 0;
            while (i < maxConnectivity() && j < neighbors.size())
            {
                const std::string& newNodeId = neighbors.at(j);
                if (existingNeighbors.find(newNodeId) == existingNeighbors.end())
                {
                    existingNeighbors.insert({newNodeId, POSITIONS[i]});
                    positionMapping.insert({newNodeId, i});
                    i += 1;
                    j += 1;
                }
                else
                {
                    j += 1;
                }
            }
        }

        for (const auto& newNeighbor : positionMapping)
        {
            int reversePositionIdx = REVERSE_POSITION_MAPPING[newNeighbor.second];
            std::unordered_map<std::string, ketu::telemetry::Position*> reverseLinkage;
            reverseLinkage.insert({nodeId, POSITIONS[reversePositionIdx]});
            connectivity_.insert({newNeighbor.first, reverseLinkage});
        }
    }

    bool GridFormationCoordinator::isNodeFrozen(const std::string& nodeId)
    {
        return frozenNodes_.find(nodeId) != frozenNodes_.end();
    }

    bool GridFormationCoordinator::isFormationComplete() {}

    const NodeMessages GridFormationCoordinator::align(const std::string& nodeId,
                                                       const NodePositions& relativeNodePositions)
    {
        frozenNodes_.insert(nodeId);
        NodeMessages messages;
        const auto& targetPositionMap = connectivity_.at(nodeId);
        for (const auto& neighbor : relativeNodePositions)
        {
            const std::string& neighborId = neighbor.first;
            const auto& sourcePosition = neighbor.second;
            const auto& targetPosition = *targetPositionMap.at(neighborId);
            ketu::communication::MessageType message = ketu::planning::move(sourcePosition, targetPosition);
            messages.insert({neighborId, message});
            if (message == ketu::communication::MessageType::STOP)
            {
                frozenNodes_.insert(neighborId);
            }
        }
        return messages;
    }

} // namespace ketu::formation
