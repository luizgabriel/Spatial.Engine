#include "CustomComponents.h"
#include "Materials.h"
#include "EditorCamera.h"
#include "Tags.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <imgui.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/TextureView.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/Popup.h>

namespace spatial::ui
{

template <>
void componentInput<editor::DefaultMaterial>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<editor::DefaultMaterial>(entity);

	ImGui::ColorEdit3("Color", &material.baseColor.r);
	ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");
}

template <>
void componentInput<editor::EditorCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::EditorCamera>(entity);

	ImGui::DragFloat("Velocity", &data.velocity);
	ImGui::DragFloat("Sensitivity", &data.sensitivity);
}

template <>
void componentInput<editor::SkyBoxMaterial>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::SkyBoxMaterial>(entity);

	ImGui::Checkbox("Show Sun", &data.showSun);
	ImGui::DragFloat("Color Alpha", &data.color.a, .01f, .0f, 1.0f);
	ImGui::ColorEdit4("Color", &data.color.r);
	ui::inputPath("Cubemap Texture", data.skybox.relativePath);
}

template <>
void componentInput<editor::GridMaterial>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::GridMaterial>(entity);

	ImGui::ColorEdit3("Color", &data.color.r);

	ImGui::DragFloat("Thickness", &data.thickness, 0.001f, .0f, 1.0f);
	ImGui::DragFloat2("Scale", &data.scale.x, 0.01f, .0f, 100.0f);
}

bool EntityProperties::displayEntityCoreComponents(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	bool changed = false;

	changed |= displayComponent<ecs::Transform>("Transform", registry, selectedEntity);

	changed |= displayComponent<ecs::PerspectiveCamera>("Perspective Camera", registry, selectedEntity);

	changed |= displayComponent<ecs::OrthographicCamera>("Orthographic Camera", registry, selectedEntity);

	changed |= displayComponent<ecs::CustomCamera>("Custom Camera", registry, selectedEntity);

	changed |= displayComponent<ecs::DirectionalLight>("Directional Light", registry, selectedEntity);

	changed |= displayComponent<ecs::SpotLight>("Spot Light", registry, selectedEntity);

	changed |= displayComponent<ecs::PointLight>("Point Light", registry, selectedEntity);

	changed |= displayComponent<ecs::Mesh>("Mesh", registry, selectedEntity);

	changed |= displayComponent<ecs::MeshMaterial>("Mesh Material", registry, selectedEntity);

	changed |= displayComponent<ecs::IndirectLight>("Indirect Light", registry, selectedEntity);

	changed |= displayComponent<ecs::SceneView>("Scene View", registry, selectedEntity);

	return changed;
}

bool EntityProperties::displayEntityEditorComponents(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	bool changed = false;

	changed |= displayComponent<editor::EditorCamera>("Editor Camera", registry, selectedEntity);
	changed |= displayComponent<editor::DefaultMaterial>("Default Material", registry, selectedEntity);
	changed |= displayComponent<editor::SkyBoxMaterial>("SkyBox Material", registry, selectedEntity);
	changed |= displayComponent<editor::GridMaterial>("Grid Material", registry, selectedEntity);

	return changed;
}

bool EntityProperties::displayComponents(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	bool isValid = registry.isValid(selectedEntity);
	if (!isValid)
	{
		ImGui::Text("No entity selected.");
		return false;
	}

	bool changed = false;
	changed |= displayEntityName(registry, selectedEntity);
	changed |= displayEntityCoreComponents(registry, selectedEntity);
	changed |= displayEntityEditorComponents(registry, selectedEntity);

	return changed;
}

bool EntityProperties::displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	bool changed = false;

	if (registry.hasAllComponents<ecs::Name>(selectedEntity))
	{
		auto& name = registry.getComponent<ecs::Name>(selectedEntity);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		changed = inputText("##Name", name.name);
	}

	return changed;
}

