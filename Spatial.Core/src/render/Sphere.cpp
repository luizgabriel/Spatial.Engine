#include <spatial/render/Sphere.h>
#include <spatial/render/IcoSphere.h>

#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/TransformManager.h>
#include <filament/VertexBuffer.h>

#include <math/norm.h>

using namespace filament;
using namespace filament::math;
using namespace utils;

namespace spatial::render
{

struct Geometry
{
    IcoSphere sphere = IcoSphere{2};
    std::vector<filament::math::short4> tangents;
    filament::VertexBuffer *vertexBuffer = nullptr;
    filament::IndexBuffer *indexBuffer = nullptr;
};

// note: this will be leaked since we don't have a good time to free it.
// this should be a cache indexed on the sphere's subdivisions
static Geometry *gGeometry{nullptr};

Sphere::Sphere(RenderEngine &engine, Material &material, bool culling)
    : m_engine(&engine),
      m_materialInstance{engine, material->createInstance()},
      m_renderable{}
{
    static_assert(sizeof(IcoSphere::Triangle) == sizeof(IcoSphere::Index) * 3,
                  "indices are not packed");

    if (gGeometry == nullptr)
    {
        gGeometry = new Geometry;

        auto const &indices = gGeometry->sphere.getIndices();
        auto const &vertices = gGeometry->sphere.getVertices();
        uint32_t indexCount = (uint32_t)(indices.size() * 3);

        gGeometry->tangents.resize(vertices.size());
        VertexBuffer::populateTangentQuaternions(VertexBuffer::QuatTangentContext{
            VertexBuffer::SHORT4,
            vertices.size(),
            gGeometry->tangents.data(),
            sizeof(filament::math::short4),
            vertices.data()});

        // todo produce correct u,v

        gGeometry->vertexBuffer = VertexBuffer::Builder()
                                      .vertexCount((uint32_t)vertices.size())
                                      .bufferCount(2)
                                      .attribute(VertexAttribute::POSITION, 0, VertexBuffer::AttributeType::FLOAT3)
                                      .attribute(VertexAttribute::TANGENTS, 1, VertexBuffer::AttributeType::SHORT4)
                                      .normalized(VertexAttribute::TANGENTS)
                                      .build(m_engine->get_ref());

        gGeometry->vertexBuffer->setBufferAt(m_engine->get_ref(), 0,
                                             VertexBuffer::BufferDescriptor(vertices.data(), vertices.size() * sizeof(float3)));

        gGeometry->vertexBuffer->setBufferAt(m_engine->get_ref(), 1,
                                             VertexBuffer::BufferDescriptor(gGeometry->tangents.data(), gGeometry->tangents.size() * sizeof(filament::math::short4)));

        gGeometry->indexBuffer = IndexBuffer::Builder()
                                     .bufferType(IndexBuffer::IndexType::USHORT)
                                     .indexCount(indexCount)
                                     .build(m_engine->get_ref());

        gGeometry->indexBuffer->setBuffer(m_engine->get_ref(),
                                          IndexBuffer::BufferDescriptor(indices.data(), indexCount * sizeof(IcoSphere::Index)));
    }

    RenderableManager::Builder(1)
        .boundingBox({{0}, {1}})
        .material(0, m_materialInstance.get())
        .geometry(0, RenderableManager::PrimitiveType::TRIANGLES, gGeometry->vertexBuffer, gGeometry->indexBuffer)
        .culling(culling)
        .build(m_engine->get_ref(), m_renderable.get());
}

Sphere &Sphere::setPosition(filament::math::float3 const &position) noexcept
{
    auto &tcm = m_engine->get()->getTransformManager();
    auto ci = tcm.getInstance(m_renderable.get());
    mat4f model = tcm.getTransform(ci);
    model[3].xyz = position;
    tcm.setTransform(ci, model);

    return *this;
}

Sphere &Sphere::setRadius(float radius) noexcept
{
    auto &tcm = m_engine->get()->getTransformManager();
    auto ci = tcm.getInstance(m_renderable.get());
    mat4f model = tcm.getTransform(ci);
    model[0].x = radius;
    model[1].y = radius;
    model[2].z = radius;
    tcm.setTransform(ci, model);

    return *this;
}

} // namespace spatial::render::shapes