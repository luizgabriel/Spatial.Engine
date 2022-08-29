#include "spatial/resources/MeshComponents.h"
#include <filament/Fence.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/graphics/MeshController.h>
#include <spatial/graphics/MeshResources.h>
#include <spatial/graphics/Renderable.h>
#include <spatial/graphics/Resources.h>

namespace spatial::graphics
{

void MeshController::updateMeshInstances(ecs::Registry& registry)
{
	registry
		.getEntities<const ecs::MeshInstance, Renderable>() //
		.each([&](const ecs::MeshInstance& meshInstance, Renderable& renderable) {
			renderable.setCastShadows(meshInstance.castShadows);
			renderable.setReceiveShadows(meshInstance.receiveShadows);
			renderable.setCulling(meshInstance.culling);
			renderable.setPriority(meshInstance.priority);
		});

	registry
		.getEntities<const ecs::MeshInstance, Renderable>() //
		.each([&](const ecs::MeshInstance& meshInstance, Renderable& renderable) {
			if (!registry.hasComponent<ecs::Mesh>(meshInstance.meshSource))
				return;

			const auto& mesh = registry.getComponent<const ecs::Mesh>(meshInstance.meshSource);
			renderable.setAxisAlignedBoundingBox(mesh.boundingBox);

			if (registry.hasComponent<SharedMaterialInstance>(meshInstance.defaultMaterialInstance))
			{
				const auto& materialInstance =
					registry.getComponent<const SharedMaterialInstance>(meshInstance.defaultMaterialInstance);

				for (size_t i = 0; i < meshInstance.partsCount; i++)
					renderable.setMaterialInstanceAt(i, materialInstance);
			}

			if (registry.hasAllComponents<SharedVertexBuffer, SharedIndexBuffer>(meshInstance.meshSource))
			{
				const auto& vertexBuffer = registry.getComponent<SharedVertexBuffer>(meshInstance.meshSource);
				const auto& indexBuffer = registry.getComponent<SharedIndexBuffer>(meshInstance.meshSource);
				auto parts = ecs::Mesh::getParts(registry, meshInstance.meshSource);
				if (parts.size() == 0)
				{
					renderable.setGeometryAt(0, Renderable::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer, 0,
											 indexBuffer->getIndexCount());
					return;
				}

				auto partsCount =
					meshInstance.partsCount > 0 ? std::min(parts.size(), meshInstance.partsCount) : parts.size();

				for (auto primitiveIndex = meshInstance.partsOffset; primitiveIndex < partsCount; primitiveIndex++)
				{
					const auto& part = registry.getComponent<ecs::MeshPart>(parts.at(primitiveIndex));
					renderable.setGeometryAt(primitiveIndex, Renderable::PrimitiveType::TRIANGLES, vertexBuffer,
											 indexBuffer, part.minIndex, part.indexCount);
				}
			}
		});

	registry
		.getEntities<const ecs::MeshPrimitive, const ecs::Child>() //
		.each([&](const auto& meshPrimitive, const ecs::Child& child) {
			auto* renderable = registry.tryGetComponent<Renderable>(child.parent);
			if (renderable == nullptr)
				return;

			if (meshPrimitive.primitiveIndex >= renderable->getPrimitiveCount())
			{
				registry.addOrReplaceComponent<ecs::tags::ShouldRecreateRenderable>(child.parent);
				return;
			}

			renderable->setBlendOrderAt(meshPrimitive.primitiveIndex, meshPrimitive.blendOrder);

			const auto* materialInstance =
				registry.tryGetComponent<const SharedMaterialInstance>(meshPrimitive.materialInstance);
			if (materialInstance != nullptr)
				renderable->setMaterialInstanceAt(meshPrimitive.primitiveIndex, *materialInstance);
			else
				renderable->resetMaterialInstance(meshPrimitive.primitiveIndex);
		});
}

void MeshController::createMeshInstances(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const Entity, const ecs::MeshInstance>(ecs::ExcludeComponents<Renderable>)
		.each([&](ecs::Entity entity, const auto& graphicsEntity, const ecs::MeshInstance& meshInstance) {
			size_t meshPartsCount =
				std::max(meshInstance.partsCount, ecs::MeshInstance::getPrimitivesCount(registry, entity));
			registry.addComponent<Renderable>(entity, engine, graphicsEntity.get(), meshPartsCount);
		});
}

void MeshController::clearDirtyRenderables(ecs::Registry& registry)
{
	{
		auto view = registry.getEntities<ecs::tags::ShouldRecreateRenderable>();
		registry.removeComponent<Renderable>(view.begin(), view.end());
		registry.removeComponent<ecs::tags::ShouldRecreateRenderable>(view.begin(), view.end());
	}

	{
		auto view = registry.getEntities<ecs::tags::ShouldRecreateMesh, const ecs::Mesh, SharedVertexBuffer>();
		view.each([&](ecs::Entity entity, const auto& mesh, const auto& vertexBuffer) {
			if (mesh.vertexData.getDesiredBufferSize() > vertexBuffer->getVertexCount())
				registry.removeComponent<SharedVertexBuffer>(entity);
			registry.removeComponent<ecs::tags::ShouldRecreateMesh>(entity);
		});
	}

	{
		auto view = registry.getEntities<ecs::tags::ShouldRecreateMesh, const ecs::Mesh, SharedIndexBuffer>();
		view.each([&](ecs::Entity entity, const auto& mesh, const auto& indexBuffer) {
			if (mesh.indexData.getDesiredBufferSize() > indexBuffer->getIndexCount())
				registry.removeComponent<SharedIndexBuffer>(entity);
			registry.removeComponent<ecs::tags::ShouldRecreateMesh>(entity);
		});
	}

	registry.removeComponentFromEntities<ecs::tags::ShouldRecreateMesh>();
}

constexpr filament::VertexAttribute toFilament(ecs::VertexAttribute type)
{
	switch (type)
	{
	case ecs::VertexAttribute::Position:
		return filament::VertexAttribute::POSITION;
	case ecs::VertexAttribute::Color:
		return filament::VertexAttribute::COLOR;
	case ecs::VertexAttribute::UV0:
		return filament::VertexAttribute::UV0;
	case ecs::VertexAttribute::Tangents:
		return filament::VertexAttribute::TANGENTS;
	}
}

constexpr filament::VertexBuffer::AttributeType toFilament(ecs::VertexAttributeType type)
{
	switch (type)
	{
	case ecs::VertexAttributeType::None:
	case ecs::VertexAttributeType::Float3:
		return filament::VertexBuffer::AttributeType::FLOAT3;
	case ecs::VertexAttributeType::Float2:
		return filament::VertexBuffer::AttributeType::FLOAT2;
	case ecs::VertexAttributeType::UnsignedByte3:
		return filament::VertexBuffer::AttributeType::UBYTE3;
	case ecs::VertexAttributeType::UnsignedByte4:
		return filament::VertexBuffer::AttributeType::UBYTE4;
	case ecs::VertexAttributeType::Half2:
		return filament::VertexBuffer::AttributeType::HALF2;
	case ecs::VertexAttributeType::Half4:
		return filament::VertexBuffer::AttributeType::HALF4;
	case ecs::VertexAttributeType::Short2:
		return filament::VertexBuffer::AttributeType::SHORT2;
	case ecs::VertexAttributeType::Short3:
		return filament::VertexBuffer::AttributeType::SHORT3;
	case ecs::VertexAttributeType::Short4:
		return filament::VertexBuffer::AttributeType::SHORT4;
	}
}

constexpr filament::IndexBuffer::IndexType toFilament(ecs::IndexType type)
{
	switch (type)
	{
	case ecs::IndexType::None:
	case ecs::IndexType::UnsignedShort:
		return filament::IndexBuffer::IndexType::USHORT;
	case ecs::IndexType::UnsignedInt:
		return filament::IndexBuffer::IndexType::UINT;
	}
}

filament::VertexBuffer::Builder makeVertexBuffer(const ecs::VertexData& vertexData)
{
	auto builder = filament::VertexBuffer::Builder() //
					   .vertexCount(static_cast<uint32_t>(vertexData.getDesiredBufferSize()))
					   .bufferCount(1);

	const auto structSize = calculateInterleavedBytesStride(vertexData.layout);
	assert(vertexData.data.size() % structSize == 0);

	if (vertexData.layout.mode == ecs::VertexLayoutMode::Interleaved)
	{
		uint32_t lastOffset = 0;
		for (const auto& attr : vertexData.layout.description)
		{
			if (attr.empty())
				continue;

			builder = builder //
						  .attribute(toFilament(attr.attribute), 0, toFilament(attr.type), lastOffset, structSize)
						  .normalized(toFilament(attr.attribute), attr.isNormalized);

			lastOffset += attr.getTypeSizeInBytes();
		}

		assert(lastOffset == structSize);
	}

	if (vertexData.layout.mode == ecs::VertexLayoutMode::Separate)
	{
		const auto vertexCount = vertexData.data.size() / structSize;
		uint32_t lastOffset = 0;
		for (const auto& attr : vertexData.layout.description)
		{
			if (attr.empty())
				continue;

			builder = builder //
						  .attribute(toFilament(attr.attribute), 0, toFilament(attr.type), lastOffset,
									 attr.getTypeSizeInBytes())
						  .normalized(toFilament(attr.attribute), attr.isNormalized);
			lastOffset += vertexCount * attr.getTypeSizeInBytes();
		}

		assert(lastOffset == structSize * vertexCount);
	}

	return builder;
}

filament::IndexBuffer::Builder makeIndexBuffer(const ecs::IndexData& data)
{
	auto flType = toFilament(data.type);
	return filament::IndexBuffer::Builder()
		.indexCount(static_cast<uint32_t>(data.getDesiredBufferSize()))
		.bufferType(flType);
}

template <typename T>
filament::backend::BufferDescriptor makeBufferDescriptor(const std::vector<T>& source)
{
	auto sizeInBytes = source.size() * sizeof(T);
	auto allocator = std::allocator<uint8_t>();
	auto allocatedData = allocator.allocate(sizeInBytes);
	auto sourceBegin = reinterpret_cast<const uint8_t*>(source.data());
	auto sourceEnd = sourceBegin + sizeInBytes;
	std::copy(sourceBegin, sourceEnd, allocatedData);

	auto callback = [](void* buffer, size_t size, void*) {
		auto allocator = std::allocator<T>();
		allocator.deallocate(reinterpret_cast<T*>(buffer), size);
	};

	return {allocatedData, sizeInBytes, callback, nullptr};
}

void MeshController::loadMeshes(filament::Engine& engine, ecs::Registry& registry)
{
	// Create Vertex Buffers
	registry.getEntities<const ecs::Mesh>(ecs::ExcludeComponents<SharedVertexBuffer>)
		.each([&](ecs::Entity entity, const auto& mesh) {
			auto vertexBuffer = toShared(createVertexBuffer(engine, makeVertexBuffer(mesh.vertexData)));
			vertexBuffer->setBufferAt(engine, 0, makeBufferDescriptor(mesh.vertexData.data));
			filament::Fence::waitAndDestroy(engine.createFence());

			registry.addComponent(entity, std::move(vertexBuffer));
		});

	// Create Index Buffers
	registry.getEntities<const ecs::Mesh>(ecs::ExcludeComponents<SharedIndexBuffer>)
		.each([&](ecs::Entity entity, const auto& mesh) {
			auto indexBuffer = toShared(createIndexBuffer(engine, makeIndexBuffer(mesh.indexData)));
			indexBuffer->setBuffer(engine, makeBufferDescriptor(mesh.indexData.data));
			filament::Fence::waitAndDestroy(engine.createFence());

			registry.addComponent(entity, std::move(indexBuffer));
		});

	registry
		.getEntities<const ecs::ResourceData, ecs::tags::IsMesh>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {
			auto filamesh = loadFilameshFromMemory(resource.data.data(), resource.data.size());
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addOrReplaceComponent(
				entity, ecs::Mesh{createVertexData(filamesh), createIndexData(filamesh), filamesh.header.aabb});
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);

			for (const auto& part : filamesh.parts)
				ecs::Mesh::addPart(registry, entity, ecs::MeshPart{part.minIndex, part.count});
		});
}

} // namespace spatial::graphics