#include "CustomComponents.h"
#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "Tags.h"
#include <boost/algorithm/string/predicate.hpp>
#include <imgui.h>
#include <spatial/ecs/Relation.h>
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

PropertiesPanel::PropertiesPanel(ecs::Registry& registry, ecs::Entity selectedEntity)
	: mWindow{"Properties"}, mRegistry{registry}, mSelectedEntity{selectedEntity}
{
	if (!mWindow.isOpen())
		return;

	bool isValid = registry.isValid(selectedEntity);
	if (!isValid)
	{
		ImGui::Text("No entity selected.");
		return;
	}

	if (registry.hasAllComponents<ecs::EntityName>(selectedEntity))
	{
		auto& name = registry.getComponent<ecs::EntityName>(selectedEntity);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		inputText("##Name", name.name);
	}

	if (onComponent<ecs::Transform>("Transform"))
		componentInput<ecs::Transform>(registry, selectedEntity);

	if (onComponent<ecs::PerspectiveCamera>("Perspective Camera"))
		componentInput<ecs::PerspectiveCamera>(registry, selectedEntity);

	if (onComponent<ecs::OrthographicCamera>("Orthographic Camera"))
		componentInput<ecs::OrthographicCamera>(registry, selectedEntity);

	if (onComponent<ecs::CustomCamera>("Custom Camera"))
		componentInput<ecs::CustomCamera>(registry, selectedEntity);

	if (onComponent<ecs::DirectionalLight>("Directional Light"))
		componentInput<ecs::DirectionalLight>(registry, selectedEntity);

	if (onComponent<ecs::SpotLight>("Spot Light"))
		componentInput<ecs::SpotLight>(registry, selectedEntity);

	if (onComponent<ecs::PointLight>("Point Light"))
		componentInput<ecs::PointLight>(registry, selectedEntity);

	if (onComponent<ecs::Mesh>("Mesh"))
		componentInput<ecs::Mesh>(registry, selectedEntity);

	if (onComponent<editor::EditorCamera>("Editor Camera"))
		componentInput<editor::EditorCamera>(mRegistry, selectedEntity);

	if (onComponent<editor::DefaultMaterial>("Default Material"))
		componentInput<editor::DefaultMaterial>(mRegistry, selectedEntity);
}

EditorMainMenu::EditorMainMenu(std::filesystem::path& rootPath, std::filesystem::path& scenePath) : mMenuBar{}
{
	auto openAction = MainMenuOption::Unknown;

	{
		auto menu = ui::Menu{"File"};
		if (menu.item("Open Project", "CTRL+SHIFT+O"))
			openAction = MainMenuOption::OpenProject;

		if (menu.item("New Scene", "CTRL+N"))
			openAction = MainMenuOption::NewScene;

		if (menu.item("Open Scene", "CTRL+O"))
			openAction = MainMenuOption::OpenScene;

		if (menu.item("Save Scene", "CTRL+S"))
			openAction = MainMenuOption::SaveScene;
	}

	if (openAction != MainMenuOption::Unknown)
	{
		const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		const ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(size.x / 2, size.y / 2));
	}

	switch (openAction)
	{
	case MainMenuOption::OpenProject:
		ui::OpenProjectModal::open();
		break;
	case MainMenuOption::SaveScene:
		ui::SaveSceneModal::open();
		break;
	case MainMenuOption::OpenScene:
		ui::OpenSceneModal::open();
		break;
	case MainMenuOption::NewScene:
		ui::NewSceneModal::open();
		break;
	case MainMenuOption::Unknown:
		break;
	}

	mSelectedOption = MainMenuOption::Unknown;

	{
		auto modal = ui::OpenProjectModal{rootPath};
		if (modal.onConfirm())
			mSelectedOption = MainMenuOption::OpenProject;
	}

	{
		auto modal = ui::SaveSceneModal{scenePath};
		if (modal.onConfirm())
			mSelectedOption = MainMenuOption::SaveScene;
	}

	{
		auto modal = ui::OpenSceneModal{scenePath};
		if (modal.onConfirm())
			mSelectedOption = MainMenuOption::OpenScene;
	}

	{
		auto modal = ui::NewSceneModal{};
		if (modal.onConfirm())
			mSelectedOption = MainMenuOption::NewScene;
	}
}

bool EditorMainMenu::onOpenProject()
{
	return mSelectedOption == MainMenuOption::OpenProject;
}

bool EditorMainMenu::onNewScene()
{
	return mSelectedOption == MainMenuOption::NewScene;
}

bool EditorMainMenu::onOpenScene()
{
	return mSelectedOption == MainMenuOption::OpenScene;
}

bool EditorMainMenu::onSaveScene()
{
	return mSelectedOption == MainMenuOption::SaveScene;
}

