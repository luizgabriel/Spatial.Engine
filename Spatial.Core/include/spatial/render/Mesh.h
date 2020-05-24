#pragma once

#include <spatial/render/Resources.h>
#include <spatial/render/Entity.h>
#include <filameshio/MeshReader.h>

namespace spatial
{

class Mesh
{
private:
	Entity m_entity;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

public:
	Mesh(filament::Engine* engine, const filamesh::MeshReader::Mesh& mesh)
		: m_entity{engine, mesh.renderable},
		  m_vertexBuffer{createResource(engine, mesh.vertexBuffer)},
		  m_indexBuffer{createResource(engine, mesh.indexBuffer)}
	{
	}

	Mesh(Mesh& other) = delete;
	Mesh& operator=(const Mesh& w) = delete;

	// region Getters
	utils::Entity get()
	{
		return getEntity();
	}

	utils::Entity getEntity()
	{
		return m_entity.get();
	}

	filament::VertexBuffer* getVertexBuffer()
	{
		return m_vertexBuffer.get();
	}

	filament::IndexBuffer* getIndexBuffer()
	{
		return m_indexBuffer.get();
	}
	// endregion
};

} // namespace spatial