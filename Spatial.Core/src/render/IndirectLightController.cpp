#include <spatial/core/Logger.h>
#include <spatial/ecs/Light.h>
#include <spatial/render/IndirectLightController.h>
#include <spatial/render/SkyboxResources.h>

namespace spatial::render
{

static auto gLogger = createDefaultLogger();

IndirectLightController::IndirectLightController(filament::Engine& engine)
	: mEngine{engine}
{
}

void IndirectLightController::onUpdateFrame(ecs::Registry& registry)
{
	registry.getEntities<const ecs::IndirectLight>(ecs::ExcludeComponents<IndirectLight>)
		.each([&](ecs::Entity entity, const ecs::IndirectLight& component) {
			const auto* reflectionsTexture = registry.tryGetComponent<const SharedTexture>(component.reflectionsTexture);

			if (!reflectionsTexture)
				return;

			auto builder =
				filament::IndirectLight::Builder().intensity(component.intensity).reflections(reflectionsTexture->get());

			const auto* irradianceValues = registry.tryGetComponent<bands_t>(component.irradianceValues);

			if (irradianceValues)
				builder = builder.irradiance(3, &irradianceValues->at(0));

			registry.addComponent<IndirectLight>(entity, mEngine, builder.build(mEngine));
		});

	registry.getEntities<const ecs::IndirectLight, IndirectLight>().each(
		[&](const ecs::IndirectLight& component, IndirectLight& light) { light->setIntensity(component.intensity); });
}

} // namespace spatial::render