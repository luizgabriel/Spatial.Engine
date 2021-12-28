#include "Serialization.h"

namespace spatial::editor
{

tl::expected<ecs::Registry, ResourceError> parseRegistry(std::istream&& istream)
{
	auto registry = ecs::Registry{};
	try
	{
		auto archive = XMLInputArchive{std::ref(istream)};
		ecs::deserialize<DefaultMaterial, SkyBoxMaterial, EditorCamera, tags::IsEditorEntity, tags::IsSkyBoxMesh,
						 tags::IsEditorView>(archive, registry);
	}
	catch (const std::exception& e)
	{
		return tl::make_unexpected(ResourceError::ParseError);
	}

	return registry;
}

} // namespace spatial::editor