void EntityProperties::addComponentMenu(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	auto menu = ui::Menu{"Add Component"};
	if (!menu.isOpen())
		return;

	if (!registry.hasAnyComponent<ecs::Transform>(selectedEntity) && menu.item("Transform"))
		registry.addComponent<ecs::Transform>(selectedEntity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::Mesh>(selectedEntity) && menu.item("Mesh"))
		registry.addComponent<ecs::Mesh>(selectedEntity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::DirectionalLight>(selectedEntity) && menu.item("Directional Light"))
		registry.addComponent<ecs::DirectionalLight>(selectedEntity);

	if (!registry.hasAnyComponent<ecs::SpotLight>(selectedEntity) && menu.item("Spot Light"))
		registry.addComponent<ecs::SpotLight>(selectedEntity);

	if (!registry.hasAnyComponent<ecs::PointLight>(selectedEntity) && menu.item("Point Light"))
		registry.addComponent<ecs::PointLight>(selectedEntity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::PerspectiveCamera>(selectedEntity) && menu.item("Perspective Camera"))
		registry.addComponent<ecs::PerspectiveCamera>(selectedEntity);

	if (!registry.hasAnyComponent<ecs::OrthographicCamera>(selectedEntity) && menu.item("Orthographic Camera"))
		registry.addComponent<ecs::OrthographicCamera>(selectedEntity);

	if (!registry.hasAnyComponent<ecs::CustomCamera>(selectedEntity) && menu.item("Custom Camera"))
		registry.addComponent<ecs::CustomCamera>(selectedEntity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::SceneView>(selectedEntity) && menu.item("View"))
		registry.addComponent<ecs::SceneView>(selectedEntity);
}

void EntityProperties::popup(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	if (!registry.isValid(selectedEntity))
		return;

	{
		auto popup = ui::Popup{"Properties Popup"};
		if (popup.isOpen())
		{
			EntityProperties::addComponentMenu(registry, selectedEntity);
		}
	}
}

NewSceneModal::NewSceneModal() : mModal{"New Scene"}
{
	if (!mModal.isOpen())
		return;

	ImGui::Text("Are you sure? If you continue, all unsaved changes will be discarded.");

	ImGui::Separator();

	mIsConfirmed = ImGui::Button("Discard unsaved changes and create a new scene.");
	if (mIsConfirmed)
		mModal.close();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		mModal.close();
}

bool NewSceneModal::onConfirm()
{
	return mModal.isOpen() && mIsConfirmed;
}

void NewSceneModal::open()
{
	ImGui::OpenPopup("New Scene");
}

OpenSceneModal::OpenSceneModal(std::filesystem::path& openPath) : mModal{"Open Scene"}
{
	if (!mModal.isOpen())
		return;

	ImGui::Text("Are you sure? If you continue, all unsaved changes will be discarded.");

	auto openPathValue = openPath.string();
	ui::inputText("Path", openPathValue);
	openPath = std::filesystem::path{openPathValue};

	ImGui::Separator();

	mIsConfirmed = ImGui::Button("Open");
	if (mIsConfirmed)
		mModal.close();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		mModal.close();
}

bool OpenSceneModal::onConfirm()
{
	return mModal.isOpen() && mIsConfirmed;
}

void OpenSceneModal::open()
{
	ImGui::OpenPopup("Open Scene");
}

SaveSceneModal::SaveSceneModal(std::filesystem::path& savePath) : mModal{"Save Scene"}
{
	if (!mModal.isOpen())
		return;

	auto savePathValue = savePath.string();
	ui::inputText("Scene File Path", savePathValue);
	savePath = std::filesystem::path{savePathValue};

	ImGui::Separator();

	mConfirmed = ImGui::Button("Save");
	if (mConfirmed)
		mModal.close();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		mModal.close();
}

bool SaveSceneModal::onConfirm()
{
	return mModal.isOpen() && mConfirmed;
}

void SaveSceneModal::open()
{
	ImGui::OpenPopup("Save Scene");
}

