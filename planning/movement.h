#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "communication/interfaces.h"
#include "telemetry/position_utils.h"

namespace ketu::planning
{

    ketu::communication::MessageType move(const ketu::telemetry::Position& source,
                                          const ketu::telemetry::Position& target);

    ketu::communication::MessageType move_within_distance(const ketu::telemetry::Position& source,
                                          const ketu::telemetry::Position& target, double proximityDistance);

} // namespace ketu::planning

#endif // MOVEMENT_H
