#ifndef COMPOSITE_FORMATION_COORDINATOR_H
#define COMPOSITE_FORMATION_COORDINATOR_H

#include <vector>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "../formation/formation_coordinator.h"
#include "../telemetry/position.h"
#include "../communication/interfaces.h"

namespace ketu::formation
{

    class CompositeFormationCoordinator : public FormationCoordinator
    {
    public:
        explicit CompositeFormationCoordinator(std::vector<ketu::formation::FormationCoordinator *> coordinators);

        void switchCoordinator(int idx);

        // Overridden virtual methods from FormationCoordinator
        int getMaxNeighborCount(const std::string &nodeId) override;

        bool isNodeLocallyFormed(const std::string &nodeId) override;

        std::vector<std::string> getLocalNeighbors(const std::string &nodeId) override;

        void setLocalNeighbors(const std::string &nodeId, const std::vector<std::string> neighbors) override;

        bool isNodeAssigned(const std::string &nodeId) override;

        bool isNodeInPosition(const std::string &sourceNodeId, const std::string &targetNodeId) override;

        bool isFormationComplete() override;

        const NodeMessages align(const std::string &nodeId, const NodePositions &relativeNodePositions) override;

    private:
        std::vector<ketu::formation::FormationCoordinator *> coordinators_;
        int active_coordinator_idx_;
    };

} // namespace ketu::formation

#endif // COMPOSITE_FORMATION_COORDINATOR_H