OpenProjectModal::OpenProjectModal(std::filesystem::path& openPath) : mModal{"Open Project"}
{
	if (!mModal.isOpen())
		return;

	auto openPathValue = openPath.string();
	ui::inputText("Path", openPathValue);
	openPath = std::filesystem::path{openPathValue};

	ImGui::Separator();

	mIsConfirmed = ImGui::Button("Open");
	if (mIsConfirmed)
		mModal.close();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		mModal.close();
}

bool OpenProjectModal::onConfirm()
{
	return mModal.isOpen() && mIsConfirmed;
}

void OpenProjectModal::open()
{
	ImGui::OpenPopup("Open Project");
}

bool SceneTree::displayTree(const ecs::Registry& registry, ecs::Entity& selectedEntity, bool showDebugEntities,
							std::string_view search)
{
	using namespace boost::algorithm;

	bool changed = false;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
	{
		selectedEntity = ecs::NullEntity;
		changed = true;
	}

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("Scene Graph Entities", 3, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 12.0f);
		ImGui::TableSetupColumn("Visibility", ImGuiTableColumnFlags_WidthFixed, 18.0f);
		ImGui::TableHeadersRow();

		auto lowerCaseSearch = std::string{search};
		to_lower(lowerCaseSearch);

		const auto onEachNodeFn = [&](ecs::Entity entity, const auto& name) {
			if (!contains(to_lower_copy(name.name), lowerCaseSearch))
				return;

			changed |= displayNode(registry, entity, selectedEntity);
		};

		if (showDebugEntities)
			registry
				.getEntities<const ecs::Name>(
					ecs::ExcludeComponents<ecs::tags::IsMaterial, ecs::Child>)
				.each(onEachNodeFn);
		else
			registry
				.getEntities<const ecs::Name>(
					ecs::ExcludeComponents<ecs::tags::IsMaterial, editor::tags::IsEditorEntity,
										   ecs::Child>)
				.each(onEachNodeFn);

		ImGui::EndTable();
	}

	return changed;
}

bool SceneTree::displayNode(const ecs::Registry& registry, ecs::Entity entity, ecs::Entity& selectedEntity)
{
	const auto& name = registry.getComponent<ecs::Name>(entity);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	const auto* relation = registry.tryGetComponent<ecs::Parent>(entity);
	const auto hasChildren = relation && relation->childrenCount > 0;

	const auto selectedFlags = selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
	const auto childrenFlags = hasChildren
								   ? ImGuiTreeNodeFlags_SpanFullWidth
								   : ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet
										 | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth;

	bool open = ImGui::TreeNodeEx(name.c_str(), childrenFlags | selectedFlags);
	if (ImGui::IsItemClicked())
		selectedEntity = entity;

	ImGui::TableNextColumn();
	ImGui::TextDisabled(registry.hasAnyComponent<editor::tags::IsEditorEntity>(entity) ? "EDITOR" : "--");
	ImGui::TableNextColumn();
	ImGui::TextDisabled("VISIBLE");

	if (open && hasChildren)
	{
		ecs::Parent::forEachChild(registry, entity,
								  [&](auto child) { SceneTree::displayNode(registry, child, selectedEntity); });

		ImGui::TreePop();
	}

	return open;
}

bool MaterialsManager::list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
							bool showEditorEntities)
{
	using namespace boost::algorithm;

	bool changed = false;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("Scene Graph Entities", 1, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableHeadersRow();

		auto lowerCaseSearch = std::string{search};
		to_lower(lowerCaseSearch);

		const auto actionFn = [&](ecs::Entity entity, const auto& name) {
			if (!contains(to_lower_copy(name.name), lowerCaseSearch))
				return;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			const auto selectedFlags = selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
												| ImGuiTreeNodeFlags_SpanFullWidth | selectedFlags);
			if (ImGui::IsItemClicked())
			{
				selectedEntity = entity;
				changed |= true;
			}
		};

		if (showEditorEntities)
		{
			registry.getEntities<const ecs::Name, const ecs::tags::IsMaterial>().each(actionFn);
		}
		else
		{
			registry
				.getEntities<const ecs::Name, const ecs::tags::IsMaterial>(
					ecs::ExcludeComponents<editor::tags::IsEditorEntity>)
				.each(actionFn);
		}

		ImGui::EndTable();
	}

	return changed;
}

