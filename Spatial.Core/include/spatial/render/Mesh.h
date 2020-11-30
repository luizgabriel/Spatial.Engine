#pragma once

#include <filament/Box.h>
#include <filament/RenderableManager.h>
#include <spatial/render/Resources.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace spatial
{

struct FilameshFileHeader
{
	uint32_t version{};
	uint32_t parts{};
	filament::Box aabb{};
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
	filament::Box boundingBox{};
	std::string materialName{};
};

struct Mesh
{
  private:
	filament::Engine& mEngine;
	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;

	filament::Box mBoundingBox;
	std::vector<MeshPart> mParts;

  public:
	explicit Mesh(filament::Engine& engine);

	Mesh(filament::Engine& engine, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, filament::Box boundingBox,
		 size_t numParts);

	Mesh(Mesh&& mesh) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	Mesh(const Mesh& mesh) = delete;
	Mesh& operator=(const Mesh& other) noexcept = delete;

	[[nodiscard]] size_t size() const
	{
		return mParts.size();
	}

	MeshPart& operator[](size_t index)
	{
		return mParts[index];
	}

	const MeshPart& operator[](size_t index) const
	{
		return mParts[index];
	}

	[[nodiscard]] auto begin() const noexcept
	{
		return mParts.begin();
	}

	[[nodiscard]] auto end() const noexcept
	{
		return mParts.end();
	}

	[[nodiscard]] auto boundingBox() const
	{
		return mBoundingBox;
	}

	[[nodiscard]] auto& getVertexBuffer()
	{
		return mVertexBuffer;
	}

	[[nodiscard]] auto& getIndexBuffer()
	{
		return mIndexBuffer;
	}

	[[nodiscard]] const auto& getVertexBuffer() const
	{
		return mVertexBuffer;
	}

	[[nodiscard]] const auto& getIndexBuffer() const
	{
		return mIndexBuffer;
	}

	[[nodiscard]] auto& getEngine()
	{
		return mEngine;
	}

	[[nodiscard]] const auto& getEngine() const
	{
		return mEngine;
	}
};

using SharedMesh = std::shared_ptr<Mesh>;

} // namespace spatial