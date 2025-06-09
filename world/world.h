#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>

#include "../telemetry/position.h"

namespace ketu::world
{

    // Renders the nodes in the works while abstracting away any
    // OpenSceneGraph functionality.
    class World
    {
    public:
        World();

        // Adds a node to the scene.
        bool addNode(const std::string& node_id, const ketu::telemetry::Position& position);

        bool updateNode(const std::string& node_id, const ketu::telemetry::Position& position);

        const std::unordered_map<std::string, ketu::telemetry::Position>& getNodePositions() const;

    private:
        std::unordered_map<std::string, ketu::telemetry::Position> node_positions_map_;
    };

} // ketu::world

#endif //WORLD_H
