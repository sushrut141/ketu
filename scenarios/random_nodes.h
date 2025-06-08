#ifndef RANDOM_NODES_H
#define RANDOM_NODES_H
#include <memory>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

#include "scenario.h"
#include "../objects/node.h"
#include "../world/world.h"

namespace ketu::scenarios
{
    class RandomNodes: Scenario
    {
    public:
        static std::unique_ptr<RandomNodes> create();

        void setup();

        void onTick();

        void onNodeUpdated();

        void run();

        void cleanup();
    private:
        RandomNodes(std::unique_ptr<ketu::world::World> world);

        void addNodeToRenderedScene(const std::string& node_id, const ketu::telemetry::Position& position);

        std::unique_ptr<ketu::world::World> world_;
        std::unique_ptr<ketu::sensing::SensingClient> sensing_client_;
        std::unique_ptr<ketu::communication::CommunicationClient> communication_client_;

        std::vector<ketu::objects::Node> nodes_;

        // OSG specific member variables
        osg::ref_ptr<osgViewer::Viewer> osgViewer_;
        osg::ref_ptr<osg::Group> osgSceneRoot_;
        osg::ref_ptr<osg::Geode> osgRootGeode_;

        std::unordered_map<std::string, osg::ref_ptr<osg::ShapeDrawable>> node_osg_drawable_map_;
    };

} // namespace ketu::scenarios
#endif //RANDOM_NODES_H
