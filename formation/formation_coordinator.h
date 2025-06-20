#ifndef FORMATION_COORDINATOR_H
#define FORMATION_COORDINATOR_H

#include <string>
#include <unordered_map>

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

        // Specifies the maximum number of edges for each node in the formation.
        // Can be used to identify how many neighbors need to be brought into formation.
        virtual int maxConnectivity() = 0;

        // Specifies whether the current node and all it's neighbors are in formation.
        virtual bool isNodeLocallyFormed(const std::string& nodeId) = 0;

        // Get the nodes assigned to the local formation around a node.
        virtual std::vector<std::string> getLocalNeighbors(const std::string& nodeId) = 0;

        // Assign nodes to be local neighbors of a node in the formation.
        // Use this to assign candidate positions to nodes to be moved to.
        // Existing positions of node is maintained if called when called multiple times with
        // the same set of nodes.
        virtual void setLocalNeighbors(const std::string& nodeId, const std::vector<std::string> neighbors) = 0;

        // Specifies whether node is already in formation.
        // This does not imply its neighbors are also in formation.
        // Use isNodeLocallyFormed() for that.
        virtual bool isNodeFrozen(const std::string& nodeId) = 0;

        // Specifies whether all nodes are in formation.
        virtual bool isFormationComplete() = 0;

        // Compute the position diff for all neighbors of a node to complete a local formation.
        virtual const NodeMessages align(const std::string& nodeId, const NodePositions& relativeNodePositions) = 0;
    };

} // namespace ketu::formation

#endif // FORMATION_COORDINATOR_H
