#include <spatial/resources/FilameshFile.h>
#include <spatial/resources/Math.h>
#include <sstream>
#include <string.h>

using namespace std;

namespace spatial
{

istream& operator>>(istream& stream, spatial::FilameshFilePart& part)
{
	stream.read(reinterpret_cast<char*>(&part.offset), 4);
	stream.read(reinterpret_cast<char*>(&part.count), 4);
	stream.read(reinterpret_cast<char*>(&part.minIndex), 4);
	stream.read(reinterpret_cast<char*>(&part.maxIndex), 4);
	stream.read(reinterpret_cast<char*>(&part.materialID), 4);

	stream >> part.boundingBox;

	return stream;
}

istream& operator>>(istream& stream, spatial::FilameshFileHeader& header)
{
	auto magic = std::array<char, 9>{};
	stream.read(magic.data(), 8);
	magic[8] = '\0';

	if (strcmp(magic.data(), "FILAMESH") != 0)
	{
		stream.setstate(ios_base::failbit);
		return stream;
	}

	stream.read(reinterpret_cast<char*>(&header.version), 4);
	stream.read(reinterpret_cast<char*>(&header.partsCount), 4);

	stream >> header.aabb;

	stream.read(reinterpret_cast<char*>(&header.flags), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetPosition), 4);
	stream.read(reinterpret_cast<char*>(&header.stridePosition), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetTangents), 4);
	stream.read(reinterpret_cast<char*>(&header.strideTangents), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetColor), 4);
	stream.read(reinterpret_cast<char*>(&header.strideColor), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetUV0), 4);
	stream.read(reinterpret_cast<char*>(&header.strideUV0), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetUV1), 4);
	stream.read(reinterpret_cast<char*>(&header.strideUV1), 4);
	stream.read(reinterpret_cast<char*>(&header.vertexCount), 4);
	stream.read(reinterpret_cast<char*>(&header.vertexSize), 4);
	stream.read(reinterpret_cast<char*>(&header.indexType), 4);
	stream.read(reinterpret_cast<char*>(&header.indexCount), 4);
	stream.read(reinterpret_cast<char*>(&header.indexSize), 4);

	return stream;
}

istream& operator>>(istream& stream, spatial::FilameshFile& filamesh)
{
	stream >> filamesh.header;

	filamesh.vertexData.resize(filamesh.header.vertexSize);
	stream.read(reinterpret_cast<char*>(&filamesh.vertexData[0]), filamesh.header.vertexSize);

	filamesh.indexData.resize(filamesh.header.indexSize);
	stream.read(reinterpret_cast<char*>(&filamesh.indexData[0]), filamesh.header.indexSize);

	filamesh.parts.resize(filamesh.header.partsCount);
	for (size_t i = 0; i < filamesh.header.partsCount; i++)
		stream >> filamesh.parts.at(i);

	uint32_t materialCount;
	stream.read(reinterpret_cast<char*>(&materialCount), 4);

	for (size_t i = 0; i < filamesh.header.partsCount; i++)
	{
		uint32_t nameLength;
		stream.read(reinterpret_cast<char*>(&nameLength), 4);

		std::getline(stream, filamesh.parts.at(i).materialName, '\0');
	}

	return stream;
}

} // namespace spatial
