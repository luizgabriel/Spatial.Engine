#include "Managers.h"
#include <boost/algorithm/string/predicate.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Script.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ecs/View.h>
#include <spatial/ui/components/DragAndDrop.h>

namespace spatial::ui
{

ecs::RuntimeView getViewFromFilter(ecs::Registry& registry, const ComponentFilter& componentFilter,
								   bool showEditorEntities) noexcept
{
	auto runtimeView = ecs::RuntimeView{};

	if (componentFilter == ComponentFilter::MeshInstances)
		runtimeView.iterate(registry.getStorage<ecs::MeshInstance>());

	if (componentFilter == ComponentFilter::Lights)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsLight>());

	if (componentFilter == ComponentFilter::MaterialInstances)
		runtimeView.iterate(registry.getStorage<ecs::MaterialInstance>());

	if (componentFilter == ComponentFilter::Views)
		runtimeView.iterate(registry.getStorage<ecs::View>());

	if (componentFilter == ComponentFilter::Textures)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsTexture>());

	if (componentFilter == ComponentFilter::Scripts)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsScript>());

	if (componentFilter == ComponentFilter::Meshes)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsMesh>());

	if (componentFilter == ComponentFilter::Cameras)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsCamera>());

	if (componentFilter == ComponentFilter::Materials)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsMaterial>());

	if (componentFilter == ComponentFilter::Resources)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsResource>());

	if (componentFilter == ComponentFilter::Renderables)
		runtimeView.iterate(registry.getStorage<ecs::tags::IsRenderable>());

	if (componentFilter == ComponentFilter::All)
		runtimeView.iterate(registry.getStorage<ecs::Name>());

	runtimeView.exclude(registry.getStorage<ecs::Child>());

	if (!showEditorEntities)
	{
		runtimeView.exclude(registry.getStorage<editor::tags::IsEditorEntity>());
	}

	return runtimeView;
}

void displayNode(ecs::Registry& registry, ecs::Entity entity)
{
	const auto* name = registry.tryGetComponent<ecs::Name>(entity);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	const auto* relation = registry.tryGetComponent<ecs::Parent>(entity);
	const auto hasChildren = relation && relation->childrenCount > 0;

	const auto selectedFlags =
		registry.hasComponent<editor::tags::IsSelected>(entity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
	const auto childrenFlags =
		hasChildren
			? ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
			: ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
				  | ImGuiTreeNodeFlags_SpanFullWidth;

	bool open = (name) ? ImGui::TreeNodeEx(name->c_str(), childrenFlags | selectedFlags)
					   : ImGui::TreeNodeEx(fmt::format("Entity #{0}", static_cast<uint32_t>(entity)).c_str(), childrenFlags | selectedFlags);

	auto shiftPressed = ImGui::IsKeyPressed(ImGuiKey_LeftShift);
	if (ImGui::IsItemClicked() && shiftPressed)
		registry.addComponent<editor::tags::IsSelected>(entity);

	if (ImGui::IsItemClicked())
		editor::tags::IsSelected::replace(registry, entity);

	if (auto dnd = ui::DragAndDropSource{}; dnd.isStarted())
		dnd.setPayload(fmt::to_string(static_cast<uint32_t>(entity)));

	if (auto dnd = ui::DragAndDropTarget{}; dnd.isStarted())
	{
		if (auto payload = dnd.getPayload(); payload)
		{
			auto draggedEntity = static_cast<ecs::Entity>(std::stoi(*payload));
			assert(registry.isValid(draggedEntity));
			if (draggedEntity != entity)
			{
				ecs::Child::changeParent(registry, draggedEntity, entity);
			}
		}
	}

	ImGui::TableNextColumn();

	if (registry.hasAnyComponent<ecs::tags::IsMainView>(entity))
		ImGui::TextDisabled("Main View");
	else if (registry.hasAnyComponent<ecs::tags::IsCamera>(entity))
		ImGui::TextDisabled("Camera");
	else if (registry.hasAnyComponent<ecs::tags::IsLight>(entity))
		ImGui::TextDisabled("Light");
	else if (registry.hasAnyComponent<ecs::View>(entity))
		ImGui::TextDisabled("View");
	else if (registry.hasAnyComponent<ecs::tags::IsScript>(entity))
		ImGui::TextDisabled("Script");
	else if (registry.hasAnyComponent<ecs::tags::IsImageTexture>(entity))
		ImGui::TextDisabled("Image Texture");
	else if (registry.hasAnyComponent<ecs::tags::IsCubeMapTexture>(entity))
		ImGui::TextDisabled("Cubemap Texture");
	else if (registry.hasAnyComponent<ecs::tags::IsTexture>(entity))
		ImGui::TextDisabled("Texture");
	else if (registry.hasAnyComponent<ecs::MaterialInstance>(entity))
		ImGui::TextDisabled("Material Instance");
	else if (registry.hasAnyComponent<ecs::tags::IsMaterial>(entity))
		ImGui::TextDisabled("Material");
	else if (registry.hasAnyComponent<ecs::tags::IsMesh>(entity))
		ImGui::TextDisabled("Mesh");
	else if (registry.hasAnyComponent<ecs::MeshInstance>(entity))
		ImGui::TextDisabled("MeshInstance");
	else if (registry.hasAnyComponent<ecs::MeshPart>(entity))
		ImGui::TextDisabled("Mesh Part");
	else if (registry.hasAnyComponent<ecs::MeshPrimitive>(entity))
		ImGui::TextDisabled("Mesh Primitive");
	else if (registry.hasAnyComponent<ecs::tags::IsMaterial>(entity))
		ImGui::TextDisabled("Material");
	else if (registry.hasAnyComponent<ecs::tags::IsIrradianceValues>(entity))
		ImGui::TextDisabled("Irradiance Values");
	else if (registry.hasAnyComponent<ecs::tags::IsRenderable>(entity))
		ImGui::TextDisabled("Renderable");

	if (open && hasChildren)
	{
		ecs::Parent::forEachChild(registry, entity, [&](auto child) { displayNode(registry, child); });

		ImGui::TreePop();
	}
}

void RegistryManager::list(ecs::Registry& registry, std::string_view search, ComponentFilter componentFilter,
						   bool showEditorEntities)
{
	using namespace boost::algorithm;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
		registry.removeComponentFromEntities<editor::tags::IsSelected>();

	const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable
								  | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("Scene Graph Entities", 2, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Type");
		ImGui::TableHeadersRow();

		const auto onEachNodeFn = [&](ecs::Entity entity) {
			const auto* name = registry.tryGetComponent<ecs::Name>(entity);

			if (name && !icontains(name->c_str(), search))
				return;

			displayNode(registry, entity);
		};

		auto runtimeView = getViewFromFilter(registry, componentFilter, showEditorEntities);
		runtimeView.each(onEachNodeFn);

		ImGui::EndTable();
	}
}

} // namespace spatial::ui