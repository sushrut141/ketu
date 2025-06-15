#ifndef SENSING_CLIENT_H
#define SENSING_CLIENT_H

#include <utility>

#include "../world/world.h"

namespace ketu::sensing
{

    // Provides access to various types of sensors nodes may require while
    // navigating through the world.
    class SensingClient
    {
    public:
        explicit SensingClient(const ketu::world::World* world);

        // Returns the ids of the k nearest nodes along with distance and relative position w.r.t
        // the source node.
        const std::vector<std::pair<std::string, std::pair<double, ketu::telemetry::Position>>>
            getKNearestNeighbors(const std::string sourceNodeId, const int k);

    private:
        const ketu::world::World* world_;
    };

} // namespace ketu::sensing

#endif // SENSING_CLIENT_H
