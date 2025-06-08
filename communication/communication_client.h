#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "world.h"

namespace ketu::communication
{
    class CommunicationClient {
    public:
        explicit CommunicationClient(const ketu::world::World* world);

    private:
        const ketu::world::World* world_;
    };

} // namespace ketu::communication

#endif //COMMUNICATION_H
