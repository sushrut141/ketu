#include "random_nodes.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <valarray>

#include "../communication/interfaces.h"
#include "../formation/grid_formation_coordinator.h"

#include "../objects/node.h"

namespace ketu::scenarios
{
    std::unique_ptr<RandomNodes> RandomNodes::create()
    {
        srand(static_cast<unsigned int>(0));
        auto world = std::make_unique<ketu::world::World>();
        return std::unique_ptr<RandomNodes>(new RandomNodes(std::move(world)));
    }

    RandomNodes::RandomNodes(std::unique_ptr<ketu::world::World> world) : Scenario(std::move(world))
    {
        this->sensing_client_ = std::make_unique<ketu::sensing::SensingClient>(this->world_.get());
        this->communication_client_ = std::make_unique<ketu::communication::CommunicationClient>(this->world_.get());
        this->formationCoordinator_ = std::make_unique<ketu::formation::GridFormationCoordinator>(world_.get());
    }

    void RandomNodes::setup()
    {
        auto leader = std::make_unique<ketu::objects::Node>("leader", sensing_client_.get(),
                                                            communication_client_.get(), formationCoordinator_.get());
        std::function<void(std::string, ketu::telemetry::Position)> nodeUpdateCallback =
            std::bind(&RandomNodes::onNodeUpdated, this, std::placeholders::_1, std::placeholders::_2);
        leader->setOnNodeUpdated(nodeUpdateCallback);
        world_->addNode(leader->getId(), ketu::telemetry::Position::from(1.0, 1.0, 0.5));
        nodes_.push_back(std::move(leader));


        for (int i = 0; i < 7; i += 1)
        {
            std::string followerId = "follower_" + std::to_string(i + 1);
            double randX = 5.0 * static_cast<double>(rand()) / RAND_MAX;
            double randY = 5.0 * static_cast<double>(rand()) / RAND_MAX;
            double randZ = 5.0 * static_cast<double>(rand()) / RAND_MAX;

            auto follower = std::make_unique<ketu::objects::Node>(
                followerId, sensing_client_.get(), communication_client_.get(), formationCoordinator_.get());

            follower->setOnNodeUpdated(nodeUpdateCallback);
            world_->addNode(follower->getId(), ketu::telemetry::Position::from(randX, randY, randZ));
            nodes_.push_back(std::move(follower));
        }
    }

    void RandomNodes::onTick(unsigned long long frameNumber)
    {
        auto leaderId = nodes_[0]->getId();
        communication_client_->sendMessage(leaderId, ketu::communication::MessageType::ANNEAL);
    }

    void RandomNodes::onNodeUpdated(std::string nodeId, ketu::telemetry::Position positionDiff)
    {
        const auto& position = world_->getNodePosition(nodeId);
        auto updatedPosition = position + positionDiff;

        std::cout << "RandomNodes::onTick() for frame " << updatedPosition.getX() << " " << updatedPosition.getY()
                  << " " << updatedPosition.getZ() << std::endl;

        world_->updateNode(nodeId, updatedPosition);
    }


} // namespace ketu::scenarios
