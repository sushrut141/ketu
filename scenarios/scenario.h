#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "../world/world.h"

#include <memory>
#include <unordered_map>

namespace ketu::scenarios
{
    class Scenario {

    public:
        // Carry out any initializations like node creation, sensor client creation etc
        // required to run this scenario.
        virtual void setup() = 0;

        // Callback invoked when a nodes state is updated.
        virtual void onNodeUpdated() = 0;

        // Run the scenario.
        void run();

    protected:
        // Base class constructor must be invoked by derived scenarios.
        Scenario(std::unique_ptr<ketu::world::World> world);

        // Invoked before each rendering frame to reflect the changed world state.
        // Changes to the world are automatically reflected in the viewer.
        // Base class method must be invoked before derived implementation.
        virtual void onTick();

        std::unique_ptr<ketu::world::World> world_;
    };


} // namespace ketu::scenarios

#endif //SCENARIOS_H
