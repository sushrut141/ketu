#ifndef RANDOM_NODES_H
#define RANDOM_NODES_H
#include <memory>

#include "scenario.h"
#include "../objects/node.h"

namespace ketu::scenarios
{
    class RandomNodes: public Scenario
    {
    public:
        static std::unique_ptr<RandomNodes> create();

        void setup();

        void onTick();

        void onNodeUpdated();
    private:
        RandomNodes(std::unique_ptr<ketu::world::World> world);

        std::unique_ptr<ketu::sensing::SensingClient> sensing_client_;
        std::unique_ptr<ketu::communication::CommunicationClient> communication_client_;

        std::vector<ketu::objects::Node> nodes_;
    };

} // namespace ketu::scenarios
#endif //RANDOM_NODES_H
