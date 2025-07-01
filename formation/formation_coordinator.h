#ifndef FORMATION_COORDINATOR_H
#define FORMATION_COORDINATOR_H

#include <string>
#include <unordered_map>

#include "../telemetry/position.h"
#include "../communication/interfaces.h"

namespace ketu::formation
{

    using NodeMessages = std::unordered_map<std::string, ketu::communication::MessageType>;
    using NodePositions = std::unordered_map<std::string, ketu::telemetry::Position>;

    // Computes node positions and maintains state to align them into a formation.
    class FormationCoordinator
    {

    public:
        virtual ~FormationCoordinator() {}

        // Get the maximum number of connected neighbors for the supplied node.
        virtual int getMaxNeighborCount(const std::string& nodeId) = 0;

        // Specifies whether the current node and all it's neighbors are in formation.
        virtual bool isNodeLocallyFormed(const std::string& nodeId) = 0;

        // Get the nodes assigned to the local formation around a node.
        virtual std::vector<std::string> getLocalNeighbors(const std::string& nodeId) = 0;

        // Assign nodes to be local neighbors of a node in the formation.
        // Use this to assign candidate positions to nodes to be moved to.
        // Existing positions of node is maintained if called when called multiple times with
        // the same set of nodes.
        virtual void setLocalNeighbors(const std::string& nodeId, const std::vector<std::string> neighbors) = 0;

        // Checks whether supplied node is already part of another node's formation.
        virtual bool isNodeAssigned(const std::string& nodeId) = 0;

        // Checks if the supplied target is in formation w.r.t to the source node.
        virtual bool isNodeInPosition(const std::string& sourceNodeId, const std::string& targetNodeId) = 0;

        // Specifies whether all nodes are in formation.
        virtual bool isFormationComplete() = 0;

        // Compute the position diff for all neighbors of a node to complete a local formation.
        virtual const NodeMessages align(const std::string& nodeId, const NodePositions& relativeNodePositions) = 0;
    };

} // namespace ketu::formation

#endif // FORMATION_COORDINATOR_H
