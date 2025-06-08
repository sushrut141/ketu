#include "position.h"

namespace ketu::telemetry
{
    Position::Position(double x, double y, double z):
    x_(x), y_(y), z_(z) {}


    double Position::getX() const { return x_; }

    double Position::getY() const { return y_; }

    double Position::getZ() const { return z_; }

} // namespace ketu::telemetry
