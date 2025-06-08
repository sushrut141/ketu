#ifndef SCENARIOS_H
#define SCENARIOS_H

namespace ketu::scenarios
{
    class Scenario {

    public:
        // Carry out any initializations like node creation, sensor client creation etc
        // required to run this scenario.
        virtual void setup() = 0;

        // Compute the next state of the world on each simulation cycle.
        virtual void onTick() = 0;

        // Callback invoked when a nodes state is updated.
        virtual void onNodeUpdated() = 0;

        // Run the scenario.
        virtual void run() = 0;

        // Cleanup hook to clear any allocated resources.
        virtual void cleanup() = 0;
    };


} // namespace ketu::scenarios

#endif //SCENARIOS_H
