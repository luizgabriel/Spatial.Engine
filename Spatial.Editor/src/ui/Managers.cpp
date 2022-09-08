#include "Managers.h"
#include "../ecs/Components.h"
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
#include <spatial/ui/components/Basic.h>
#include <spatial/ui/components/Combo.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Popup.h>

namespace spatial::ui
{

void SceneTree::displayTree(ecs::Registry& registry, bool showDebugEntities, std::string_view search)
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

		const auto onEachNodeFn = [&](ecs::Entity entity, const auto& name) {
			if (!icontains(name.name, search))
				return;

			displayNode(registry, entity);
		};

		if (showDebugEntities)
			registry
				.getEntities<const ecs::Name>(ecs::Exclude<ecs::tags::IsResource, ecs::View, ecs::Child>) //
				.each(onEachNodeFn);
		else
			registry //
				.getEntities<const ecs::Name>(
					ecs::Exclude<ecs::tags::IsResource, ecs::View, editor::tags::IsEditorEntity, ecs::Child>)
				.each(onEachNodeFn);

		ImGui::EndTable();
	}
}

void SceneTree::displayNode(ecs::Registry& registry, ecs::Entity entity)
{
	const auto& name = registry.getComponent<ecs::Name>(entity);

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

	bool open = ImGui::TreeNodeEx(name.c_str(), childrenFlags | selectedFlags);
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
	else if (registry.hasAnyComponent<ecs::tags::IsRenderable>(entity))
		ImGui::TextDisabled("Renderable");

	if (open && hasChildren)
	{
		ecs::Parent::forEachChild(registry, entity, [&](auto child) { SceneTree::displayNode(registry, child); });

		ImGui::TreePop();
	}
}

void ResourceManager::header(std::string& search, ResourceManager::ResourceType& filter,
							 graphics::OptionalTexture icons)
{
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ui::spanToAvailWidth();

	ui::Combo::fromEnum("##ResourceTypeFilter", filter);

	ImGui::NextColumn();
	ui::Search::text(search, std::move(icons));

	ImGui::Columns(1);
}

void ResourceManager::list(ecs::Registry& registry, std::string_view search, ResourceManager::ResourceType type,
						   bool showEditorEntities)
{
	using namespace boost::algorithm;
	using namespace std::string_view_literals;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("AssetsManagerTable", 2, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Type");
		ImGui::TableHeadersRow();

		const auto eachFn = [&](ecs::Entity entity, const auto& name) {
			if (!icontains(name.name, search))
				return;

			if (type != ResourceType::All)
			{
				if (type == ResourceType::Material && !registry.hasAllComponents<ecs::tags::IsMaterial>(entity))
					return;

				if (type == ResourceType::Mesh && !registry.hasAllComponents<ecs::tags::IsMesh>(entity))
					return;

				if (type == ResourceType::Script && !registry.hasAllComponents<ecs::tags::IsScript>(entity))
					return;

				if (type == ResourceType::Texture
					&& !registry.hasAnyComponent<ecs::tags::IsImageTexture, ecs::tags::IsCubeMapTexture,
												 ecs::tags::IsIrradianceValues>(entity))
					return;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			const auto selectedFlags = registry.hasComponent<editor::tags::IsSelected>(entity)
										   ? ImGuiTreeNodeFlags_Selected
										   : ImGuiTreeNodeFlags_None;
			ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
												| ImGuiTreeNodeFlags_SpanFullWidth | selectedFlags);
			if (ImGui::IsItemClicked())
				editor::tags::IsSelected::replace(registry, entity);

			const auto* resource = registry.tryGetComponent<const ecs::FileSystemResource>(entity);
			if (resource != nullptr)
				ui::Popup::show("ResourcePopup", [&]() { ImGui::Text("%s", resource->relativePath.c_str()); });

			ImGui::TableNextColumn();
			std::string_view typeText;
			if (registry.hasAllComponents<ecs::tags::IsMesh>(entity))
				typeText = "Mesh"sv;
			else if (registry.hasAllComponents<ecs::tags::IsMaterial>(entity))
				typeText = "Material"sv;
			else if (registry.hasAllComponents<ecs::tags::IsScript>(entity))
				typeText = "Script"sv;
			else if (registry.hasAnyComponent<ecs::tags::IsImageTexture>(entity))
				typeText = "Image Texture"sv;
			else if (registry.hasAnyComponent<ecs::tags::IsCubeMapTexture>(entity))
				typeText = "CubeMap Texture"sv;
			else if (registry.hasAnyComponent<ecs::tags::IsIrradianceValues>(entity))
				typeText = "Irradiance Values"sv;

			ImGui::TextDisabled("%s", typeText.data());
		};

		if (showEditorEntities)
			registry.getEntities<const ecs::tags::IsResource, const ecs::Name>().each(eachFn);
		else
			registry
				.getEntities<const ecs::tags::IsResource, const ecs::Name>(ecs::Exclude<editor::tags::IsEditorEntity>)
				.each(eachFn);

		ImGui::EndTable();
	}
}

void MaterialsManager::list(ecs::Registry& registry, std::string_view search, bool showEditorEntities)
{
	using namespace boost::algorithm;
	using namespace std::string_view_literals;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("MaterialInstancesTable", 1, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		const auto eachFn = [&](ecs::Entity entity, const auto&, const auto& name) {
			if (!icontains(name.name, search))
				return;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			const auto selectedFlags = registry.hasComponent<editor::tags::IsSelected>(entity)
										   ? ImGuiTreeNodeFlags_Selected
										   : ImGuiTreeNodeFlags_None;
			ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
												| ImGuiTreeNodeFlags_SpanFullWidth | selectedFlags);

			if (ImGui::IsItemClicked())
				editor::tags::IsSelected::replace(registry, entity);
		};

		if (showEditorEntities)
			registry.getEntities<const ecs::MaterialInstance, const ecs::Name>().each(eachFn);
		else
			registry
				.getEntities<const ecs::MaterialInstance, const ecs::Name>(ecs::Exclude<editor::tags::IsEditorEntity>)
				.each(eachFn);

		ImGui::EndTable();
	}
}

void ViewsManager::list(ecs::Registry& registry, std::string_view search, bool showEditorEntities)
{
	using namespace boost::algorithm;
	using namespace std::string_view_literals;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("ViewsTable", 1, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		const auto eachFn = [&](ecs::Entity entity, const auto&, const auto& name) {
			if (!icontains(name.name, search))
				return;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			const auto selectedFlags = registry.hasComponent<editor::tags::IsSelected>(entity)
										   ? ImGuiTreeNodeFlags_Selected
										   : ImGuiTreeNodeFlags_None;
			ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
												| ImGuiTreeNodeFlags_SpanFullWidth | selectedFlags);

			if (ImGui::IsItemClicked())
				editor::tags::IsSelected::replace(registry, entity);
		};

		if (showEditorEntities)
			registry
				.getEntities<const ecs::View, const ecs::Name>() //
				.each(eachFn);
		else
			registry
				.getEntities<const ecs::View, const ecs::Name>(ecs::Exclude<editor::tags::IsEditorEntity>) //
				.each(eachFn);

		ImGui::EndTable();
	}
}

} // namespace spatial::ui