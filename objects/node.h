#ifndef NODE_H
#define NODE_H

#include "world.h"

namespace ketu::objects
{

    class Node {
    public:
        explicit Node(const ketu::world::World* world);

    private:
        const ketu::world::World* world_;
    };

} // namespace ketu::objects

#endif //NODE_H
