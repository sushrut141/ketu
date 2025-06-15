#ifndef RANDOM_NODES_H
#define RANDOM_NODES_H
#include <memory>

#include "../objects/node.h"
#include "scenario.h"

namespace ketu::scenarios
{
    class RandomNodes : public Scenario
    {
    public:
        static std::unique_ptr<RandomNodes> create();

        void setup();

    private:
        RandomNodes(std::unique_ptr<ketu::world::World> world);

        void onTick(unsigned long long frameNumber);

        void onNodeUpdated(std::string nodeId, ketu::telemetry::Position positionDiff);

        std::unique_ptr<ketu::sensing::SensingClient> sensing_client_;
        std::unique_ptr<ketu::communication::CommunicationClient> communication_client_;
        std::unique_ptr<ketu::formation::FormationCoordinator> formationCoordinator_;

        std::vector<std::unique_ptr<ketu::objects::Node>> nodes_;
    };

} // namespace ketu::scenarios
#endif // RANDOM_NODES_H
