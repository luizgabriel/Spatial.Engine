#pragma once

#include <spatial/render/Mesh.h>

namespace fl = filament;

namespace spatial
{

Mesh::Mesh(fl::Engine& engine, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, fl::Box boundingBox, size_t numParts)
	: m_engine{engine},
	  m_vertexBuffer{std::move(vertexBuffer)},
	  m_indexBuffer{std::move(indexBuffer)},
	  m_boundingBox{boundingBox},
	  m_parts(numParts)
{
}

void Mesh::build(utils::Entity entity, filament::MaterialInstance* defaultInstance)
{
	auto map = MaterialsMap{{"default", defaultInstance}};
	return build(entity, map);
}

void Mesh::build(utils::Entity entity, const MaterialsMap& map)
{
	auto builder = fl::RenderableManager::Builder(size());
	builder.boundingBox(boundingBox());

	size_t index{};
	for (auto& part : m_parts) {
		builder.geometry(index, fl::RenderableManager::PrimitiveType::TRIANGLES,
						 getVertexBuffer().get(), getIndexBuffer().get(), part.offset,
						 part.minIndex, part.maxIndex, part.indexCount);

		if (map.contains(part.materialName))
			builder.material(index, map.at(part.materialName));
		else
			builder.material(index, map.at("default"));

		index++;
	}

	builder.build(getEngine(), entity);
}

}