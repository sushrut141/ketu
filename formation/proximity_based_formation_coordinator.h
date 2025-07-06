#ifndef PROXIMITY_BASED_FORMATION_COORDINATOR_H
#define PROXIMITY_BASED_FORMATION_COORDINATOR_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "formation_coordinator.h"
#include "world/world.h"

namespace ketu::formation
{
    class ProximityBasedFormationCoordinator : public ketu::formation::FormationCoordinator {
    public:

        ProximityBasedFormationCoordinator(const ketu::world::World* world);

        int getMaxNeighborCount(const std::string& nodeId) override;

        bool isNodeLocallyFormed(const std::string& nodeId) override;

        std::vector<std::string> getLocalNeighbors(const std::string& nodeId) override;

        void setLocalNeighbors(const std::string& nodeId, const std::vector<std::string> neighbors) override;

        bool isNodeAssigned(const std::string& nodeId) override;

        bool isNodeInPosition(const std::string& sourceNodeId, const std::string& targetNodeId) override;

        bool isFormationComplete() override;

        const ketu::formation::NodeMessages align(const std::string& nodeId,
                                                  const ketu::formation::NodePositions& relativeNodePositions) override;

    private:
        const ketu::world::World* world_;

        std::unordered_map<std::string, std::set<std::string>> neighbors_;

        std::set<std::string> assignedNodes_;
    };

} // namespace ketu::formation

#endif //PROXIMITY_BASED_FORMATION_COORDINATOR_H
