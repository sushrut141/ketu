
#include "../telemetry/position.h"
#include "grid_formation_coordinator.h"

namespace ketu::formation
{

    GridFormationCoordinator::GridFormationCoordinator(const ketu::world::World* world) :
        FormationCoordinator(), world_(world)
    {
        frozen_nodes_ = {};
    }

    int GridFormationCoordinator::maxConnectivity() {}

    bool GridFormationCoordinator::isNodeLocallyFormed(const std::string& nodeId) {}

    std::vector<std::string> GridFormationCoordinator::getLocalNeighbors(const std::string& nodeId) {}

    void GridFormationCoordinator::setLocalNeighbors(const std::string& nodeId,
                                                     const std::vector<std::string> neighbors)
    {
    }

    bool GridFormationCoordinator::isNodeFrozen(const std::string& nodeId) {}

    bool GridFormationCoordinator::isFormationComplete() {}

    const NodeMessages GridFormationCoordinator::align(const std::string& nodeId,
                                                       const NodePositions& relativeNodePositions)
    {
    }

} // namespace ketu::formation
