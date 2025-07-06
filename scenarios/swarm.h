#ifndef SWARM_H
#define SWARM_H

#include <memory>

#include "../objects/node.h"
#include "../formation/composite_formation_coordinator.h"
#include "scenario.h"

namespace ketu::scenarios
{
    class Swarm : public Scenario
    {
    public:
        static std::unique_ptr<Swarm> create();

        void setup() override;

        void onEntitySelected(const std::string& entityId) override;

        void onSelectedEntityMoved(const std::string& entityId, float xDistance, float yDistance) override;

    private:
        Swarm(std::unique_ptr<ketu::world::World> world);

        void onTick(unsigned long long frameNumber) override;

        void onNodeUpdated(std::string nodeId, ketu::telemetry::Position positionDiff);

        std::unique_ptr<ketu::sensing::SensingClient> sensing_client_;
        std::unique_ptr<ketu::communication::CommunicationClient> communication_client_;
        std::unique_ptr<ketu::formation::CompositeFormationCoordinator> formationCoordinator_;

        std::vector<std::unique_ptr<ketu::formation::FormationCoordinator>> coordinators_;

        std::vector<std::unique_ptr<ketu::objects::Node>> nodes_;

        double frameCounter_;
        bool cycle_;
    };

} // namespace ketu::scenarios

#endif //SWARM_H
