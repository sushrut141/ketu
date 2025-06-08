#include "world.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform> // For simpler positioning
#include <iostream>
#include <vector>
#include <string>


namespace ketu::world {

World::World()
{
    osgViewer_ = new osgViewer::Viewer();
    osgSceneRoot_ = new osg::Group();
    osgRootGeode_ = new osg::Geode();
    osgRootGeode_->setName("RootGeode");
}


void World::addNode(const std::shared_ptr<ketu::objects::Node> node)
{
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(
        osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f); // Center at origin, radius 1.0
    osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere.get());

    sphereDrawable->setName(node->getId());
    sphereDrawable->setColor(osg::Vec4(0.0f, 0.5f, 1.0f, 1.0f));

    osgRootGeode_->addDrawable(sphereDrawable.get());

    this->nodes_.push_back(node);
    this->node_osg_drawable_map_.insert(std::make_pair(node->getId(), sphereDrawable));
}


void World::render() {
    osgViewer_->setSceneData(osgRootGeode_.get());
    osgViewer_->run();
}

} // namespace ketu::world
