#pragma once

#include <spatial/common/Math.h>
#include <spatial/resources/BoundingBox.h>

#include <cstdint>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

namespace spatial
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

struct FilameshFile
{
	FilameshFileHeader header{};
	std::vector<FilameshFilePart> parts{};
	std::vector<uint8_t> vertexData{};
	std::vector<uint8_t> indexData{};
};

std::istream& operator>>(std::istream& stream, spatial::FilameshFilePart& part);

std::istream& operator>>(std::istream& stream, spatial::FilameshFileHeader& header);

std::istream& operator>>(std::istream& stream, spatial::FilameshFile& filamesh);

FilameshFile loadFilameshFromMemory(const uint8_t* data, std::size_t size);

} // namespace spatial
