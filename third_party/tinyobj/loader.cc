#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <set>

#include "loader.h"
#include "tinyobjloader.h"


namespace ketu::thirdparty::tinyobj
{

    namespace
    {
        bool LoadObjMeshWithTinyObjLoader(
            const std::string& filePath,
            int scalingFactor,
            std::vector<ketu::telemetry::Position>* vertices,
            std::map<int, std::vector<int>>* adjacency_list
        )
        {
            ::tinyobj::ObjReaderConfig reader_config;
            // We want faces to build the adjacency list, so triangulation is helpful for consistency
            reader_config.triangulate = true; // Ensure faces are triangulated for simpler adjacency processing
            reader_config.mtl_search_path = "./"; // Or specify a path to your .mtl file if you had one

            ::tinyobj::ObjReader reader;

            if (!reader.ParseFromFile(filePath, reader_config))
            {
                if (!reader.Error().empty())
                {
                    std::cerr << "TinyObjLoader Error: " << reader.Error() << std::endl;
                }
                throw std::runtime_error("::tinyobj::LoadObj failed to load file");
            }

            if (!reader.Warning().empty())
            {
                std::cout << "TinyObjLoader Warning: " << reader.Warning() << std::endl;
            }

            auto& attrib = reader.GetAttrib();
            auto& shapes = reader.GetShapes();

            // 1. Populate vertices
            for (size_t v = 0; v < attrib.vertices.size() / 3; ++v)
            {
                vertices->push_back({
                    scalingFactor * attrib.vertices[3 * v + 0], // X
                    scalingFactor * attrib.vertices[3 * v + 1], // Y
                    scalingFactor * attrib.vertices[3 * v + 2] // Z
                });
            }

            // 2. Populate adjacency list using faces
            // Use std::set to avoid duplicate entries in adjacency list before converting to vector
            std::map<int, std::set<int>> temp_adjacency_sets;

            for (const auto& shape : shapes)
            {
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
                {
                    size_t fv =
                        shape.mesh
                            .num_face_vertices[f]; // Number of vertices in this face (should be 3 if triangulate=true)

                    // Get the vertex indices for the current face
                    std::vector<int> face_vertex_indices;
                    for (size_t v_in_face = 0; v_in_face < fv; ++v_in_face)
                    {
                        ::tinyobj::index_t idx = shape.mesh.indices[index_offset + v_in_face];
                        face_vertex_indices.push_back(idx.vertex_index); // Store 0-based vertex index
                    }

                    // Build adjacencies for this face
                    // For a triangle (v0, v1, v2), connections are (v0-v1), (v1-v2), (v2-v0)
                    for (size_t i = 0; i < fv; ++i)
                    {
                        int v_curr = face_vertex_indices[i];
                        int v_next = face_vertex_indices[(i + 1) % fv]; // Wrap around for the last edge

                        // Add bidirectional connections
                        temp_adjacency_sets[v_curr].insert(v_next);
                        temp_adjacency_sets[v_next].insert(v_curr);
                    }
                    index_offset += fv;
                }
            }

            // Convert sets to vectors for the final adjacency list
            for (const auto& pair : temp_adjacency_sets)
            {
                (*adjacency_list)[pair.first].assign(pair.second.begin(), pair.second.end());
            }
            for (int i = 0; i < vertices->size(); ++i)
            {
                if (adjacency_list->find(i) == adjacency_list->end())
                {
                    adjacency_list->insert({i, {}});
                }
            }
            std::cout << "OBJ: Loaded " << vertices->size() << " vertices and "
                      << adjacency_list->size() << " unique vertex adjacencies from " << filePath << std::endl;
            return true;
        }

    } // namespace

    std::unique_ptr<Loader> Loader::fromOBJFile(const std::string& path, int scalingFactor)
    {
        auto loader = std::make_unique<Loader>();
        loader->scalingFactor_ = scalingFactor;
        LoadObjMeshWithTinyObjLoader(path, loader->scalingFactor_, &loader->vertices_, &loader->connectivity_);
        return std::move(loader);
    }

    const std::vector<ketu::telemetry::Position>& Loader::getVertices() const
    {
        return vertices_;
    }

    const std::map<int, std::vector<int>>& Loader::getConnectivity() const
    {
        return connectivity_;
    }

} // namespace ketu::thirdparty::tinyobj
