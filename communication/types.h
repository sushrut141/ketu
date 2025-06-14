#ifndef TYPES_H
#define TYPES_H


namespace ketu::communication
{
    enum MessageType
    {
        UNSPECIFIED = 0,
        // Move along X Axis wrt North.
        MOVE_X = 1,
        // Move along Y Axis wrt North.
        MOVE_Y = 2,
        // Move along Z Axis wrt North.
        MOVE_Z = 3,
        // Attempt creating formations with surrounding nodes.
        ANNEAL = 4
    };

} // namespace ketu::communication

#endif //TYPES_H
