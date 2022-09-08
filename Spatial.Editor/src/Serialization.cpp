#include "Serialization.h"
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>
#include <spatial/serialization/Snapshot.h>

namespace spatial::editor
{

template <typename Archive, typename SnapshotType>
void serializeEditorComponents(Archive& ar, SnapshotType& snapshot)
{
	snapshot.template component<ColorMaterial>(ar);
	snapshot.template component<SkyBoxMaterial>(ar);
	snapshot.template component<GridMaterial>(ar);
	snapshot.template component<StandardOpaqueMaterial>(ar);
	snapshot.template component<EditorCamera>(ar);
	snapshot.template component<tags::IsEditorEntity>(ar);
	snapshot.template component<tags::IsSelected>(ar);
	snapshot.template component<tags::IsSelectedView>(ar);
	snapshot.template component<tags::IsEditorWhiteTexture>(ar);
	snapshot.template component<tags::IsEditorBlackTexture>(ar);
	snapshot.template component<tags::IsSkyboxMesh>(ar);
}

ecs::Registry parseRegistry(std::istream&& istream)
{
	auto registry = ecs::Registry{};

	{
		auto archive = JsonInputArchive{istream};
		auto snapshot = ecs::SnapshotLoader{registry};
		snapshot.entities(archive);
		ecs::serializeCoreComponents(archive, snapshot);
		serializeEditorComponents(archive, snapshot);
	}

	return registry;
}

void writeRegistry(const ecs::Registry& registry, std::ostream& ostream)
{
	auto archive = JsonOutputArchive{ostream};
	auto snapshot = ecs::Snapshot{registry};
	snapshot.entities(archive);
	ecs::serializeCoreComponents(archive, snapshot);
	serializeEditorComponents(archive, snapshot);
}

} // namespace spatial::editor