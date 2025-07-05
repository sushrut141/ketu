#include "composite_formation_coordinator.h"
#include <stdexcept> // For std::invalid_argument and std::out_of_range
#include <vector>    // For std::vector (though likely included via header)
#include <string>    // For std::string (though likely included via header)

namespace ketu::formation
{

    CompositeFormationCoordinator::CompositeFormationCoordinator(std::vector<ketu::formation::FormationCoordinator *> coordinators)
        : coordinators_(coordinators), active_coordinator_idx_(0)
    {
        if (coordinators_.empty())
        {
            throw std::invalid_argument("Coordinators vector cannot be empty.");
        }
        // Optional: Check for nullptrs within the coordinators vector itself if that's a concern.
        // for (const auto* coord : coordinators_) {
        //     if (!coord) {
        //         throw std::invalid_argument("Coordinator list contains null pointers.");
        //     }
        // }
    }

    void CompositeFormationCoordinator::switchCoordinator(int idx)
    {
        // The check idx >= coordinators_.size() also covers the case where coordinators_ is empty,
        // though the constructor should prevent empty coordinators_ from being stored.
        // However, size() on an empty vector is 0, so idx >= 0 would be idx >= .size().
        if (idx < 0 || static_cast<size_t>(idx) >= coordinators_.size())
        {
            throw std::out_of_range("Invalid coordinator index.");
        }
        active_coordinator_idx_ = idx;
    }

    int CompositeFormationCoordinator::getMaxNeighborCount(const std::string &nodeId)
    {
        // No explicit null check for coordinators_[active_coordinator_idx_] here,
        // as the constructor ensures coordinators_ is not empty and switchCoordinator ensures idx is valid.
        // If a FormationCoordinator* itself within the vector could be null, that's a different contract.
        return coordinators_[active_coordinator_idx_]->getMaxNeighborCount(nodeId);
    }

    bool CompositeFormationCoordinator::isNodeLocallyFormed(const std::string &nodeId)
    {
        return coordinators_[active_coordinator_idx_]->isNodeLocallyFormed(nodeId);
    }

    std::vector<std::string> CompositeFormationCoordinator::getLocalNeighbors(const std::string &nodeId)
    {
        return coordinators_[active_coordinator_idx_]->getLocalNeighbors(nodeId);
    }

    void CompositeFormationCoordinator::setLocalNeighbors(const std::string &nodeId, const std::vector<std::string> neighbors)
    {
        coordinators_[active_coordinator_idx_]->setLocalNeighbors(nodeId, neighbors);
    }

    bool CompositeFormationCoordinator::isNodeAssigned(const std::string &nodeId)
    {
        return coordinators_[active_coordinator_idx_]->isNodeAssigned(nodeId);
    }

    bool CompositeFormationCoordinator::isNodeInPosition(const std::string &sourceNodeId, const std::string &targetNodeId)
    {
        return coordinators_[active_coordinator_idx_]->isNodeInPosition(sourceNodeId, targetNodeId);
    }

    bool CompositeFormationCoordinator::isFormationComplete()
    {
        return coordinators_[active_coordinator_idx_]->isFormationComplete();
    }

    const NodeMessages CompositeFormationCoordinator::align(const std::string &nodeId, const NodePositions &relativeNodePositions)
    {
        return coordinators_[active_coordinator_idx_]->align(nodeId, relativeNodePositions);
    }

} // namespace ketu::formation
