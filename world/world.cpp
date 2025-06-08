#include "world.h"

#include <vector>
#include <string>


namespace ketu::world {

World::World() {}

bool World::addNode(const std::string& node_id, const ketu::telemetry::Position& position)
{
    auto inserted = this->node_positions_map_.insert(std::make_pair(node_id, position));
    return inserted.second;
}

bool World::updateNode(const std::string& node_id, const ketu::telemetry::Position& position)
{
    auto it = this->node_positions_map_.find(node_id);
    if (it == this->node_positions_map_.end())
    {
        it->second = position;
        return true;
    }
    return false;
}

} // namespace ketu::world
