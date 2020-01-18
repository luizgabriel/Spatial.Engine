#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/render/EntityResource.h>
#include <filameshio/MeshReader.h>

namespace spatial::render
{

class MeshResource
{
private:
    EntityResource m_entity;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;

public:
    MeshResource(filament::Engine *m_engine)
        : m_entity{m_engine},
          m_vertexBuffer{m_engine},
          m_indexBuffer{m_engine}
    {
    }

    MeshResource(filament::Engine *m_engine, const filamesh::MeshReader::Mesh &mesh)
        : m_entity{m_engine, mesh.renderable},
          m_vertexBuffer{m_engine, mesh.vertexBuffer},
          m_indexBuffer{m_engine, mesh.indexBuffer}
    {
    }

    MeshResource(MeshResource &&other) noexcept
        : m_entity{std::move(other.m_entity)},
          m_vertexBuffer{std::move(other.m_vertexBuffer)},
          m_indexBuffer{std::move(other.m_indexBuffer)}
    {
    }

    MeshResource &operator=(MeshResource &&other) noexcept
    {
        m_entity = std::move(other.m_entity);
        m_vertexBuffer = std::move(other.m_vertexBuffer);
        m_indexBuffer = std::move(other.m_indexBuffer);

        return *this;
    }

    MeshResource(MeshResource &other) = delete;
    MeshResource &operator=(const MeshResource &w) = delete;

    //region Getters
    utils::Entity get()
    {
        return getEntity();
    }

    utils::Entity getEntity()
    {
        return m_entity.get();
    }

    filament::VertexBuffer *getVertexBuffer()
    {
        return m_vertexBuffer.get();
    }

    filament::IndexBuffer *getIndexBuffer()
    {
        return m_indexBuffer.get();
    }
    //endregion
};

} // namespace spatial::render