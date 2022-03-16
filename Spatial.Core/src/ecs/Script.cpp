#include <spatial/ecs/Script.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/RegistryUtils.h>

namespace spatial::ecs {


EntityConstHandle Script::find(const Registry& registry, const std::filesystem::path& resourcePath)
{
	auto view = registry.getEntities<const Script>();
	auto it = std::find_if(view.begin(), view.end(), [&](ecs::Entity entity) {
		auto& material = registry.getComponent<const Script>(entity);
		return material.resource.relativePath == resourcePath;
	});

	if (it == view.end())
		return handleOf(registry, NullEntity);

	return handleOf(registry, *it);
}

EntityHandle Script::findOrCreate(Registry& registry, const std::filesystem::path& resourcePath)
{
	auto found = Script::find(registry, resourcePath);
	if (found)
		return handleOf(registry, found);

	auto material = createEntity(registry);
	material.add(Name{resourcePath.filename()});
	material.add(Script{resourcePath});
	material.add<ecs::tags::IsResource>();

	return material;
}

const char* ScriptInfo::Property::getTypeName() const
{
	return std::visit([](const auto& t){
		return std::decay_t<decltype(t)>::typeName;
	}, type);
}

bool ScriptInfo::Property::operator<(const ScriptInfo::Property& rhs) const
{
	return name < rhs.name;
}

}