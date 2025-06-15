#ifndef FORMATION_COORDINATOR_H
#define FORMATION_COORDINATOR_H

#include <unordered_map>

#include "../communication/interfaces.h"

namespace ketu::formation
{
    // Computes node positions and maintains state to align them into a formation.
    class FormationCoordinator
    {

    public:

        virtual ~FormationCoordinator() {}

        // Specifies whether supplied nodes are in formation.
        virtual bool isFormed() = 0;

        // Move the available nodes in vicinity of source node into formation.
        // The method accepts a map of nodeId and a pair of (distance, relativePosition)
        // to the source node.
        virtual const std::unordered_map<std::string, ketu::communication::MessageType>
        computeAlignment(const std::vector<std::pair<std::string, std::pair<double, ketu::telemetry::Position>>>& nodes) = 0;
    };

} // namespace ketu::formation

#endif // FORMATION_COORDINATOR_H
