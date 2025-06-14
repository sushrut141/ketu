#include "node.h"
#include <cassert>

namespace ketu::objects
{
    constexpr double MOVEMENT_STEP = 0.001;

    Node::Node(
        const std::string& node_id,
        const ketu::sensing::SensingClient* sensing_client,
        const ketu::communication::CommunicationClient* communication_client)
            : node_id_(node_id),
                sensing_client_(sensing_client),
                communication_client_(communication_client)
    {
    }

    const std::string& Node::getId() const
    {
        return this->node_id_;
    }

    void Node::setOnNodeUpdated(const std::function<void(ketu::telemetry::Position)>& callback)
    {
        onNodeUpdated_ = callback;
    }

    void Node::onMessage(const ketu::communication::MessageType& message_type)
    {
        switch (message_type)
        {
        case ketu::communication::MessageType::ANNEAL:
            onNodeAnneal_();
            break;
        case ketu::communication::MessageType::MOVE_X:
        case ketu::communication::MessageType::MOVE_Y:
        case ketu::communication::MessageType::MOVE_Z:
            onNodeMove_(message_type);
        default:
            break;
        }
    }


    void Node::onNodeMove_(const ketu::communication::MessageType& message_type)
    {
        ketu::telemetry::Position position;
        switch (message_type)
        {
        case ketu::communication::MessageType::MOVE_X:
            position = ketu::telemetry::Position(MOVEMENT_STEP, 0.0, 0.0);
        case ketu::communication::MessageType::MOVE_Y:
            position = ketu::telemetry::Position(0.0, MOVEMENT_STEP, 0.0);
        case ketu::communication::MessageType::MOVE_Z:
            position = ketu::telemetry::Position(0.0, 0.0, MOVEMENT_STEP);
        default:
            assert(false, "Unknown message type encountered");
        }
        onNodeUpdated_(position);
    }

    void Node::onNodeAnneal_()
    {

    }


} // namespace ketu::objects
