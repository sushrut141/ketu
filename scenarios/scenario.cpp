#include "scenario.h"
#include <osg/Geometry>

#include "simulation_viewer.h"

namespace ketu::scenarios
{

    Scenario::Scenario(std::unique_ptr<ketu::world::World> world)
    {
        world_ = std::move(world);
        // Setup ODG rendering variables.
        std::function<void()> tick_callback = std::bind(&Scenario::onTick, this);
        osgViewer_ = new SimulationViewer(tick_callback);
        osgSceneRoot_ = new osg::Group();
        osgRootGeode_ = new osg::Geode();
        osgRootGeode_->setName("RootGeode");
        osgSceneRoot_->addChild(osgRootGeode_);
    }

    void Scenario::run()
    {
        osgViewer_->run();
    }

    void Scenario::onTick()
    {
        auto nodePositions = this->world_->getNodePositions();
        for (const auto& node_id_position_pair : nodePositions)
        {
            bool nodeNotInScene = this->node_osg_drawable_map_.find(node_id_position_pair.first) == this->node_osg_drawable_map_.end();
            if (nodeNotInScene)
            {
                addNodeToRenderedScene(node_id_position_pair.first, node_id_position_pair.second);
                osgViewer_->setSceneData(osgRootGeode_.get());
            }
        }
    }

    void Scenario::addNodeToRenderedScene(
            const std::string& node_id,
            const ketu::telemetry::Position& position)
    {
        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(
        osg::Vec3(position.getX(), position.getY(), position.getZ()), 0.1f);
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere.get());

        sphereDrawable->setName(node_id);
        sphereDrawable->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

        this->osgRootGeode_->addDrawable(sphereDrawable.get());
        this->node_osg_drawable_map_.insert(std::make_pair(node_id, sphereDrawable));
    }



} // namespace ketu::scenarios