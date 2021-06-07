#pragma once

#include <filament/Box.h>
#include <filament/RenderableManager.h>
#include <spatial/render/Resources.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace spatial::render
{

struct FilameshFileHeader
{
	uint32_t version{};
	uint32_t partsCount{};
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

struct FilameshFilePart
{
	uint32_t offset{};
	uint32_t count{};
	uint32_t minIndex{};
	uint32_t maxIndex{};
	uint32_t materialID{};
	filament::Box boundingBox{};
	std::string materialName{};
};

class FilameshFile
{
  private:
	FilameshFileHeader mHeader;
	SharedVertexBuffer mVertexBuffer;
	SharedIndexBuffer mIndexBuffer;
	std::vector<FilameshFilePart> mParts;

  public:
	FilameshFile(FilameshFileHeader header) : mHeader{std::move(header)}, mVertexBuffer{}, mIndexBuffer{}, mParts(header.partsCount)
	{
	}

	auto& getParts() noexcept
	{
		return mParts;
	}

	const auto& getParts() const noexcept
	{
		return mParts;
	}

	const auto& getHeader() const noexcept
	{
		return mHeader;
	}

	auto getPartsCount() const noexcept
	{
		return mParts.size();
	}

	const auto& getBoundingBox() const noexcept
	{
		return getHeader().aabb;
	}

	const auto& getVertexBuffer() const
	{
		return mVertexBuffer;
	}

	const auto& getIndexBuffer() const
	{
		return mIndexBuffer;
	}

	void setVertexBuffer(const SharedVertexBuffer& vertexBuffer) noexcept
	{
		mVertexBuffer = vertexBuffer;
	}

	void setIndexBuffer(const SharedIndexBuffer& indexBuffer) noexcept
	{
		mIndexBuffer = indexBuffer;
	}

};

using SharedFilameshFile = std::shared_ptr<FilameshFile>;

} // namespace spatial::render