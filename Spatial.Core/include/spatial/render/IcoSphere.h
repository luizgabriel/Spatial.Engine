#pragma once

#include <math/vec3.h>
#include <map>
#include <vector>

namespace spatial::render
{

class IcoSphere
{
public:
    using Index = uint16_t;

    struct Triangle
    {
        Index vertex[3];
    };

    using TriangleList = std::vector<Triangle>;
    using VertexList = std::vector<filament::math::float3>;
    using IndexedMesh = std::pair<VertexList, TriangleList>;

    explicit IcoSphere(size_t subdivisions);

    IndexedMesh const &getMesh() const { return m_mesh; }
    VertexList const &getVertices() const { return m_mesh.first; }
    TriangleList const &getIndices() const { return m_mesh.second; }

private:
    static const IcoSphere::VertexList s_vertices;
    static const IcoSphere::TriangleList s_triangles;

    using Lookup = std::map<std::pair<Index, Index>, Index>;
    Index vertexForEdge(Lookup &lookup, VertexList &vertices, Index first, Index second);
    TriangleList subdivide(VertexList &vertices, TriangleList const &triangles);
    IndexedMesh makeIcosphere(int subdivisions);

    IndexedMesh m_mesh;
};

} // namespace spatial::render