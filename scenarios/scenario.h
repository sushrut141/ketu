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

        // Run the scenario.
        void run();

    protected:
        // Base class constructor must be invoked by derived scenarios.
        Scenario(std::unique_ptr<ketu::world::World> world);

        // Invoked before each rendering frame to reflect the changed world state.
        // Changes to the world are automatically reflected in the viewer.
        virtual void onTick(unsigned long long frameNumber) = 0;

        std::unique_ptr<ketu::world::World> world_;

    private:
        unsigned long long frameNumber_;
    };


} // namespace ketu::scenarios

#endif //SCENARIOS_H
