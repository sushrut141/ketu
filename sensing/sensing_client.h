#ifndef SENSING_CLIENT_H
#define SENSING_CLIENT_H

#include "../world/world.h"

namespace ketu::sensing {

// Provides access to various types of sensors nodes may require while
// navigating through the world.
class SensingClient {
public:
    explicit SensingClient(const ketu::world::World* world);

private:
    const ketu::world::World* world_;
};

} // ketu::sensing

#endif // SENSING_CLIENT_H
