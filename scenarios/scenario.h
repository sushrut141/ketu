#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "../world/world.h"

#include <memory>
#include <unordered_map>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

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

    private:

        void addNodeToRenderedScene(const std::string& node_id, const ketu::telemetry::Position& position);

        // OSG specific member variables
        osg::ref_ptr<osgViewer::Viewer> osgViewer_;
        osg::ref_ptr<osg::Group> osgSceneRoot_;
        osg::ref_ptr<osg::Geode> osgRootGeode_;

        std::unordered_map<std::string, osg::ref_ptr<osg::ShapeDrawable>> node_osg_drawable_map_;
    };


} // namespace ketu::scenarios

#endif //SCENARIOS_H
