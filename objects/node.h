#ifndef NODE_H
#define NODE_H

#include "world.h"
#include "../sensing/sensing_client.h"

namespace ketu::objects
{

    class Node {
    public:
        explicit Node(const ketu::sensing::SensingClient* sensing_client);

    private:
        const ketu::sensing::SensingClient* sensing_client_;
    };

} // namespace ketu::objects

#endif // NODE_H
