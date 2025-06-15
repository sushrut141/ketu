#ifndef INTERFACE_H
#define INTERFACE_H

#include "../communication/communication_client.h"

namespace ketu::formation
{

    enum Formation
    {
        UNSPECIFIED = 0,
        // Default grid formation.
        GRID = 1,
    };
} // namespace ketu::formation

#endif // INTERFACE_H
