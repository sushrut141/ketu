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

        const std::vector<std::pair<std::string, double>>& getKNearestNeighbors(const std::string sourceNodeId, const int k);

    private:
        const ketu::world::World* world_;
    };

} // namespace ketu::sensing

#endif // SENSING_CLIENT_H
