#include "position.h"

namespace ketu::telemetry
{
    Position::Position(double x, double y, double z):
    x_(x), y_(y), z_(z) {}

    Position::Position():
    x_(0.0), y_(0.0), z_(0.0) {}

    double Position::getX() const { return x_; }

    double Position::getY() const { return y_; }

    double Position::getZ() const { return z_; }

    Position Position::from(double x, double y, double z)
    {
        Position p(x, y, z);
        return p;
    }

    Position Position::operator+(const Position& rhs) const
    {
        return Position(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
    }

} // namespace ketu::telemetry
