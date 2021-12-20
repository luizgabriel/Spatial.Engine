#include "Serialization.h"

namespace spatial::editor
{

tl::expected<ecs::Registry, ResourceError> parseRegistry(std::istream&& istream)
{
	auto archive = XMLInputArchive{istream};
	auto registry = ecs::Registry{};
	try
	{
		ecs::deserialize<editor::DefaultMaterial, editor::EditorCamera, editor::tags::IsEditorEntity>(archive,
																									  registry);
	}
	catch (const std::exception& e)
	{
		return tl::make_unexpected(ResourceError::ParseError);
	}

	return registry;
}

}