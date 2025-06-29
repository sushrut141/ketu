#ifndef GRID_FORMATION_COORDINATOR_H
#define GRID_FORMATION_COORDINATOR_H

#include <unordered_map>
#include <unordered_set>

#include "../world/world.h"
#include "formation_coordinator.h"

namespace ketu::formation
{

    class GridFormationCoordinator : public FormationCoordinator
    {

    public:
        GridFormationCoordinator(const ketu::world::World* world);

        int maxConnectivity() override;

        int getMaxNeighborCount(const std::string& nodeId) override;

        bool isNodeLocallyFormed(const std::string& nodeId) override;

        std::vector<std::string> getLocalNeighbors(const std::string& nodeId) override;

        void setLocalNeighbors(const std::string& nodeId, const std::vector<std::string> neighbors) override;

        bool isNodeFrozen(const std::string& nodeId) override;

        bool isNodeAssigned(const std::string& nodeId) override;

        bool isNodeInPosition(const std::string& sourceNodeId, const std::string& targetNodeId) override;

        bool isFormationComplete() override;

        const NodeMessages align(const std::string& nodeId, const NodePositions& relativeNodePositions) override;

    private:
        const ketu::world::World* world_;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> connectivity_;
        std::unordered_set<std::string> frozenNodes_;
    };

} // namespace ketu::formation

#endif // GRID_FORMATION_COORDINATOR_H
