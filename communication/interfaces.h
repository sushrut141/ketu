#ifndef INTEFACES_H
#define INTEFACES_H

#include <iostream>


namespace ketu::communication
{

    // Types of messages that can be sent.
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
