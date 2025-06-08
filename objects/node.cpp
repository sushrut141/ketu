#include "node.h"

namespace ketu::objects
{
    using ketu::world::World;

    Node::Node(const ketu::world::World* world)
    {
        this->world_ = world;
    }


} // namespace ketu::objects
