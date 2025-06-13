#include "random_nodes.h"

#include <iostream>
#include <cmath>
#include <valarray>

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
        ketu::telemetry::Position position(1.0, 1.0, 0.5);

        nodes_.push_back(node);
        world_->addNode(node.getId(), position);
    }

    void RandomNodes::onTick(unsigned long long frameNumber)
    {
        for (const auto& node : nodes_)
        {
            const auto& position = world_->getNodePosition(node.getId());

            double newX = 2.0 * std::sin(frameNumber * M_PI / 180000);
            double newY = 2.0 * std::cos(frameNumber * M_PI / 180000);
            double newZ = 2.0 * std::tan(frameNumber * M_PI / 180000);

            std::cout << "RandomNodes::onTick() for frame " << newX  << " " << newY << " " << newZ << std::endl;

            world_->updateNode(node.getId(), ketu::telemetry::Position(newX, newY, newZ));
        }
    }

} // namespace ketu::scenarios
