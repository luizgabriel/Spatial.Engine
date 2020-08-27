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

void Mesh::build(utils::Entity entity, filament::MaterialInstance* defaultInstance)
{
	auto map = MaterialsMap{{"DefaultMaterial", defaultInstance}};
	return build(entity, map);
}

void Mesh::build(utils::Entity entity, const MaterialsMap& map)
{
	auto builder = fl::RenderableManager::Builder(size());
	builder.boundingBox(boundingBox());

	size_t index{};
	for (auto& part : mParts)
	{
		builder.geometry(index, fl::RenderableManager::PrimitiveType::TRIANGLES, getVertexBuffer().get(),
						 getIndexBuffer().get(), part.offset, part.minIndex, part.maxIndex, part.indexCount);

		if (map.find(part.materialName) != map.end())
			builder.material(index, map.at(part.materialName));
		else
			builder.material(index, map.at("DefaultMaterial"));

		index++;
	}

	builder.build(getEngine(), entity);
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