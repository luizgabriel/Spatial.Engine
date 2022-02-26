#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/SkyboxResources.h>

using namespace boost::algorithm;

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

MaterialController::MaterialController(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}, mJobQueue{}, mTextures{}
{
	mJobQueue.connect<LoadResourceEvent<ImageTexture>>(*this);
	mJobQueue.connect<LoadResourceEvent<CubeMapTexture>>(*this);
}

void MaterialController::load(uint32_t resourceId, Texture&& texture)
{
	mTextures.emplace(resourceId, std::move(texture));
}

void MaterialController::onStartFrame()
{
	mJobQueue.update();
}

void MaterialController::onEvent(const LoadResourceEvent<ImageTexture>& event)
{
	if (event.texture.isEmpty())
		return;

	if (!ends_with(event.texture.relativePath.c_str(), ".png")
		&& !ends_with(event.texture.relativePath.c_str(), ".jpg"))
		return;

	auto data = mFileSystem.readBinary(event.texture.relativePath.c_str());
	if (data.empty())
	{
		gLogger.warn("Image Texture not found: {}", event.texture.relativePath.c_str());
		return;
	}

	auto texture = render::createTexture(mEngine, data.data(), data.size());
	mTextures.emplace(event.texture.getId(), std::move(texture));
}

void MaterialController::onEvent(const LoadResourceEvent<CubeMapTexture>& event)
{
	if (event.texture.isEmpty())
		return;

	if (!ends_with(event.texture.relativePath.c_str(), ".ktx"))
		return;

	auto data = mFileSystem.readBinary(event.texture.relativePath.c_str());
	if (data.empty())
	{
		gLogger.warn("CubeMap Texture not found: {}", event.texture.relativePath.c_str());
		return;
	}

	auto texture = render::createKtxTexture(mEngine, data.data(), data.size());
	mTextures.emplace(event.texture.getId(), std::move(texture));
}

void MaterialController::onUpdateFrame(ecs::Registry& registry)
{
	using namespace boost::algorithm;

	registry.getEntities<const ecs::PrecompiledMaterial>(ecs::ExcludeComponents<SharedMaterial>)
		.each([&](ecs::Entity entity, const ecs::PrecompiledMaterial& precompiledMaterial) {
			if (precompiledMaterial.resource.isEmpty()
				|| !ends_with(precompiledMaterial.resource.relativePath.c_str(), ".filamat"))
				return;

			auto data = mFileSystem.readBinary(precompiledMaterial.resource.relativePath.c_str());
			auto material = toShared(render::createMaterial(mEngine, data.data(), data.size()));

			registry.addComponent<SharedMaterial>(entity, material);
		});

	registry.getEntities<const ecs::MaterialInstance>(ecs::ExcludeComponents<SharedMaterialInstance>)
		.each([&](ecs::Entity entity, const ecs::MaterialInstance& materialInstance) {
			if (!registry.hasAllComponents<SharedMaterial>(materialInstance.materialEntity))
				return;

			auto material = registry.getComponent<const SharedMaterial>(materialInstance.materialEntity);
			registry.addComponent<SharedMaterialInstance>(entity,
														  toShared(render::createMaterialInstance(mEngine, material)));
		});

	auto d1 = registry.getEntities<ecs::tags::IsMaterialDirty, SharedMaterialInstance>();
	registry.removeComponent<SharedMaterialInstance>(d1.begin(), d1.end());

	auto d2 = registry.getEntities<ecs::tags::IsMaterialDirty, SharedMaterial>();
	registry.removeComponent<SharedMaterial>(d2.begin(), d2.end());

	auto d3 = registry.getEntities<ecs::tags::IsMaterialDirty>();
	registry.removeComponent<ecs::tags::IsMaterialDirty>(d3.begin(), d3.end());
}

} // namespace spatial::render