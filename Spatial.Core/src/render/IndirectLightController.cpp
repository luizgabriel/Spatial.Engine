#include "spatial/common/StringHelpers.h"
#include "spatial/core/Logger.h"
#include "spatial/ecs/Light.h"
#include <fstream>
#include <spatial/render/IndirectLightController.h>
#include <spatial/render/SkyboxResources.h>

namespace spatial::render
{

IndirectLightController::IndirectLightController(filament::Engine& engine) : mEngine{engine}, mTextures{}
{
}

void IndirectLightController::loadTexture(ResourceId resourceId, const uint8_t* data, size_t size)
{
	mTextures.emplace(resourceId, createKtxTexture(mEngine, data, size));
}

void IndirectLightController::loadIrradianceValues(ResourceId resourceId, const bands_t& bands)
{
	mBands.emplace(resourceId, bands);
}

tl::expected<bands_t, ResourceError> toBandsData(const std::filesystem::path& path)
{
	auto ifs = std::ifstream{path};
	if (!ifs) return tl::make_unexpected(ResourceError::OpenFailed);

	try
	{
		auto bands = bands_t{};
		ifs >> bands;
		return bands;
	}
	catch (const std::ios::failure& e)
	{
		return tl::make_unexpected(ResourceError::ParseError);
	}
}

void IndirectLightController::onUpdateFrame(ecs::Registry& registry)
{
	registry.getEntities<ecs::IndirectLight>().each([&](ecs::Entity entity, ecs::IndirectLight& component) {
		auto reflectionsTextureId = component.reflectionsTexturePath.getId();

		if (mTextures.find(reflectionsTextureId) == mTextures.end())
		{
			auto result = makeAbsolutePath(mRootPath, component.reflectionsTexturePath.relativePath)
							  .and_then(validateResourcePath)
							  .and_then(openFileReadStream)
							  .transform(toVectorData)
							  .map_error(logResourceError);

			if (!result.has_value())
				return;

			const auto& value = result.value();
			loadTexture(reflectionsTextureId, &value[0], value.size());
		}
	});

	registry.getEntities<ecs::IndirectLight>().each([&](ecs::Entity entity, ecs::IndirectLight& component) {
		auto irradianceValuesId = component.irradianceValuesPath.getId();

		if (mBands.find(irradianceValuesId) == mBands.end())
		{
			auto result = makeAbsolutePath(mRootPath, component.reflectionsTexturePath.relativePath)
							  .and_then(validateResourcePath)
							  .and_then(toBandsData)
							  .map_error(logResourceError);

			if (!result.has_value())
				return;

			loadIrradianceValues(irradianceValuesId, result.value());
		}
	});

	registry.getEntities<ecs::IndirectLight>(ecs::ExcludeComponents<IndirectLight>)
		.each([&](ecs::Entity entity, ecs::IndirectLight& component) {
			const auto reflectionsId = component.reflectionsTexturePath.getId();

			if (mTextures.find(reflectionsId) == mTextures.end())
				return;

			const auto& reflectionsTexture = mTextures.at(reflectionsId);

			auto builder =
				filament::IndirectLight::Builder().intensity(component.intensity).reflections(reflectionsTexture.get());

			if (mBands.find(reflectionsId) != mBands.end())
			{
				const auto& irradianceBands = mBands.at(reflectionsId);
				builder = builder.irradiance(3, &irradianceBands[0]);
			}

			registry.addComponent<IndirectLight>(entity, mEngine, builder.build(mEngine));
		});

	registry.getEntities<ecs::IndirectLight, IndirectLight>().each(
		[&](ecs::IndirectLight& component, IndirectLight& light) { light->setIntensity(component.intensity); });
}

void IndirectLightController::setRootPath(const std::filesystem::path& rootPath)
{
	mRootPath = rootPath;
}

} // namespace spatial::render