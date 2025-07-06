#include "swarm.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <valarray>

#include "../communication/interfaces.h"
#include "../formation/mesh_based_formation_coordinator.h"

#include "../objects/node.h"

namespace ketu::scenarios
{

    constexpr int NUM_FOLLOWERS = 11;
    constexpr double SCALING = 5.0;

    std::unique_ptr<Swarm> Swarm::create()
    {
        srand(static_cast<unsigned int>(0));
        auto world = std::make_unique<ketu::world::World>();
        return std::unique_ptr<Swarm>(new Swarm(std::move(world)));
    }

    Swarm::Swarm(std::unique_ptr<ketu::world::World> world) : Scenario(std::move(world))
    {
        this->sensing_client_ = std::make_unique<ketu::sensing::SensingClient>(this->world_.get());
        this->communication_client_ = std::make_unique<ketu::communication::CommunicationClient>(this->world_.get());

        coordinators_.push_back(
            std::make_unique<ketu::formation::MeshBasedFormationCoordinator>("icosahedron.obj", 1.0, world_.get()));
        coordinators_.push_back(
            std::make_unique<ketu::formation::MeshBasedFormationCoordinator>("plane.obj", 2.0, world_.get()));
        std::vector<ketu::formation::FormationCoordinator*> coordinators;
        coordinators.push_back(coordinators_[0].get());
        coordinators.push_back(coordinators_[1].get());

        this->formationCoordinator_ = std::make_unique<ketu::formation::CompositeFormationCoordinator>(coordinators);
        ;
        frameCounter_ = 0.0f;
    }

    void Swarm::setup()
    {
        auto leader = std::make_unique<ketu::objects::Node>("leader", sensing_client_.get(),
                                                            communication_client_.get(), formationCoordinator_.get());
        std::function<void(std::string, ketu::telemetry::Position)> nodeUpdateCallback =
            std::bind(&Swarm::onNodeUpdated, this, std::placeholders::_1, std::placeholders::_2);
        leader->setOnNodeUpdated(nodeUpdateCallback);
        world_->addNode(leader->getId(), ketu::telemetry::Position::from(SCALING * cos(0), 1.0, SCALING * sin(0)));
        nodes_.push_back(std::move(leader));


        for (int i = 0; i < NUM_FOLLOWERS; i += 1)
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

    void Swarm::onTick(unsigned long long frameNumber)
    {
        auto leaderId = nodes_[0]->getId();
        if (frameNumber < 1000)
        {
            std::cout << "Frame number " << frameNumber << std::endl;
            communication_client_->sendMessage(leaderId, ketu::communication::MessageType::ANNEAL);
            return;
        }

        double y = 1.0 + (frameCounter_ / 10);

        double x = SCALING * cos(frameCounter_);
        double z = SCALING * sin(frameCounter_);

        auto position = ketu::telemetry::Position::from(x, y, z);
        world_->updateNode(leaderId, position);

        frameCounter_ += 0.005;

        std::cout << "Frame counter : " << frameCounter_ << " frameNumber: " << frameNumber << std::endl;
        int frameSection = ((int)(frameCounter_)) % 5;
        if (frameCounter_ > 0 && frameSection < 2)
        {
            formationCoordinator_->switchCoordinator(0);
        } else
        {
            formationCoordinator_->switchCoordinator(1);
        }


        communication_client_->sendMessage(leaderId, ketu::communication::MessageType::ANNEAL);
    }

    void Swarm::onEntitySelected(const std::string& entityId)
    {
        std::cout << "selected entity id " << entityId << std::endl;
    }

    void Swarm::onNodeUpdated(std::string nodeId, ketu::telemetry::Position positionDiff)
    {
        const auto& position = world_->getNodePosition(nodeId);
        auto updatedPosition = position + positionDiff;
        world_->updateNode(nodeId, updatedPosition);
    }

    void Swarm::onSelectedEntityMoved(const std::string& entityId, float xDistance, float yDistance)
    {
        const auto& position = world_->getNodePosition(entityId);
        auto newPosition = ketu::telemetry::Position::from(position.getX() - (0.01 * xDistance),
                                                           position.getY() - (0.01 * yDistance), position.getZ());
        world_->updateNode(entityId, newPosition);
    }


} // namespace ketu::scenarios
