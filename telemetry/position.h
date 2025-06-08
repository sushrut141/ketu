#ifndef POSITION_H
#define POSITION_H


namespace ketu::telemetry
{
    class Position {
    public:
        Position(double x, double y, double z);

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
