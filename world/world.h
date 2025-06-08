#ifndef WORLD_H
#define WORLD_H

namespace ketu::world
{

    // Renders the nodes in the works while abstracting away any
    // OpenSceneGraph functionality.
    class World
    {
    public:
        // Render all the nodes in the world.
        void render();
    };

} // ketu::world

#endif //WORLD_H
