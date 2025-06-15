
#include "../telemetry/position.h"
#include "grid_formation_coordinator.h"

namespace ketu::formation
{

    const std::vector<ketu::telemetry::Position> GRID_RELATIVE_POSITIONS = {
        // Top.
        ketu::telemetry::Position::from(0.0, 0.0, 1.0),
        // Bottom.
        ketu::telemetry::Position::from(0.0, 0.0, -1.0),
        // Left.
        ketu::telemetry::Position::from(-1.0, 0.0, 0.0),
        // Right.
        ketu::telemetry::Position::from(1.0, 0.0, 0.0),
        // Ahead.
        ketu::telemetry::Position::from(0.0, 1.0, 0.0),
        // Backward.
        ketu::telemetry::Position::from(0.0, -1.0, 0.0),
    };

    GridFormationCoordinator::GridFormationCoordinator( const ketu::world::World* world)
        : FormationCoordinator(), world_()
    {
        frozen_nodes_ = {};
    }

    bool GridFormationCoordinator::isFormed()
    {
        return world_->getNodePositions().size() == frozen_nodes_.size();
    }

    const std::unordered_map<std::string, ketu::communication::MessageType> GridFormationCoordinator::computeAlignment(
        const std::vector<std::pair<std::string, std::pair<double, ketu::telemetry::Position>>>& nodes)
    {


    }




} // namespace ketu::formation
