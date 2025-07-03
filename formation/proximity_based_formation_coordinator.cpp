#include "proximity_based_formation_coordinator.h"

#include "../planning/movement.h"

namespace ketu::formation
{
    constexpr int MAX_NEIGHBORS = 3;
    constexpr double PROXIMITY_DISTANCE = 0.5f;
    ketu::telemetry::Position ORIGIN = ketu::telemetry::Position::from(0.0, 0.0, 0.0);

     ProximityBasedFormationCoordinator::ProximityBasedFormationCoordinator(const ketu::world::World* world)
     {
         world_ = world;
         neighbors_ = {};
         assignedNodes_ = {};
     }


    int ProximityBasedFormationCoordinator::getMaxNeighborCount(const std::string& nodeId)
    {
        return MAX_NEIGHBORS;
    }

    bool ProximityBasedFormationCoordinator::isNodeLocallyFormed(const std::string& nodeId)
     {
         if (neighbors_.find(nodeId) == neighbors_.end())
         {
             return false;
         }
         const auto& neighbors = neighbors_.at(nodeId);
         for (const auto& neighbor : neighbors)
         {
             if (!isNodeInPosition(nodeId, neighbor))
             {
                 return false;
             }
         }
         return true;
     }

    std::vector<std::string> ProximityBasedFormationCoordinator::getLocalNeighbors(const std::string& nodeId)
     {
        if (neighbors_.find(nodeId) != neighbors_.end())
        {
            auto neighbors = neighbors_.at(nodeId);
            return std::vector<std::string>(neighbors.begin(), neighbors.end());
        }
         return {};
     }

    void ProximityBasedFormationCoordinator::setLocalNeighbors(const std::string& nodeId,
                                                               const std::vector<std::string> neighbors)
    {
         if (neighbors_.find(nodeId) == neighbors_.end())
         {
             neighbors_.insert({nodeId, {}});
             assignedNodes_.insert(nodeId);
         }
         std::set<std::string>& existingNeighbors = neighbors_.at(nodeId);
         int neighborPtr = 0;
         while (existingNeighbors.size() < MAX_NEIGHBORS && neighborPtr < neighbors.size())
         {
             const auto& neighbor = neighbors.at(neighborPtr);
             bool notExistingNeighbor = existingNeighbors.find(neighbor) == existingNeighbors.end();
             bool notNotAssigned = !isNodeAssigned(neighbor);
             if (notExistingNeighbor && notNotAssigned)
             {
                 existingNeighbors.insert(neighbor);
                 assignedNodes_.insert(neighbor);
             }
             neighborPtr += 1;
         }
    }

    bool ProximityBasedFormationCoordinator::isNodeAssigned(const std::string& nodeId)
     {
        return assignedNodes_.find(nodeId) != assignedNodes_.end();
     }

    bool ProximityBasedFormationCoordinator::isNodeInPosition(const std::string& sourceNodeId,
                                                              const std::string& targetNodeId)
    {
         const auto& sourcePosition = world_->getNodePosition(sourceNodeId);
         const auto& targetPosition = world_->getNodePosition(targetNodeId);
         const auto relativePosition = targetPosition - sourcePosition;
         auto message = ketu::planning::move_within_distance(relativePosition, ORIGIN, PROXIMITY_DISTANCE);
         return message == ketu::communication::STOP;
    }

    bool ProximityBasedFormationCoordinator::isFormationComplete() {}

    const ketu::formation::NodeMessages
    ProximityBasedFormationCoordinator::align(const std::string& nodeId,
                                              const ketu::formation::NodePositions& relativeNodePositions)
    {
         ketu::formation::NodeMessages messages = {};
         for (const auto& [neighborId, neighborPosition] : relativeNodePositions)
         {
            auto message =
                ketu::planning::move_within_distance(neighborPosition, ORIGIN, PROXIMITY_DISTANCE);
             messages.insert({neighborId, message});
         }
         return messages;
    }

} // namespace ketu::formation
