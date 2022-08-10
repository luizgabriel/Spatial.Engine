#include <spatial/ecs/Resource.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::graphics
{

OptionalTexture getTexture(const ecs::Registry& registry, ecs::Entity entity)
{
	if (!registry.hasComponent<SharedTexture>(entity))
		return std::nullopt;

	return registry.getComponent<const SharedTexture>(entity);
}

OptionalTexture getTexture(const ecs::Registry& registry, std::string_view resourcePath)
{
	return getTexture(registry, ecs::Resource::find(registry, resourcePath));
}

} // namespace spatial::graphics