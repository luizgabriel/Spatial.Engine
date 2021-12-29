#include "Serialization.h"

namespace spatial::editor
{

tl::expected<ecs::Registry, ResourceError> parseRegistry(std::istream&& istream)
{
	auto registry = ecs::Registry{};
	try
	{
		auto archive = JsonInputArchive{std::ref(istream)};
		ecs::deserialize<DefaultMaterial, SkyBoxMaterial, GridMaterial, EditorCamera, tags::IsEditorEntity,
						 tags::IsSkyBoxMesh, tags::IsEditorView>(archive, registry);
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
	ecs::serialize<DefaultMaterial, SkyBoxMaterial, GridMaterial, EditorCamera, tags::IsEditorEntity,
				   tags::IsSkyBoxMesh, tags::IsEditorView>(archive, registry);
}

} // namespace spatial::editor