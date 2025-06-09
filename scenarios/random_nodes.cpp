#include "random_nodes.h"

#include <iostream>
#include "../objects/node.h"

namespace ketu::scenarios
{
    std::unique_ptr<RandomNodes> RandomNodes::create()
    {
        auto world = std::make_unique<ketu::world::World>();
        return std::unique_ptr<RandomNodes>(new RandomNodes(std::move(world)));
    }

    RandomNodes::RandomNodes(std::unique_ptr<ketu::world::World> world)
        : Scenario(std::move(world))
    {
        this->sensing_client_ = std::make_unique<ketu::sensing::SensingClient>(this->world_.get());
        this->communication_client_ = std::make_unique<ketu::communication::CommunicationClient>(this->world_.get());
    }

    void RandomNodes::setup()
    {
        ketu::objects::Node node("sphere_1", sensing_client_.get(), communication_client_.get());
        ketu::telemetry::Position position(1.0, 1.0, 100.5);

        nodes_.push_back(node);
        world_->addNode(node.getId(), position);
    }

    void RandomNodes::onNodeUpdated()
    {

    }

    void RandomNodes::onTick()
    {
        Scenario::onTick();
        std::cout << "RandomNodes::onTick()" << std::endl;
    }

} // namespace ketu::scenarios
