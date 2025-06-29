#ifndef MESH_BASED_FORMATION_COORDINATOR_H
#define MESH_BASED_FORMATION_COORDINATOR_H

#include <memory>
#include <string>

#include "../third_party/tinyobj/loader.h"
#include "formation_coordinator.h"

namespace ketu::formation
{
    class MeshBasedFormationCoordinator : public FormationCoordinator
    {
    public:
        MeshBasedFormationCoordinator(const std::string& meshPath);

        int maxConnectivity() override;

        bool isNodeLocallyFormed(const std::string& nodeId) override;

        std::vector<std::string> getLocalNeighbors(const std::string& nodeId) override;

        void setLocalNeighbors(const std::string& nodeId, const std::vector<std::string> neighbors) override;

        bool isNodeFrozen(const std::string& nodeId) override;

        bool isNodeAssigned(const std::string& nodeId) override;

        bool isNodeInPosition(const std::string& sourceNodeId, const std::string& targetNodeId) override;

        bool isFormationComplete() override;

        const NodeMessages align(const std::string& nodeId, const NodePositions& relativeNodePositions) override;

    private:
        std::unique_ptr<ketu::thirdparty::tinyobj::Loader> loader_;
    };

} // ketu::formation

#endif // MESH_BASED_FORMATION_COORDINATOR_H
