#include "sensing_client.h"

#include <queue>
#include <unordered_map>

#include "../telemetry/position_utils.h"

namespace ketu::sensing
{

    SensingClient::SensingClient(const ketu::world::World* world) { this->world_ = world; }

    std::unordered_map<std::string, telemetry::Position>
    SensingClient::getKNearestNeighbors(const std::string sourceNodeId, const int k)
    {
        const ketu::telemetry::Position& sourcePosition = world_->getNodePosition(sourceNodeId);
        const std::unordered_map<std::string, telemetry::Position>& neighbors = world_->getNodePositions();

        std::priority_queue<std::pair<double, std::string>> max_heap;
        for (const auto& nodeIdPositionPair : neighbors)
        {
            const std::string targetNodeId = nodeIdPositionPair.first;
            if (sourceNodeId == targetNodeId)
            {
                continue;
            }
            const ketu::telemetry::Position& targetPosition = nodeIdPositionPair.second;
            double dist = ketu::telemetry::distance(sourcePosition, targetPosition);

            if (max_heap.size() < k)
            {
                max_heap.push({dist, targetNodeId});
            }
            else if (dist < max_heap.top().first)
            {
                max_heap.pop();
                max_heap.push({dist, targetNodeId});
            }
        }

        std::unordered_map<std::string, telemetry::Position> targetPositions;
        while (!max_heap.empty())
        {
            auto nearestNodeId = max_heap.top().second;
            auto nearestPosition = neighbors.at(nearestNodeId);
            auto relativePosition = nearestPosition - sourcePosition;
            targetPositions.insert({nearestNodeId, relativePosition});
            max_heap.pop();
        }

        return targetPositions;
    }


} // namespace ketu::sensing
