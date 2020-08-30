#include <spatial/render/Mesh.h>

namespace fl = filament;

namespace spatial
{

Mesh::Mesh(fl::Engine& engine, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, fl::Box boundingBox,
		   size_t numParts)
	: mEngine{engine},
	  mVertexBuffer{std::move(vertexBuffer)},
	  mIndexBuffer{std::move(indexBuffer)},
	  mBoundingBox{boundingBox},
	  mParts(numParts)
{
}

Mesh::Mesh(filament::Engine& engine)
	 : mEngine{engine}, mVertexBuffer{engine}, mIndexBuffer{engine}, mBoundingBox{}, mParts(0)
{
}

Mesh::Mesh(Mesh&& other) noexcept
	 : mEngine{other.mEngine},
	   mVertexBuffer{std::move(other.mVertexBuffer)},
	   mIndexBuffer{std::move(other.mIndexBuffer)},
	   mBoundingBox{std::move(other.mBoundingBox)},
	   mParts(std::move(other.mParts))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	mVertexBuffer = std::move(other.mVertexBuffer);
	mIndexBuffer = std::move(other.mIndexBuffer);
	mBoundingBox = std::move(other.mBoundingBox);
	mParts = std::move(other.mParts);

	return *this;
}

} // namespace spatial