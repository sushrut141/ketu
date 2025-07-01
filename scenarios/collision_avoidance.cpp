#include "collision_avoidance.h"
#include <functional>
#include <iostream>
#include <valarray>

#include "../communication/interfaces.h"

#include "../objects/node.h"
#include "../formation/mesh_based_formation_coordinator.h"

namespace ketu::scenarios
{

    std::unique_ptr<CollisionAvoidance> CollisionAvoidance::create()
    {
        srand(static_cast<unsigned int>(0));
        auto world = std::make_unique<ketu::world::World>();
        return std::unique_ptr<CollisionAvoidance>(new CollisionAvoidance(std::move(world)));
    }

    CollisionAvoidance::CollisionAvoidance(std::unique_ptr<ketu::world::World> world) : Scenario(std::move(world))
    {
        this->sensing_client_ = std::make_unique<ketu::sensing::SensingClient>(this->world_.get());
        this->communication_client_ = std::make_unique<ketu::communication::CommunicationClient>(this->world_.get());
        this->formationCoordinator_ =
            std::make_unique<ketu::formation::MeshBasedFormationCoordinator>("cylinder.obj", world_.get());
    }

    void CollisionAvoidance::setup()
    {
        auto leader = std::make_unique<ketu::objects::Node>("leader", sensing_client_.get(),
                                                            communication_client_.get(), formationCoordinator_.get());
        std::function<void(std::string, ketu::telemetry::Position)> nodeUpdateCallback =
            std::bind(&CollisionAvoidance::onNodeUpdated, this, std::placeholders::_1, std::placeholders::_2);
        leader->setOnNodeUpdated(nodeUpdateCallback);
        world_->addNode(leader->getId(), ketu::telemetry::Position::from(0.0, 0.0, 0.0));
        nodes_.push_back(std::move(leader));

        auto xMover = std::make_unique<ketu::objects::Node>("x_movable", sensing_client_.get(),
                                                            communication_client_.get(), formationCoordinator_.get());
        xMover->setOnNodeUpdated(nodeUpdateCallback);
        world_->addNode(xMover->getId(), ketu::telemetry::Position::from(10.0, 0.0, 0.0));
        nodes_.push_back(std::move(xMover));

        auto yMover = std::make_unique<ketu::objects::Node>("y_movable", sensing_client_.get(),
                                                            communication_client_.get(), formationCoordinator_.get());
        yMover->setOnNodeUpdated(nodeUpdateCallback);
        world_->addNode(yMover->getId(), ketu::telemetry::Position::from(0.0, 10.0, 0.0));
        nodes_.push_back(std::move(yMover));

        auto zMover = std::make_unique<ketu::objects::Node>("z_movable", sensing_client_.get(),
                                                            communication_client_.get(), formationCoordinator_.get());
        zMover->setOnNodeUpdated(nodeUpdateCallback);
        world_->addNode(zMover->getId(), ketu::telemetry::Position::from(0.0, 0.0, 10.0));
        nodes_.push_back(std::move(zMover));
    }

    void CollisionAvoidance::onTick(unsigned long long frameNumber)
    {
        auto xMovableId = nodes_[1]->getId();
        auto yMovableId = nodes_[2]->getId();
        auto zMovableId = nodes_[3]->getId();

        communication_client_->sendMessage(xMovableId, ketu::communication::MessageType::MOVE_X_NEGATIVE);
        communication_client_->sendMessage(yMovableId, ketu::communication::MessageType::MOVE_Y_NEGATIVE);
        // communication_client_->sendMessage(zMovableId, ketu::communication::MessageType::MOVE_Z_NEGATIVE);
    }

    void CollisionAvoidance::onNodeUpdated(std::string nodeId, ketu::telemetry::Position positionDiff)
    {
        const auto& position = world_->getNodePosition(nodeId);
        auto updatedPosition = position + positionDiff;
        world_->updateNode(nodeId, updatedPosition);
    }

    void CollisionAvoidance::onEntitySelected(const std::string& entityId) {}

    void CollisionAvoidance::onSelectedEntityMoved(const std::string& entityId, float xDistance, float yDistance) {}

} // namespace ketu::scenarios
