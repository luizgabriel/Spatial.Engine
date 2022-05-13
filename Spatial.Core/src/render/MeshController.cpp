#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/Resources.h>

namespace spatial::render
{

void MeshController::updateMeshInstances(ecs::Registry& registry)
{
	updateMeshGeometries(registry);
	clearDirtyRenderables(registry);
}

void MeshController::createMeshInstances(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const Entity, const ecs::MeshInstance>(ecs::ExcludeComponents<Renderable>)
		.each([&](ecs::Entity e, const auto& entity, const auto& meshInstance) {
			auto meshPartsCount = 0;
			if (registry.hasAllComponents<MeshGeometries>(meshInstance.meshSource))
				meshPartsCount = registry.getComponent<const MeshGeometries>(meshInstance.meshSource).size();

			if (meshPartsCount <= 0)
				return;

			auto partsCount = meshInstance.slice.count == 0 ? meshPartsCount : meshInstance.slice.count;
			registry.addComponent<Renderable>(e, engine, entity.get(), partsCount);
		});
}

void MeshController::updateMeshGeometries(ecs::Registry& registry)
{
	registry.getEntities<const ecs::MeshInstance, Renderable>().each(
		[&](const ecs::MeshInstance& meshInstance, Renderable& renderable) {
			renderable.setCastShadows(meshInstance.castShadows);
			renderable.setReceiveShadows(meshInstance.receiveShadows);
			renderable.setCulling(meshInstance.culling);
			renderable.setPriority(meshInstance.priority);
		});

	registry.getEntities<const ecs::MeshInstance, Renderable>().each([&](const ecs::MeshInstance& meshInstance,
																		 Renderable& renderable) {
		if (!registry.hasAllComponents<MeshGeometries, SharedVertexBuffer, SharedIndexBuffer>(meshInstance.meshSource))
			return;

		auto& parts = registry.getComponent<const MeshGeometries>(meshInstance.meshSource);
		auto partsCount = meshInstance.slice.count == 0 ? parts.size() : meshInstance.slice.count;

		auto& vertexBuffer = registry.getComponent<const SharedVertexBuffer>(meshInstance.meshSource);
		auto& indexBuffer = registry.getComponent<const SharedIndexBuffer>(meshInstance.meshSource);

		auto* boundingBox = registry.tryGetComponent<const filament::Box>(meshInstance.meshSource);
		if (boundingBox)
			renderable.setAxisAlignedBoundingBox(*boundingBox);

		const auto numParts = std::min(partsCount, parts.size());
		for (auto i = 0; i < numParts; i++)
		{
			auto& geometry = parts.at(std::min(meshInstance.slice.offset + i, parts.size() - 1));
			renderable.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer,
									 geometry.offset, geometry.count);
		}

		if (registry.hasAllComponents<SharedMaterialInstance>(meshInstance.defaultMaterial))
		{
			const auto& materialInstance =
				registry.getComponent<const SharedMaterialInstance>(meshInstance.defaultMaterial);

			for (auto i = 0; i < numParts; i++)
				renderable.setMaterialInstanceAt(i, materialInstance);
		}
	});

	registry.getEntities<const ecs::MeshMaterial, const ecs::Child>().each(
		[&](ecs::Entity e, const auto& meshMaterial, const ecs::Child& child) {
			if (!registry.hasAllComponents<Renderable, ecs::MeshInstance>(child.parent))
				return;

			auto& renderable = registry.getComponent<Renderable>(child.parent);
			auto& mesh = registry.getComponent<const ecs::MeshInstance>(child.parent);

			if (!registry.hasAllComponents<SharedMaterialInstance>(meshMaterial.materialInstanceEntity))
			{
				renderable.resetMaterialInstance(meshMaterial.primitiveIndex);
				return;
			}

			auto& materialInstance =
				registry.getComponent<const SharedMaterialInstance>(meshMaterial.materialInstanceEntity);
			renderable.setMaterialInstanceAt(meshMaterial.primitiveIndex, materialInstance);
		});
}

void MeshController::clearDirtyRenderables(ecs::Registry& registry)
{
	registry.removeComponentFromEntities<ecs::tags::ShouldRecreateRenderable, Renderable>();
}

void MeshController::loadMeshes(filament::Engine& engine, ecs::Registry& registry)
{
	registry.getEntities<const ecs::RuntimeMesh>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::RuntimeMesh& runtimeMesh) {
			auto vb =
				toShared(render::VertexBuffer{engine, filament::VertexBuffer::Builder()
														  .vertexCount(runtimeMesh.vertexData.size())
														  .bufferCount(1)
														  .attribute(filament::VertexAttribute::POSITION, 0,
																	 filament::VertexBuffer::AttributeType::FLOAT3, 0)
														  .build(engine)});
			vb->setBufferAt(engine, 0,
							{runtimeMesh.vertexData.data(),
							 sizeof(decltype(runtimeMesh.vertexData)::value_type) * runtimeMesh.vertexData.size()});

			auto ib = toShared(render::IndexBuffer{engine, filament::IndexBuffer::Builder()
															   .indexCount(runtimeMesh.indexData.size())
															   .bufferType(filament::IndexBuffer::IndexType::USHORT)
															   .build(engine)});

			ib->setBuffer(engine, {runtimeMesh.indexData.data(),
								   sizeof(decltype(runtimeMesh.indexData)::value_type) * runtimeMesh.indexData.size()});

			registry.addOrReplaceComponent(entity, render::MeshGeometries{{0, ib->getIndexCount()}});
			registry.addOrReplaceComponent(entity, std::move(vb));
			registry.addOrReplaceComponent(entity, std::move(ib));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});

	registry.getEntities<const ecs::ResourceData, ecs::tags::IsMesh>(ecs::ExcludeComponents<SharedVertexBuffer>)
		.each([&](ecs::Entity entity, const ecs::ResourceData& resource) {

			auto filamesh = loadFilameshFromMemory(resource.data.data(), resource.data.size());
			registry.removeComponent<ecs::ResourceData>(entity);
			registry.addOrReplaceComponent(entity, toShared(createVertexBuffer(engine, filamesh)));
			registry.addOrReplaceComponent(entity, toShared(createIndexBuffer(engine, filamesh)));
			registry.addOrReplaceComponent(entity, filament::Box{filamesh.header.aabb});
			registry.addOrReplaceComponent(entity, createMeshGeometries(filamesh));
		});
}

} // namespace spatial::render