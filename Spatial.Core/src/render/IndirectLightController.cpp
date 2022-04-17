#include <spatial/core/Logger.h>
#include <spatial/ecs/Light.h>
#include <spatial/render/IndirectLightController.h>
#include <spatial/render/SkyboxResources.h>

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

IndirectLightController::IndirectLightController(filament::Engine& engine, FileSystem& fileSystem)
	: mEngine{engine}, mFileSystem{fileSystem}
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

void IndirectLightController::onUpdateFrame(ecs::Registry& registry)
{
	registry.getEntities<ecs::IndirectLight>().each([&](ecs::Entity entity, ecs::IndirectLight& component) {
		auto reflectionsTextureId = component.reflectionsTexturePath.getId();

		if (component.reflectionsTexturePath.isEmpty())
			return;

		if (mTextures.find(reflectionsTextureId) != mTextures.end())
			return;

		auto data = mFileSystem.readBinary(component.reflectionsTexturePath.relativePath.c_str());
		if (data.empty())
		{
			gLogger.warn("Could not load indirect light: {}", component.reflectionsTexturePath.relativePath.c_str());
			return;
		}

		loadTexture(reflectionsTextureId, &data[0], data.size());
	});

	registry.getEntities<ecs::IndirectLight>().each([&](ecs::Entity entity, ecs::IndirectLight& component) {
		auto irradianceValuesId = component.irradianceValuesPath.getId();

		if (component.irradianceValuesPath.isEmpty())
			return;

		if (mBands.find(irradianceValuesId) != mBands.end())
			return;

		auto stream = mFileSystem.openReadStream(component.irradianceValuesPath.relativePath.c_str());
		if (stream->fail())
		{
			gLogger.warn("Could not load irradiance values: {}", component.irradianceValuesPath.relativePath.c_str());
			return;
		}

		auto bands = bands_t{};
		*stream >> bands;

		loadIrradianceValues(irradianceValuesId, bands);
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

} // namespace spatial::render