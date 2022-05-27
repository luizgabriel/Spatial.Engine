#include <spatial/ecs/Resource.h>
#include <spatial/graphics/Resources.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::graphics
{

const filament::Texture* getTexture(const ecs::Registry& registry, ecs::Entity entity)
{
	const auto* texture = registry.tryGetComponent<SharedTexture>(entity);
	if (texture)
		return texture->get();

	return nullptr;
}

const filament::Texture* getTexture(const ecs::Registry& registry, std::string_view resourcePath)
{
	return getTexture(registry, ecs::Resource::find(registry, resourcePath));
}

} // namespace spatial::graphics