#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/ecs/Tags.h>

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
		[&](const auto& meshInstance, auto& renderable) {
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

			for (auto i = 0; i < std::min(partsCount, parts.size()); i++)
			{
				const auto& geometry = parts[std::min(meshInstance.slice.offset + i, parts.size() - 1)];
				renderable.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer, indexBuffer,
										 geometry.offset, geometry.count);
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
	auto d1 = registry.getEntities<ecs::tags::ShouldRecreateRenderable, Renderable>();
	registry.removeComponent<Renderable>(d1.begin(), d1.end());
}

void MeshController::onStartFrame(ecs::Registry& registry)
{
	using namespace boost::algorithm;

	registry.getEntities<const ecs::Mesh>(ecs::ExcludeComponents<ecs::tags::IsMeshLoaded>)
		.each([&](ecs::Entity entity, const ecs::Mesh& mesh) {
			if (mesh.resource.isEmpty() || !ends_with(mesh.resource.filename(), ".filamesh"))
				return;

			auto stream = mFileSystem.openReadStream(mesh.resource.relativePath.c_str());
			if (stream->bad())
			{
				gLogger.warn("Could not load mesh: {}", mesh.resource.relativePath.c_str());
				return;
			}

			auto filamesh = FilameshFile{};
			*stream >> filamesh;

			registry.addOrReplaceComponent(entity, toShared(createVertexBuffer(mEngine, filamesh)));
			registry.addOrReplaceComponent(entity, toShared(createIndexBuffer(mEngine, filamesh)));
			registry.addOrReplaceComponent(entity, filament::Box{filamesh.header.aabb});
			registry.addOrReplaceComponent(entity, createMeshGeometries(filamesh));
			registry.addOrReplaceComponent<ecs::tags::IsMeshLoaded>(entity);
		});
}

} // namespace spatial::render