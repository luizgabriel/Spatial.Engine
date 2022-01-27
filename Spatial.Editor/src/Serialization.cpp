#include "Serialization.h"
#include <spatial/resources/ResourceLoaderUtils.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

namespace spatial::editor
{

ecs::Registry parseRegistry(std::istream& istream)
{
	auto registry = ecs::Registry{};

	auto archive = JsonInputArchive{istream};
	ecs::deserialize<ColorMaterial, SkyBoxMaterial, GridMaterial, StandardOpaqueMaterial, EditorCamera,
					 tags::IsEditorEntity, tags::IsSkyBoxMeshInstance, tags::IsEditorView, tags::IsGridPlane,
					 tags::IsSkyBoxMeshResource>(archive, registry);

	return registry;
}

void writeRegistry(const ecs::Registry& registry, std::ostream& ostream)
{
	auto archive = JsonOutputArchive{ostream};
	ecs::serialize<ColorMaterial, SkyBoxMaterial, GridMaterial, StandardOpaqueMaterial, EditorCamera,
				   tags::IsEditorEntity, tags::IsSkyBoxMeshInstance, tags::IsEditorView, tags::IsGridPlane,
				   tags::IsSkyBoxMeshResource>(archive, registry);
}

} // namespace spatial::editor