#include <algorithm>

#include "movement.h"

namespace ketu::planning
{
    constexpr double POSITION_ERROR = 0.01f;

    ketu::communication::MessageType move(const ketu::telemetry::Position& source,
                                          const ketu::telemetry::Position& target)
    {
        double distance = ketu::telemetry::distance(source, target);
        if (distance < POSITION_ERROR)
        {
            return ketu::communication::MessageType::STOP;
        }
        auto diff = target - source;
        double arr[3] = {abs(diff.getX()), abs(diff.getY()), abs(diff.getZ())};
        auto max_it = std::max_element(arr, arr + 3);
        int max_index = std::distance(arr, max_it);

        switch (max_index)
        {
        case 0:
            return (diff.getX() > 0 ? ketu::communication::MessageType::MOVE_X_POSITIVE
                                    : ketu::communication::MessageType::MOVE_X_NEGATIVE);
        case 1:
            return (diff.getY() > 0 ? ketu::communication::MessageType::MOVE_Y_POSITIVE
                                    : ketu::communication::MessageType::MOVE_Y_NEGATIVE);
        case 2:
            return (diff.getZ() > 0 ? ketu::communication::MessageType::MOVE_Z_POSITIVE
                                    : ketu::communication::MessageType::MOVE_Z_NEGATIVE);
        default:
            return ketu::communication::MessageType::UNSPECIFIED;
        }
    }

} // namespace ketu::planning
