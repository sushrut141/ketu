#ifndef GRID_FORMATIONCOORDINATOR_H
#define GRID_FORMATIONCOORDINATOR_H

#include <unordered_set>

#include "../world/world.h"
#include "formation_coordinator.h"

namespace ketu::formation
{
    class GridFormationCoordinator : public FormationCoordinator {

    public:
        GridFormationCoordinator(const ketu::world::World* world);

        // Specifies whether supplied nodes are in formation.
        bool isFormed() override;

        // Move the available nodes in vicinity of source node into formation.
        // The method accepts a map of nodeId and a pair of (distance, relativePosition)
        // to the source node.
        const std::unordered_map<std::string, ketu::communication::MessageType>
        computeAlignment(const std::vector<std::pair<std::string, std::pair<double, ketu::telemetry::Position>>>& nodes) override;

    private:
        const ketu::world::World* world_;
        std::unordered_set<std::string> frozen_nodes_;
    };

}  // ketu::formation

#endif //GRID_FORMATIONCOORDINATOR_H
