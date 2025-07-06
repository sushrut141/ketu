#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H
#include <memory>

#include "objects/node.h"
#include "scenario.h"

namespace ketu::scenarios
{
    class CollisionAvoidance : public Scenario
    {
    public:
        static std::unique_ptr<CollisionAvoidance> create();

        void setup() override;

        void onEntitySelected(const std::string& entityId) override;

        void onSelectedEntityMoved(const std::string& entityId, float xDistance, float yDistance) override;

    private:
        CollisionAvoidance(std::unique_ptr<ketu::world::World> world);

        void onTick(unsigned long long frameNumber) override;

        void onNodeUpdated(std::string nodeId, ketu::telemetry::Position positionDiff);

        std::unique_ptr<ketu::sensing::SensingClient> sensing_client_;
        std::unique_ptr<ketu::communication::CommunicationClient> communication_client_;
        std::unique_ptr<ketu::formation::FormationCoordinator> formationCoordinator_;

        std::vector<std::unique_ptr<ketu::objects::Node>> nodes_;

    };


} // namespace ketu::scenarios

#endif //COLLISION_AVOIDANCE_H
