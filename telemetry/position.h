#ifndef POSITION_H
#define POSITION_H


namespace ketu::telemetry
{
    class Position {
    public:
        Position(double x, double y, double z);

        Position();

        static Position from(double x, double y, double z);

        Position operator+(const Position& other) const;

        double getX() const;
        double getY() const;
        double getZ() const;

    private:
        double x_;
        double y_;
        double z_;
    };

} // namespace ketu::telemetry




#endif //POSITION_H
