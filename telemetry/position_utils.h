#ifndef POSITION_UTILS_H
#define POSITION_UTILS_H

#include "position.h"

namespace ketu::telemetry
{
    // Gets the distance between the two points in the world.
    double distance(const Position& p1, const Position& p2);

} // namespace ketu::telemetry

#endif // POSITION_UTILS_H
