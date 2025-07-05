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
        explicit CompositeFormationCoordinator(std::vector<ketu::formation::FormationCoordinator *> coordinators)
            : coordinators_(coordinators), active_coordinator_idx_(0)
        {
            if (coordinators_.empty())
            {
                throw std::invalid_argument("Coordinators vector cannot be empty.");
            }
        }

        void switchCoordinator(int idx)
        {
            if (idx < 0 || idx >= coordinators_.size())
            {
                throw std::out_of_range("Invalid coordinator index.");
            }
            active_coordinator_idx_ = idx;
        }

        // Overridden virtual methods from FormationCoordinator
        int getMaxNeighborCount(const std::string &nodeId) override
        {
            return coordinators_[active_coordinator_idx_]->getMaxNeighborCount(nodeId);
        }

        bool isNodeLocallyFormed(const std::string &nodeId) override
        {
            return coordinators_[active_coordinator_idx_]->isNodeLocallyFormed(nodeId);
        }

        std::vector<std::string> getLocalNeighbors(const std::string &nodeId) override
        {
            return coordinators_[active_coordinator_idx_]->getLocalNeighbors(nodeId);
        }

        void setLocalNeighbors(const std::string &nodeId, const std::vector<std::string> neighbors) override
        {
            coordinators_[active_coordinator_idx_]->setLocalNeighbors(nodeId, neighbors);
        }

        bool isNodeAssigned(const std::string &nodeId) override
        {
            return coordinators_[active_coordinator_idx_]->isNodeAssigned(nodeId);
        }

        bool isNodeInPosition(const std::string &sourceNodeId, const std::string &targetNodeId) override
        {
            return coordinators_[active_coordinator_idx_]->isNodeInPosition(sourceNodeId, targetNodeId);
        }

        bool isFormationComplete() override
        {
            return coordinators_[active_coordinator_idx_]->isFormationComplete();
        }

        const NodeMessages align(const std::string &nodeId, const NodePositions &relativeNodePositions) override
        {
            return coordinators_[active_coordinator_idx_]->align(nodeId, relativeNodePositions);
        }

    private:
        std::vector<ketu::formation::FormationCoordinator *> coordinators_;
        int active_coordinator_idx_;
    };

} // namespace ketu::formation

#endif // COMPOSITE_FORMATION_COORDINATOR_H
