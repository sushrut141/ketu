#include "random_nodes.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <valarray>

#include "../communication/interfaces.h"

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
    }

    void RandomNodes::setup()
    {
        auto node =
            std::make_unique<ketu::objects::Node>("sphere_1", sensing_client_.get(), communication_client_.get());
        std::function<void(std::string, ketu::telemetry::Position)> nodeUpdateCallback =
            std::bind(&RandomNodes::onNodeUpdated, this, std::placeholders::_1, std::placeholders::_2);
        node->setOnNodeUpdated(nodeUpdateCallback);

        ketu::telemetry::Position position(1.0, 1.0, 0.5);
        world_->addNode(node->getId(), position);

        nodes_.push_back(std::move(node));
    }

    void RandomNodes::onTick(unsigned long long frameNumber)
    {
        for (const auto& node : nodes_)
        {
            const std::string& nodeId = node->getId();
            double randomNumber = static_cast<double>(rand()) / RAND_MAX;
            if (randomNumber < 0.3)
            {
                communication_client_->sendMessage(nodeId, ketu::communication::MessageType::MOVE_X);
            }
            else if (randomNumber < 0.6)
            {
                communication_client_->sendMessage(nodeId, ketu::communication::MessageType::MOVE_Y);
            }
            else
            {
                communication_client_->sendMessage(nodeId, ketu::communication::MessageType::MOVE_Z);
            }
        }
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