bool MaterialsManager::popup(ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	static auto newMaterialsCount = 1;

	bool changed = false;

	auto popup = ui::Popup{"Materials Window Popup"};
	if (popup.isOpen())
	{
		{
			auto menu = ui::Menu{"Create Material"};

			if (menu.item("Default Material"))
			{
				selectedEntity = ecs::build(registry)
									 .withName(fmt::format("Default Material {0}", newMaterialsCount++))
									 .asMaterial<editor::DefaultMaterial>();
				changed = true;
			}
		}

		{
			auto menu = ui::Menu{"Create SkyBox"};

			if (menu.item("Default SkyBox"))
			{
				selectedEntity = ecs::build(registry)
									 .withName("Default SkyBox")
									 .asMaterial<editor::SkyBoxMaterial>();
				changed = true;
			}
		}
	}

	return changed;
}

bool EditorDragAndDrop::loadScene(std::filesystem::path& scenePath, ecs::Entity& selectedEntity)
{
	auto dnd = DragAndDropTarget{};
	const auto result = dnd.getPathPayload(AssetsExplorer::DND_SELECTED_FILE);
	if (result && boost::algorithm::ends_with(result->filename().c_str(), ".spatial.json"))
	{
		selectedEntity = ecs::NullEntity;
		scenePath = result.value();
		return true;
	}

	return false;
}

bool EditorDragAndDrop::loadMesh(ecs::Registry& registry, ecs::Entity& selectedEntity,
								 math::float3 createEntityPosition)
{
	auto dnd = DragAndDropTarget{};
	auto result = dnd.getPathPayload(AssetsExplorer::DND_SELECTED_FILE);

	if (result && boost::algorithm::ends_with(result->filename().c_str(), ".filamesh"))
	{
		selectedEntity = ecs::build(registry)
							 .withName(result->stem().string())
							 .asTransform()
							 .withPosition(createEntityPosition)
							 .asMesh()
							 .withPath(result.value());

		return true;
	}

	return false;
}

