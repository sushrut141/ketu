#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <memory>
#include <map>

#include "telemetry/position.h"

namespace ketu::thirdparty::tinyobj
{

    class Loader
    {
    public:
        // Creates an instance of the loader from an obj file.
        static std::unique_ptr<Loader> fromOBJFile(const std::string& path, double scalingFactor);

        // Gets the positions of the vertices for the mesh.
        const std::vector<ketu::telemetry::Position>& getVertices() const;

        // Gets the connectivity graph as a map of vertex indexes to the vertex indexes it is
        // connected to.
        const std::map<int, std::vector<int>>& getConnectivity() const;


    private:

        std::vector<ketu::telemetry::Position> vertices_;
        std::map<int, std::vector<int>> connectivity_;
        double scalingFactor_;
    };

} // ketu::thirdparty::tinyobj

#endif //LOADER_H
