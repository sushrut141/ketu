#include <cmath>

#include "position_utils.h"

namespace ketu::telemetry
{
    double distance(const Position& p1, const Position& p2)
    {
        return std::sqrt(std::pow(p1.getX() - p2.getX(), 2) + std::pow(p1.getY() - p2.getY(), 2) +
                         std::pow(p1.getZ() - p2.getZ(), 2));
    }

} // namespace ketu::telemetry