bool SceneOptionsMenu::createEntitiesMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
										  math::float3 createEntitiesPosition, bool addAsChild)
{
	auto menu = Menu{"Create Entity"};
	if (!menu.isOpen())
		return false;

	bool changed = false;
	ecs::Entity newEntity = ecs::NullEntity;

	if (menu.item("Empty"))
	{
		newEntity = ecs::build(registry).withName("Empty Entity").with<ecs::tags::IsRenderable>();
		changed = true;
	}

	ImGui::Separator();

	if (menu.item("Cube"))
	{
		newEntity = ecs::build(registry)
						.withName("Cube")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMesh()
						.withPath("editor://meshes/cube.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Plane"))
	{
		newEntity = ecs::build(registry)
						.withName("Plane")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMesh()
						.withPath("editor://meshes/plane.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Sphere"))
	{
		newEntity = ecs::build(registry)
						.withName("Sphere")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMesh()
						.withPath("editor://meshes/sphere.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Cylinder"))
	{
		newEntity = ecs::build(registry)
						.withName("Cylinder")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMesh()
						.withPath("editor://meshes/cylinder.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	ImGui::Separator();

	if (menu.item("Point Light"))
	{
		newEntity = ecs::build(registry)
						.withName("Point Light")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asPointLight();
		changed = true;
	}

	if (menu.item("Directional Light"))
	{
		newEntity = ecs::build(registry).withName("Directional Light").asDirectionalLight();
		changed = true;
	}

	if (menu.item("Spot Light"))
	{
		newEntity = ecs::build(registry)
						.withName("Spot Light")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asSpotLight();
		changed = true;
	}

	ImGui::Separator();

	if (menu.item("Perspective Camera"))
	{
		newEntity = ecs::build(registry)
						.withName("Perspective Camera")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asPerspectiveCamera();
		changed = true;
	}

	if (menu.item("Orthographic Camera"))
	{
		newEntity = ecs::build(registry)
						.withName("Orthographic Camera")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asOrthographicCamera();
		changed = true;
	}

	ImGui::Separator();

	if (menu.item("Scene View"))
	{
		newEntity = ecs::build(registry)
						.withName("Scene View")
						.asSceneView();
		changed = true;
	}

	if (registry.isValid(newEntity))
	{
		if (registry.isValid(selectedEntity) && addAsChild)
			ecs::Parent::addChild(registry, selectedEntity, newEntity);

		selectedEntity = newEntity;
	}

	return changed;
}

bool SceneOptionsMenu::addChildMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
									math::float3 createEntitiesPosition)
{
	if (!registry.isValid(selectedEntity))
		return false;

	auto* name = registry.tryGetComponent<ecs::Name>(selectedEntity);
	auto menu = Menu{name ? fmt::format("Add Child to \"{}\"", name->c_str()) : "Add Child"};

	if (!menu.isOpen())
		return false;
	return SceneOptionsMenu::createEntitiesMenu(registry, selectedEntity, createEntitiesPosition, true);
}

bool SceneOptionsMenu::removeMenu(ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	if (!registry.isValid(selectedEntity))
		return false;

	auto* name = registry.tryGetComponent<ecs::Name>(selectedEntity);
	bool changed = false;
	if (Menu::itemButton(name ? fmt::format("Remove \"{}\"", name->c_str()) : "Remove Entity"))
	{
		if (registry.hasAnyComponent<ecs::Child>(selectedEntity))
			ecs::Child::remove(registry, selectedEntity); // TODO: Should dispatch a job

		if (registry.hasAnyComponent<ecs::Parent>(selectedEntity))
			ecs::Parent::destroyChildren(registry, selectedEntity); // TODO: Should dispatch a job

		registry.destroy(selectedEntity); // TODO: Should dispatch a job
		selectedEntity = ecs::NullEntity;
		changed = true;
	}

	return changed;
}

bool EditorMainMenu::fileMenu()
{
	bool changed = false;
	auto action = FileMenuAction::Unknown;

	{
		auto menu = ui::Menu{"File"};
		if (menu.item("Open Project", "CTRL+SHIFT+O"))
			action = FileMenuAction::OpenProject;

		if (menu.item("New Scene", "CTRL+N"))
			action = FileMenuAction::NewScene;

		if (menu.item("Open Scene", "CTRL+O"))
			action = FileMenuAction::OpenScene;

		if (menu.item("Save Scene", "CTRL+S"))
			action = FileMenuAction::SaveScene;
	}

	if (action != FileMenuAction::Unknown)
	{
		const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		const ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(size.x / 2, size.y / 2));
		changed = true;
	}

	switch (action)
	{
	case FileMenuAction::OpenProject:
		ui::OpenProjectModal::open();
		break;
	case FileMenuAction::SaveScene:
		ui::SaveSceneModal::open();
		break;
	case FileMenuAction::OpenScene:
		ui::OpenSceneModal::open();
		break;
	case FileMenuAction::NewScene:
		ui::NewSceneModal::open();
		break;
	case FileMenuAction::Unknown:
		break;
	}

	static std::filesystem::path currentProjectFolder = "";
	static std::filesystem::path currentScenePath = "";

	return changed;
}

bool EditorMainMenu::viewOptionsMenu(bool& isEditorEntitiesShowing)
{
	auto menu = Menu{"View"};
	if (!menu.isOpen())
		return false;

	bool changed = false;

	if (menu.item("Toggle Debug Entities"))
	{
		isEditorEntitiesShowing = !isEditorEntitiesShowing;
		changed = true;
	}

	return changed;
}

} // namespace spatial::ui