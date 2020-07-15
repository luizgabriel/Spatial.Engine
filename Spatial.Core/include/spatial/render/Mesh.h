#pragma once

#include <spatial/render/Resources.h>
#include <filament/Box.h>
#include <filament/RenderableManager.h>

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>

namespace spatial
{

struct FilameshFileHeader
{
	uint32_t version{};
	uint32_t parts{};
	filament::Box aabb;
	uint32_t flags{};
	uint32_t offsetPosition{};
	uint32_t stridePosition{};
	uint32_t offsetTangents{};
	uint32_t strideTangents{};
	uint32_t offsetColor{};
	uint32_t strideColor{};
	uint32_t offsetUV0{};
	uint32_t strideUV0{};
	uint32_t offsetUV1{};
	uint32_t strideUV1{};
	uint32_t vertexCount{};
	uint32_t vertexSize{};
	uint32_t indexType{};
	uint32_t indexCount{};
	uint32_t indexSize{};
};

struct MeshPart
{
	uint32_t offset{};
	uint32_t indexCount{};
	uint32_t minIndex{};
	uint32_t maxIndex{};
	uint32_t materialID{};
	filament::Box boundingBox;

	std::string materialName;
};


struct Mesh
{
private:
	filament::Engine& m_engine;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	filament::Box m_boundingBox;
	std::vector<MeshPart> m_parts;

public:
	Mesh(filament::Engine& engine, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, filament::Box boundingBox, size_t numParts);

	[[nodiscard]] size_t size() const
	{
		return m_parts.size();
	}

	MeshPart& operator[](size_t index)
	{
		return m_parts[index];
	}

	const MeshPart& operator[](size_t index) const
	{
		return m_parts[index];
	}

	[[nodiscard]] auto begin() const noexcept
	{
		return m_parts.begin();
	}

	[[nodiscard]] auto end() const noexcept
	{
		return m_parts.end();
	}

	[[nodiscard]] auto boundingBox() const
	{
		return m_boundingBox;
	}

	[[nodiscard]] auto& getVertexBuffer()
	{
		return m_vertexBuffer;
	}

	[[nodiscard]] auto& getIndexBuffer()
	{
		return m_indexBuffer;
	}

	[[nodiscard]] auto& getVertexBuffer() const
	{
		return m_vertexBuffer;
	}

	[[nodiscard]] auto& getIndexBuffer() const
	{
		return m_indexBuffer;
	}

	[[nodiscard]] auto& getEngine() const
	{
		return m_engine;
	}

	void build(utils::Entity entity, filament::MaterialInstance* defaultInstance);

	using MaterialsMap = std::unordered_map<std::string, filament::MaterialInstance*>;
	void build(utils::Entity entity, const MaterialsMap& map);
};


} // namespace spatial