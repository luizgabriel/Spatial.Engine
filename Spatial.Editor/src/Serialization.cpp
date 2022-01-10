#include "Serialization.h"
#include <spatial/resources/ResourceLoader.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

namespace spatial::editor
{

tl::expected<ecs::Registry, ResourceError> parseRegistry(std::istream&& istream)
{
	auto registry = ecs::Registry{};
	try
	{
		auto archive = JsonInputArchive{istream};
		ecs::deserialize<ColorMaterial, SkyBoxMaterial, GridMaterial, EditorCamera, tags::IsEditorEntity,
						 tags::IsSkyBoxMesh, tags::IsEditorView, tags::IsGridPlane>(archive, registry);
	}
	catch (const std::exception& e)
	{
		return tl::make_unexpected(ResourceError::ParseError);
	}

	return registry;
}

void writeRegistry(const ecs::Registry& registry, std::ostream&& ostream)
{
	auto archive = JsonOutputArchive{ostream};
	ecs::serialize<ColorMaterial, SkyBoxMaterial, GridMaterial, StandardOpaqueMaterial, EditorCamera, tags::IsEditorEntity,
				   tags::IsSkyBoxMesh, tags::IsEditorView, tags::IsGridPlane>(archive, registry);
}

} // namespace spatial::editor