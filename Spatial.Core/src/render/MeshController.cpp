#include "spatial/ecs/Resource.h"
#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

MeshController::MeshController(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}
{
}

void MeshController::onUpdateFrame(ecs::Registry& registry)
{
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	clearDirtyRenderables(registry);
}

void MeshController::createRenderableMeshes(ecs::Registry& registry)
{
	registry.getEntities<const Entity, const ecs::MeshInstance>(ecs::ExcludeComponents<Renderable>)
		.each([&](ecs::Entity e, const auto& entity, const auto& meshInstance) {
			auto meshPartsCount = 0;
			if (registry.hasAllComponents<MeshGeometries>(meshInstance.meshSource))
				meshPartsCount = registry.getComponent<const MeshGeometries>(meshInstance.meshSource).size();

			if (meshPartsCount <= 0)
				return;

			const auto partsCount = meshInstance.slice.count == 0 ? meshPartsCount : meshInstance.slice.count;
			registry.addComponent<Renderable>(e, mEngine, entity.get(), partsCount);
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

	registry.getEntities<const ecs::MeshInstance, Renderable>().each(
		[&](const ecs::MeshInstance& meshInstance, Renderable& renderable) {
			if (!registry.hasAllComponents<MeshGeometries, SharedVertexBuffer, SharedIndexBuffer>(
					meshInstance.meshSource))
				return;

			const auto& parts = registry.getComponent<const MeshGeometries>(meshInstance.meshSource);
			const auto partsCount = meshInstance.slice.count == 0 ? parts.size() : meshInstance.slice.count;

			auto vertexBuffer = registry.getComponent<const SharedVertexBuffer>(meshInstance.meshSource);
			auto indexBuffer = registry.getComponent<const SharedIndexBuffer>(meshInstance.meshSource);

			const auto* boundingBox = registry.tryGetComponent<filament::Box>(meshInstance.meshSource);
			if (boundingBox)
				renderable.setAxisAlignedBoundingBox(*boundingBox);

			const auto numParts = std::min(partsCount, parts.size());

			for (auto i = 0; i < numParts; i++)
			{
				const auto& geometry = parts[std::min(meshInstance.slice.offset + i, parts.size() - 1)];
				renderable.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer,
										 geometry.offset, geometry.count);
			}

			if (registry.hasAllComponents<SharedMaterialInstance>(meshInstance.defaultMaterial)) {
				const auto& materialInstance = registry.getComponent<const SharedMaterialInstance>(meshInstance.defaultMaterial);

				for (auto i = 0; i < numParts; i++)
					renderable.setMaterialInstanceAt(i, materialInstance);
			}
		});

	registry.getEntities<const ecs::MeshMaterial, const ecs::Child>().each([&](ecs::Entity e, const auto& meshMaterial,
																			   const ecs::Child& child) {
		if (!registry.hasAllComponents<Renderable, ecs::MeshInstance>(child.parent))
			return;

		auto& renderable = registry.getComponent<Renderable>(child.parent);
		const auto& mesh = registry.getComponent<const ecs::MeshInstance>(child.parent);

		if (!registry.hasAllComponents<SharedMaterialInstance>(meshMaterial.materialInstanceEntity))
		{
			renderable.resetMaterialInstance(meshMaterial.primitiveIndex);
			return;
		}

		const auto& materialInstance = registry.getComponent<const SharedMaterialInstance>(meshMaterial.materialInstanceEntity);
		renderable.setMaterialInstanceAt(meshMaterial.primitiveIndex, materialInstance);
	});
}

void MeshController::clearDirtyRenderables(ecs::Registry& registry)
{
	registry.removeComponentFromEntities<ecs::tags::ShouldRecreateRenderable, Renderable>();
}

void MeshController::onStartFrame(ecs::Registry& registry)
{
	using namespace boost::algorithm;

	registry.getEntities<const ecs::Resource, ecs::tags::IsMesh>(ecs::ExcludeComponents<ecs::tags::IsResourceLoaded>)
		.each([&](ecs::Entity entity, const ecs::Resource& resource) {
			if (resource.relativePath.empty() || !ends_with(resource.relativePath.c_str(), ".filamesh"))
				return;

			auto stream = mFileSystem.openReadStream(resource.relativePath.string());
			if (stream->bad())
			{
				gLogger.warn("Could not load mesh: {}", resource.relativePath.string());
				return;
			}

			auto filamesh = FilameshFile{};
			*stream >> filamesh;

			registry.addOrReplaceComponent(entity, toShared(createVertexBuffer(mEngine, filamesh)));
			registry.addOrReplaceComponent(entity, toShared(createIndexBuffer(mEngine, filamesh)));
			registry.addOrReplaceComponent(entity, filament::Box{filamesh.header.aabb});
			registry.addOrReplaceComponent(entity, createMeshGeometries(filamesh));
			registry.addComponent<ecs::tags::IsResourceLoaded>(entity);
		});
}

} // namespace spatial::render