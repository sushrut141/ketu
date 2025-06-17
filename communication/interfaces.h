#ifndef INTEFACES_H
#define INTEFACES_H

#include <iostream>


namespace ketu::communication
{

    // Types of messages that can be sent.
    enum MessageType
    {
        UNSPECIFIED = 0,
        // Stop all movement.
        STOP = 1,
        // Move along positive X Axis wrt North.
        MOVE_X_POSITIVE = 2,
        // Move along negative X Axis wrt North.
        MOVE_X_NEGATIVE = 3,
        // Move along positive Y Axis wrt North.
        MOVE_Y_POSITIVE = 4,
        // Move along negative Y Axis wrt North.
        MOVE_Y_NEGATIVE = 5,
        // Move along positive Z Axis wrt North.
        MOVE_Z_POSITIVE = 6,
        // Move along positive Z Axis wrt North.
        MOVE_Z_NEGATIVE = 7,
        // Attempt creating formations with surrounding nodes.
        ANNEAL = 8
    };

    // Interface for all objects that use communication client to implement.
    class Communicable
    {
    public:
        // Method invoked on message received.
        virtual void onMessage(const MessageType& message_type) = 0;

        virtual ~Communicable() { std::cout << "Communicable destroyed." << std::endl; }
    };

} // namespace ketu::communication

#endif // INTEFACES_H
