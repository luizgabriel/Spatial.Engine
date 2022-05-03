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
	registry.getEntities<const ecs::IndirectLight>().each([&](ecs::Entity entity, const ecs::IndirectLight& component) {
		auto reflectionsTextureId = entt::hashed_string::value(component.reflectionsTexturePath.c_str());

		if (component.reflectionsTexturePath.empty())
			return;

		if (mTextures.find(reflectionsTextureId) != mTextures.end())
			return;

		const auto data = mFileSystem.readBinary(component.reflectionsTexturePath);
		if (data.empty())
		{
			gLogger.warn("Could not load indirect light: {}", component.reflectionsTexturePath);
			return;
		}

		loadTexture(reflectionsTextureId, &data[0], data.size());
	});

	registry.getEntities<const ecs::IndirectLight>().each([&](ecs::Entity entity, const ecs::IndirectLight& component) {
		const auto irradianceValuesId = entt::hashed_string::value(component.irradianceValuesPath.c_str());

		if (component.irradianceValuesPath.empty())
			return;

		if (mBands.find(irradianceValuesId) != mBands.end())
			return;

		auto stream = mFileSystem.openReadStream(component.irradianceValuesPath);
		if (stream->fail())
		{
			gLogger.warn("Could not load irradiance values: {}", component.irradianceValuesPath);
			return;
		}

		auto bands = bands_t{};
		*stream >> bands;

		loadIrradianceValues(irradianceValuesId, bands);
	});

	registry.getEntities<const ecs::IndirectLight>(ecs::ExcludeComponents<IndirectLight>)
		.each([&](ecs::Entity entity, const ecs::IndirectLight& component) {
			const auto reflectionsId = entt::hashed_string::value(component.reflectionsTexturePath.c_str());

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

	registry.getEntities<const ecs::IndirectLight, IndirectLight>().each(
		[&](const ecs::IndirectLight& component, IndirectLight& light) { light->setIntensity(component.intensity); });
}

} // namespace spatial::render