NewSceneModal::NewSceneModal() : mModal{"New Scene"}
{
	if (!mModal.isOpen()) return;

	ImGui::Text("Are you sure? If you continue, all unsaved changes will be discarded.");

	ImGui::Separator();

	mIsConfirmed = ImGui::Button("Discard unsaved changes and create a new scene.");
	if (mIsConfirmed)
	{
		mModal.close();
	}

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

SceneGraphWindow::SceneGraphWindow(bool& showDebugEntities)
	: mWindow{"Scene Graph"}, mShowEditorEntities{showDebugEntities}
{
}

bool SceneGraphWindow::header(ecs::Registry& registry, std::string& search)
{
	if (!mWindow.isOpen())
		return false;

	{
		static auto newEntitiesCount = 1;

		auto popup = ui::Popup{"Scene Graph Popup"};
		if (popup.isOpen())
		{
			{
				auto menu = ui::Menu{"Create Entity"};
				if (menu.item("Empty"))
					ecs::build(registry).withName(fmt::format("Entity {0}", newEntitiesCount++));

				ImGui::Separator();

				if (menu.item("Cube"))
					ecs::build(registry)
						.withName("Cube")
						.asTransform()
						.asMesh("editor://meshes/cube.filamesh")
						.withSubMesh(0, 1);

				if (menu.item("Plane"))
					ecs::build(registry)
						.withName("Plane")
						.asTransform()
						.asMesh("editor://meshes/plane.filamesh")
						.withSubMesh(0, 1);

				if (menu.item("Sphere"))
					ecs::build(registry)
						.withName("Sphere")
						.asTransform()
						.asMesh("editor://meshes/sphere.filamesh")
						.withSubMesh(0, 1);

				if (menu.item("Cylinder"))
					ecs::build(registry)
						.withName("Cylinder")
						.asTransform()
						.asMesh("editor://meshes/sphere.filamesh")
						.withSubMesh(0, 1);

				ImGui::Separator();

				if (menu.item("Point Light"))
					ecs::build(registry).withName("Point Light").asTransform().asPointLight();

				if (menu.item("Directional Light"))
					ecs::build(registry).withName("Directional Light").asDirectionalLight();

				if (menu.item("Spot Light"))
					ecs::build(registry).withName("Spot Light").asTransform().asSpotLight();

				ImGui::Separator();

				if (menu.item("Perspective Camera"))
					ecs::build(registry)
						.withName(fmt::format("Perspective Camera {0}", newEntitiesCount++))
						.asPerspectiveCamera();

				if (menu.item("Orthographic Camera"))
					ecs::build(registry)
						.withName(fmt::format("Orthographic Camera {0}", newEntitiesCount++))
						.asOrthographicCamera();
			}

			{
				auto menu = ui::Menu{"View"};
				if (menu.item("Toggle Debug Entities"))
					mShowEditorEntities = !mShowEditorEntities;
			}
		}
	}

	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	bool changed = ui::inputText("Search: ", search);

	return changed;
}

bool SceneGraphWindow::listTree(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search)
{
	if (!mWindow.isOpen())
		return false;

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

		const auto onEachNodeFn = [&](ecs::Entity entity, const auto& name) {
			if (!boost::algorithm::contains(name.name, search))
				return;

			changed |= displayNode(registry, entity, selectedEntity);
		};

		if (mShowEditorEntities)
			registry.getEntities<const ecs::EntityName>(ecs::ExcludeComponents<ecs::tags::IsMaterial>)
				.each(onEachNodeFn);
		else
			registry
				.getEntities<const ecs::EntityName>(
					ecs::ExcludeComponents<ecs::tags::IsMaterial, editor::tags::IsEditorEntity>)
				.each(onEachNodeFn);

		ImGui::EndTable();
	}

	return changed;
}

bool SceneGraphWindow::displayNode(const ecs::Registry& registry, ecs::Entity entity, ecs::Entity& selectedEntity)
{
	const auto& name = registry.getComponent<ecs::EntityName>(entity);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	const auto* relation = registry.getComponentIfExists<ecs::Relation>(entity);
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
		for (auto child = relation->first; registry.isValid(child);
			 child = registry.getComponent<const ecs::Relation>(child).next)
			displayNode(registry, child, selectedEntity);

		ImGui::TreePop();
	}

	return open;
}

MaterialsWindow::MaterialsWindow() : mWindow{"Materials"}
{
}

bool MaterialsWindow::header(ecs::Registry& registry, std::string& search)
{
	if (!mWindow.isOpen())
		return false;

	{
		static auto newMaterialsCount = 1;

		auto popup = ui::Popup{"Materials Window Popup"};
		if (popup.isOpen())
		{
			{
				auto menu = ui::Menu{"Create Material"};

				if (menu.item("Default Material"))
					ecs::build(registry)
						.withName(fmt::format("Default Material {0}", newMaterialsCount++))
						.asMaterial<editor::DefaultMaterial>();
			}
		}
	}

	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	bool changed = ui::inputText("Search: ", search);

	return changed;
}

bool MaterialsWindow::list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search)
{
	if (!mWindow.isOpen())
		return false;

	bool changed = false;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("Scene Graph Entities", 1, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableHeadersRow();

		registry.getEntities<const ecs::EntityName, const ecs::tags::IsMaterial>().each([&](ecs::Entity entity,
																							const auto& name) {
			if (!boost::algorithm::contains(name.c_str(), search))
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
		});

		ImGui::EndTable();
	}

	return changed;
}

} // namespace spatial::ui