#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

#include "../objects/node.h"

namespace ketu::world
{

    // Renders the nodes in the works while abstracting away any
    // OpenSceneGraph functionality.
    class World
    {
    public:
        World();

        // Adds a node to the scene.
        void addNode(const std::shared_ptr<ketu::objects::Node> node);

        // Render all the nodes in the world.
        void render();
    private:
        std::vector<std::shared_ptr<ketu::objects::Node>> nodes_;
        std::unordered_map<std::string, osg::ref_ptr<osg::ShapeDrawable>> node_osg_drawable_map_;

        // OSG specific member variables
        osg::ref_ptr<osgViewer::Viewer> osgViewer_;
        osg::ref_ptr<osg::Group> osgSceneRoot_;
        osg::ref_ptr<osg::Geode> osgRootGeode_;
    };

} // ketu::world

#endif //WORLD_H
