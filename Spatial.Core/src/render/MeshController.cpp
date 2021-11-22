#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/render/Entity.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/Resources.h>
#include <spatial/resources/FilameshFile.h>
#include <fstream>

namespace spatial::render
{

auto gLogger = createDefaultLogger();

MeshController::MeshController(filament::Engine& engine)
	: mEngine{engine},
	  mRoot{},
	  mVertexBuffers{},
	  mIndexBuffers{},
	  mMeshGeometries{},
	  mBoundingBoxes{}
{
}

void MeshController::load(MeshId resourceId, const FilameshFile& filamesh)
{
	mVertexBuffers.emplace(resourceId, createVertexBuffer(mEngine, filamesh));
	mIndexBuffers.emplace(resourceId, createIndexBuffer(mEngine, filamesh));
	mBoundingBoxes.emplace(resourceId, filamesh.header.aabb);

	auto geometries = std::vector<MeshGeometry>(filamesh.parts.size());

	for (auto i = 0; i < filamesh.parts.size(); i++)
	{
		const auto& part = filamesh.parts[i];
		geometries.at(i) = MeshGeometry{part.offset, part.count};
	}

	mMeshGeometries.emplace(resourceId, std::move(geometries));
}

void MeshController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	populateMeshesDatabase(registry);
	createRenderableMeshes(registry);
	updateMeshGeometries(registry);
	clearDeletedOrDirtyMeshes(registry);
}

void MeshController::createRenderableMeshes(ecs::Registry& registry)
{
	registry.getEntities<const Entity, ecs::Mesh>(ecs::ExcludeComponents<Renderable>)
		.each([&](ecs::Entity e, const auto& entity, const auto& mesh) {
			const auto resourceId = mesh.getResourceId();

			if (!hasMeshData(mesh.getResourceId()))
				return;

			const auto& parts = mMeshGeometries.at(resourceId);
			const auto partsCount = mesh.partsCount == 0 ? parts.size() : mesh.partsCount;
			registry.addComponent<Renderable>(e, mEngine, entity.get(), partsCount);
		});
}

void MeshController::updateMeshGeometries(ecs::Registry& registry)
{
	registry.getEntities<const ecs::Mesh, Renderable>().each([&, this](const auto& mesh, auto& renderable) {
		const auto resourceId = mesh.getResourceId();

		if (!hasMeshData(resourceId))
			return;

		const auto& parts = mMeshGeometries.at(resourceId);
		auto& vertexBuffer = mVertexBuffers.at(resourceId);
		auto& indexBuffer = mIndexBuffers.at(resourceId);
		auto& boundingBox = mBoundingBoxes.at(resourceId);

		renderable.setAxisAlignedBoundingBox(boundingBox);
		renderable.setCastShadows(mesh.castShadows);
		renderable.setReceiveShadows(mesh.receiveShadows);
		renderable.setCulling(mesh.culling);

		const auto partsCount = mesh.partsCount == 0 ? parts.size() : mesh.partsCount;

		for (auto i = 0; i < std::min(partsCount, parts.size()); i++)
		{
			const auto& geometry = parts[std::min(mesh.partsOffset + i, parts.size() - 1)];
			renderable.setGeometryAt(i, Renderable::PrimitiveType::TRIANGLES, vertexBuffer.get(), indexBuffer.get(),
									 geometry.offset, geometry.count);

			if (registry.hasAnyComponent<MaterialInstance>(mesh.defaultMaterial))
			{
				const auto& materialInstance = registry.getComponent<const MaterialInstance>(mesh.defaultMaterial);
				renderable.setMaterialInstanceAt(i, materialInstance.get());
			}
			else if (mDefaultMaterialInstance.get())
			{
				renderable.setMaterialInstanceAt(i, mDefaultMaterialInstance.get());
			}
		}
	});
}

bool MeshController::hasMeshData(MeshId resourceId) const
{
	return mMeshGeometries.find(resourceId) != mMeshGeometries.end();
}

void MeshController::clearDeletedOrDirtyMeshes(ecs::Registry& registry)
{
	{
		auto view = registry.getEntities<Renderable>(ecs::ExcludeComponents<ecs::Mesh>);
		registry.removeComponent<Renderable>(view.begin(), view.end());
	}
}

void MeshController::setRootPath(const std::filesystem::path& root)
{
	mRoot = root;
}

void MeshController::populateMeshesDatabase(ecs::Registry& registry)
{
	registry.getEntities<const ecs::Mesh>().each([this](auto& mesh) {
		const auto resourceId = mesh.getResourceId();

		if (hasMeshData(resourceId))
			return;

		const auto path = mRoot / mesh.resourcePath;

		if (!boost::algorithm::ends_with(path.string(), ".filamesh") || !std::filesystem::exists(path))
			return;

		auto fs = std::ifstream{path};
		if (!fs)
			return;

		try
		{
			auto filamesh = FilameshFile{};
			fs >> filamesh;
			load(resourceId, filamesh);
		}
		catch (const std::ios::failure& e)
		{
			gLogger.error(fmt::format("Could not load filamesh file: {0}\n Reason: ", path.c_str(), e.what()));
		}
	});
}

void MeshController::setDefaultMaterialInstance(const SharedMaterialInstance& defaultMaterialInstance)
{
	mDefaultMaterialInstance = defaultMaterialInstance;
}

} // namespace spatial::render