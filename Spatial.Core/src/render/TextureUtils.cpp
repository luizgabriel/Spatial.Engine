#include <spatial/ecs/Resource.h>
#include <spatial/render/Resources.h>
#include <spatial/render/TextureUtils.h>

namespace spatial::render
{

const filament::Texture* getTexture(const ecs::Registry& registry, ecs::Entity entity)
{
	const auto* texture = registry.tryGetComponent<render::SharedTexture>(entity);
	if (texture)
		return texture->get();

	return nullptr;
}

const filament::Texture* getTexture(const ecs::Registry& registry, std::string_view resourcePath)
{
	return getTexture(registry, ecs::Resource::find(registry, resourcePath));
}

} // namespace spatial::render