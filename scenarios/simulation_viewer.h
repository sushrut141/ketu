#ifndef SIMULATION_VIEWER_H
#define SIMULATION_VIEWER_H

#include <osgViewer/Viewer>
#include <functional>

namespace ketu::scenarios
{
    class SimulationViewer : public osgViewer::Viewer {

    public:
        SimulationViewer(std::function<void()> tick_callback) : tick_callback_(tick_callback) {}

        virtual void frame(double simulationTime) override {
            tick_callback_();
            osgViewer::Viewer::frame();
        }

    private:
        std::function<void()> tick_callback_;

    };

}  // namespace ketu::scenarios
#endif //SIMULATION_VIEWER_H
