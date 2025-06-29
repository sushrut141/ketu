#include <iostream>

#include "mesh_based_formation_coordinator.h"

#include "../planning/movement.h"

namespace ketu::formation
{

    MeshBasedFormationCoordinator::MeshBasedFormationCoordinator(const std::string& meshPath,
                                                                 const ketu::world::World* world) :
        FormationCoordinator(), world_(world)
    {
        loader_ = std::move(ketu::thirdparty::tinyobj::Loader::fromOBJFile(meshPath));
        nodeSlotMapping_ = {};
        availableSlots_ = {};
        for (int i = 0 ; i < loader_->getVertices().size() ; i += 1)
        {
            availableSlots_.insert(i);
        }
    }

    int MeshBasedFormationCoordinator::maxConnectivity() { return 0; }

    int MeshBasedFormationCoordinator::getMaxNeighBorCount(const std::string& nodeId)
    {
        if (nodeSlotMapping_.find(nodeId) != nodeSlotMapping_.end())
        {
            const int slot = nodeSlotMapping_[nodeId];
            const auto& connectivity = loader_->getConnectivity().at(slot);
            return connectivity.size();
        }
        return 0;
    }


    bool MeshBasedFormationCoordinator::isNodeLocallyFormed(const std::string& nodeId)
    {
        if (!isNodeAssigned(nodeId))
        {
            return false;
        }
        const int nodeIdx = nodeSlotMapping_[nodeId];
        const auto neighbors = getLocalNeighbors(nodeId);
        const auto& connectivity = loader_->getConnectivity().at(nodeIdx);
        if (neighbors.size() != connectivity.size())
        {
            return false;
        }

        for (const std::string& neighbor: neighbors)
        {
            if (!isNodeInPosition(nodeId, neighbor))
            {
                return false;
            }
        }
        return true;
    }

    std::vector<std::string> MeshBasedFormationCoordinator::getLocalNeighbors(const std::string& nodeId)
    {
        if (!isNodeAssigned(nodeId))
        {
            return {};
        }
        const int nodeIdx = nodeSlotMapping_[nodeId];
        const auto& connectivity = loader_->getConnectivity().at(nodeIdx);

        std::vector<std::string> neighbors;
        for (const auto& nodeIdIdxPair: nodeSlotMapping_)
        {
            if (std::find(connectivity.begin(), connectivity.end(), nodeIdIdxPair.second) != connectivity.end())
            {
                neighbors.push_back(nodeIdIdxPair.first);
            }
            if (neighbors.size() == connectivity.size())
            {
                break;
            }
        }
        return neighbors;
    }

    void MeshBasedFormationCoordinator::setLocalNeighbors(const std::string& nodeId,
                                                          const std::vector<std::string> neighbors)
    {
        if (!isNodeAssigned(nodeId))
        {
            int nodeIdx = *availableSlots_.begin();
            nodeSlotMapping_[nodeId] = nodeIdx;
            availableSlots_.erase(nodeIdx);
        }
        const int nodeIdx = nodeSlotMapping_[nodeId];
        const auto& connectivity = loader_->getConnectivity().at(nodeIdx);
        int neighborPtr = 0;
        int connectivityPtr = 0;
        while (neighborPtr < neighbors.size() && connectivityPtr < connectivity.size())
        {
            const std::string& neighbor = neighbors[neighborPtr];
            const int connectivityIdx = connectivity[connectivityPtr];
            if (nodeSlotMapping_.find(neighbor) != nodeSlotMapping_.end())
            {
                neighborPtr += 1;
                continue;
            }
            if (availableSlots_.find(connectivityIdx) == availableSlots_.end())
            {
                connectivityPtr += 1;
                continue;
            }
            nodeSlotMapping_[neighbor] = connectivityIdx;
            availableSlots_.erase(connectivityIdx);

            neighborPtr += 1;
            connectivityPtr += 1;
        }
    }

    bool MeshBasedFormationCoordinator::isNodeFrozen(const std::string& nodeId) { return false; }

    bool MeshBasedFormationCoordinator::isNodeAssigned(const std::string& nodeId)
    {
        return nodeSlotMapping_.find(nodeId) != nodeSlotMapping_.end();
    }

    bool MeshBasedFormationCoordinator::isNodeInPosition(const std::string& sourceNodeId,
                                                         const std::string& targetNodeId)
    {
        const bool sourceNotAssigned = !isNodeAssigned(sourceNodeId);
        const bool targetNotAssigned = !isNodeAssigned(targetNodeId);
        if (sourceNotAssigned || targetNotAssigned)
        {
            return false;
        }
        const int sourceIdx = nodeSlotMapping_[sourceNodeId];
        const int targetIdx = nodeSlotMapping_[targetNodeId];
        const auto expectedTargetPosition = loader_->getVertices().at(targetIdx) - loader_->getVertices().at(sourceIdx);
        const auto targetCurrentPosition = world_->getNodePosition(targetNodeId) - world_->getNodePosition(sourceNodeId);
        return ketu::planning::move(targetCurrentPosition, expectedTargetPosition) == ketu::communication::MessageType::STOP;
    }

    bool MeshBasedFormationCoordinator::isFormationComplete() {}

    const NodeMessages MeshBasedFormationCoordinator::align(const std::string& nodeId,
                                                            const NodePositions& relativeNodePositions)
    {

        if (!isNodeAssigned(nodeId))
        {
            throw std::runtime_error("MeshBasedFormationCoordinator::align: node not assigned");
        }
        NodeMessages messages;
        const int nodeIdx = nodeSlotMapping_.at(nodeId);
        const auto& connectivity = loader_->getConnectivity().at(nodeIdx);
        for (const auto& [neighborId, neighborRelativePosition] : relativeNodePositions)
        {
            const int neighborIdx = nodeSlotMapping_[neighborId];
            const auto& expectedNeighborPosition = loader_->getVertices().at(neighborIdx) - loader_->getVertices().at(nodeIdx);
            const ketu::communication::MessageType message = ketu::planning::move(neighborRelativePosition, expectedNeighborPosition);
            messages.insert({neighborId, message});
            if (message == ketu::communication::MessageType::STOP)
            {
                std::cout << "Node " << neighborId << " is in target position" << std::endl;
            }
        }
        return messages;
    }

} // namespace ketu::formation
