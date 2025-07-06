#ifndef MESH_BASED_FORMATION_COORDINATOR_H
#define MESH_BASED_FORMATION_COORDINATOR_H

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>


#include "formation_coordinator.h"
#include "third_party/tinyobj/loader.h"
#include "world/world.h"

namespace ketu::formation
{
    class MeshBasedFormationCoordinator : public FormationCoordinator
    {
    public:
        MeshBasedFormationCoordinator(const std::string& meshPath, double scalingFactor, const ketu::world::World* world);

        int getMaxNeighborCount(const std::string& nodeId) override;

        bool isNodeLocallyFormed(const std::string& nodeId) override;

        std::vector<std::string> getLocalNeighbors(const std::string& nodeId) override;

        void setLocalNeighbors(const std::string& nodeId, const std::vector<std::string> neighbors) override;

        bool isNodeAssigned(const std::string& nodeId) override;

        bool isNodeInPosition(const std::string& sourceNodeId, const std::string& targetNodeId) override;

        bool isFormationComplete() override;

        const NodeMessages align(const std::string& nodeId, const NodePositions& relativeNodePositions) override;

    private:
        void assignPriority(const std::string& nodeId);

        const ketu::world::World* world_;
        std::unique_ptr<ketu::thirdparty::tinyobj::Loader> loader_;
        std::unordered_map<std::string, int> nodeSlotMapping_;
        std::set<int> availableSlots_;

        // priority
        int priorityCounter_;
        std::unordered_map<std::string, int> priorityMapping_;
    };

} // namespace ketu::formation

#endif // MESH_BASED_FORMATION_COORDINATOR_H
