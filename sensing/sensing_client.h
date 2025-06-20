#ifndef SENSING_CLIENT_H
#define SENSING_CLIENT_H

#include <unordered_map>

#include "../world/world.h"

namespace ketu::sensing
{

    // Provides access to various types of sensors nodes may require while
    // navigating through the world.
    class SensingClient
    {
    public:
        explicit SensingClient(const ketu::world::World* world);

        // Returns the k nearest neighbors and their relative positions w.r.t
        // to the current node.
        std::unordered_map<std::string, telemetry::Position> getKNearestNeighbors(const std::string sourceNodeId,
                                                                                  const int k) const;

        // Returns the distance to the supplied nodes.
        std::unordered_map<std::string, telemetry::Position> getDistanceToNodes(const std::string sourceNodeId,
                                                                                const std::vector<std::string>& nodes) const;

    private:
        const ketu::world::World* world_;
    };

} // namespace ketu::sensing

#endif // SENSING_CLIENT_H
