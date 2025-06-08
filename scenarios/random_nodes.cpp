#include "random_nodes.h"

#include <iostream>
#include <functional>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Geometry>

#include "simulation_viewer.h"
#include "../objects/node.h"

namespace ketu::scenarios
{
    std::unique_ptr<RandomNodes> RandomNodes::create()
    {
        auto world = std::make_unique<ketu::world::World>();
        return std::unique_ptr<RandomNodes>(new RandomNodes(std::move(world)));
    }

    RandomNodes::RandomNodes(std::unique_ptr<ketu::world::World> world)
    {
        this->world_ = std::move(world);
        this->sensing_client_ = std::make_unique<ketu::sensing::SensingClient>(this->world_.get());
        this->communication_client_ = std::make_unique<ketu::communication::CommunicationClient>(this->world_.get());

        // Setup ODG rendering variables.
        std::function<void()> tick_callback = std::bind(&RandomNodes::onTick, this);
        osgViewer_ = new SimulationViewer(tick_callback);
        osgSceneRoot_ = new osg::Group();
        osgRootGeode_ = new osg::Geode();
        osgRootGeode_->setName("RootGeode");
        osgSceneRoot_->addChild(osgRootGeode_);
    }

    void RandomNodes::setup()
    {
        ketu::objects::Node node("sphere_1", sensing_client_.get(), communication_client_.get());
        ketu::telemetry::Position position(1.0, 1.0, 100.5);

        nodes_.push_back(node);
        world_->addNode(node.getId(), position);
        addNodeToRenderedScene(node.getId(), position);
    }

    void RandomNodes::onNodeUpdated()
    {

    }

    void RandomNodes::onTick()
    {
        std::cout << "RandomNodes::onTick()" << std::endl;
    }

    void RandomNodes::run()
    {
        osgViewer_->setSceneData(osgRootGeode_.get());
        osgViewer_->run();
    }

    void RandomNodes::cleanup()
    {

    }

    void RandomNodes::addNodeToRenderedScene(
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
