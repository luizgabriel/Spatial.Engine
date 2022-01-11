#include "CustomComponents.h"
#include "EditorCamera.h"
#include "Materials.h"
#include "Tags.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/Relation.h>
#include <spatial/render/TextureView.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/MaterialInputs.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/Popup.h>

namespace spatial::ui
{

void ComponentInputImpl<editor::ColorMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<editor::ColorMaterial>(entity);

	ImGui::ColorEdit3("Color", &material.baseColor.r);
	ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");
}

void ComponentInputImpl<editor::EditorCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::EditorCamera>(entity);

	ImGui::DragFloat("Velocity", &data.velocity);
	ImGui::DragFloat("Sensitivity", &data.sensitivity);
}

void ComponentInputImpl<editor::GridMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::GridMaterial>(entity);

	ImGui::ColorEdit3("Color", &data.color.r);
	ImGui::DragFloat("Thickness", &data.thickness, 0.001f, .0f, 1.0f);
	ImGui::DragFloat2("Scale", &data.scale.x, 0.01f, .0f, 100.0f);
}

void ComponentInputImpl<editor::SkyBoxMaterial, const filament::Texture&>::draw(ecs::Registry& registry,
																				ecs::Entity entity,
																				const filament::Texture& icons)
{

	auto& data = registry.getComponent<editor::SkyBoxMaterial>(entity);

	ui::cubemapInput("Cubemap", data.color, data.skybox, icons);
	ImGui::Checkbox("Show Sun", &data.showSun);
}

void ComponentInputImpl<editor::StandardOpaqueMaterial, const filament::Texture&,
						const render::ImageTextureFinder&>::draw(ecs::Registry& registry, ecs::Entity entity,
																 const filament::Texture& icons,
																 const render::ImageTextureFinder& finder)

{
	auto& data = registry.getComponent<editor::StandardOpaqueMaterial>(entity);
	ui::albedoInput("Albedo", data.baseColor, data.albedo, finder(data.albedo), icons);
	ui::inputHelp("The Albedo Property", "Defines the perceived color of an object."
										 "The effect depends on the nature of the surface,"
										 "controlled by the metallic property.");

	ui::separator(2);

	ImGui::DragFloat2("Tiling", &data.tiling.x, .01f, .01f);
	ImGui::DragFloat2("Offset", &data.offset.x, .01f);

	ui::separator(1);

	ui::mapInput("Metallic", data.metallic, data.metallicMap, finder(data.metallicMap), icons);
	ui::inputHelp(
		"The Metallic Property",
		"The metallic property defines whether the surface is a metallic (conductor) or a non-metallic (dielectric) "
		"surface. This property should be used as a binary value, set to either 0 or 1. Intermediate values are only "
		"truly useful to create transitions between different types of surfaces when using textures.");

	ui::separator(1);

	ui::mapInput("Roughness", data.roughness, data.roughnessMap, finder(data.roughnessMap), icons);
	ui::inputHelp("The Roughness Property",
				  "The roughness property controls the perceived smoothness of the surface. When roughness is set to "
				  "0, the surface is perfectly smooth and highly glossy. The rougher a surface is, the “blurrier” the "
				  "reflections are. This property is often called glossiness in other engines and tools, and is simply "
				  "the opposite of the roughness (roughness = 1 - glossiness).");

	ui::separator(1);

	ui::mapInput("Reflectance", data.reflectance, data.reflectanceMap, finder(data.reflectanceMap), icons);

	ui::separator(1);

	ui::mapInput("Ambient Occlusion", data.ambientOcclusionMap, finder(data.ambientOcclusionMap), icons);

	ui::separator(1);

	ui::mapInput("Normal", data.normalMap, finder(data.normalMap), icons, Icons::normalMap.uv());

	ui::separator(1);

	ui::mapInput("Bent Normals", data.bentNormalMap, finder(data.bentNormalMap), icons, Icons::normalMap.uv());

	ui::separator(1);

	ui::mapInput("Height Map", data.height, data.heightMap, finder(data.heightMap), icons);
}

bool EntityProperties::displayComponents(ecs::Registry& registry, ecs::Entity entity, const filament::Texture& icons,
										 const render::ImageTextureFinder& finder)
{
	bool isValid = registry.isValid(entity);
	if (!isValid)
	{
		ImGui::Text("No entity selected.");
		return false;
	}

	bool changed = false;
	displayEntityName(registry, entity);

	componentCollapse<ecs::Transform>(registry, entity);
	componentCollapse<ecs::PerspectiveCamera>(registry, entity);
	componentCollapse<ecs::OrthographicCamera>(registry, entity);
	componentCollapse<ecs::CustomCamera>(registry, entity);
	componentCollapse<ecs::DirectionalLight>(registry, entity);
	componentCollapse<ecs::SpotLight>(registry, entity);
	componentCollapse<ecs::PointLight>(registry, entity);
	componentCollapse<ecs::IndirectLight>(registry, entity);
	componentCollapse<ecs::Mesh>(registry, entity);
	componentCollapse<ecs::MeshMaterial>(registry, entity);
	componentCollapse<ecs::SceneView>(registry, entity);

	componentCollapse<editor::EditorCamera>(registry, entity);
	componentCollapse<editor::ColorMaterial>(registry, entity);
	componentCollapse<editor::SkyBoxMaterial>(registry, entity, icons);
	componentCollapse<editor::GridMaterial>(registry, entity);
	componentCollapse<editor::StandardOpaqueMaterial>(registry, entity, icons, finder);

	return changed;
}

void EntityProperties::displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	bool changed = false;

	if (registry.hasAllComponents<ecs::Name>(selectedEntity))
	{
		auto& name = registry.getComponent<ecs::Name>(selectedEntity);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		changed = inputText("##Name", name.name);
	}
}

void EntityProperties::addComponentMenu(ecs::Registry& registry, ecs::Entity entity)
{
	auto menu = ui::Menu{"Add Component"};
	if (!menu.isOpen())
		return;

	if (!registry.hasAnyComponent<ecs::Transform>(entity) && menu.item("Transform"))
		registry.addComponent<ecs::Transform>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::Mesh>(entity) && menu.item("Mesh"))
		registry.addComponent<ecs::Mesh>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::DirectionalLight>(entity) && menu.item("Directional Light"))
		registry.addComponent<ecs::DirectionalLight>(entity);

	if (!registry.hasAnyComponent<ecs::SpotLight>(entity) && menu.item("Spot Light"))
		registry.addComponent<ecs::SpotLight>(entity);

	if (!registry.hasAnyComponent<ecs::PointLight>(entity) && menu.item("Point Light"))
		registry.addComponent<ecs::PointLight>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::PerspectiveCamera>(entity) && menu.item("Perspective Camera"))
		registry.addComponent<ecs::PerspectiveCamera>(entity);

	if (!registry.hasAnyComponent<ecs::OrthographicCamera>(entity) && menu.item("Orthographic Camera"))
		registry.addComponent<ecs::OrthographicCamera>(entity);

	if (!registry.hasAnyComponent<ecs::CustomCamera>(entity) && menu.item("Custom Camera"))
		registry.addComponent<ecs::CustomCamera>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::SceneView>(entity) && menu.item("Scene View"))
		registry.addComponent<ecs::SceneView>(entity);
}

void EntityProperties::popup(ecs::Registry& registry, ecs::Entity entity)
{
	if (!registry.isValid(entity))
		return;

	if (registry.hasAnyComponent<ecs::tags::IsMaterial>(entity))
		return;

	{
		auto popup = ui::Popup{"Properties Popup"};
		if (popup.isOpen())
			EntityProperties::addComponentMenu(registry, entity);
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
			registry.getEntities<const ecs::Name>(ecs::ExcludeComponents<ecs::tags::IsMaterial, ecs::Child>)
				.each(onEachNodeFn);
		else
			registry
				.getEntities<const ecs::Name>(
					ecs::ExcludeComponents<ecs::tags::IsMaterial, editor::tags::IsEditorEntity, ecs::Child>)
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
			registry.getEntities<const ecs::Name, const ecs::tags::IsMaterial>().each(actionFn);
		else
			registry
				.getEntities<const ecs::Name, const ecs::tags::IsMaterial>(
					ecs::ExcludeComponents<editor::tags::IsEditorEntity>)
				.each(actionFn);

		ImGui::EndTable();
	}

	return changed;
}

bool MaterialsManager::popup(ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	bool changed = false;

	auto popup = ui::Popup{"Materials Window Popup"};
	ecs::Entity createdEntity = ecs::NullEntity;

	if (popup.isOpen())
	{
		{
			auto menu = ui::Menu{"Create Material"};

			if (menu.item("Standard Opaque"))
			{
				createdEntity =
					ecs::build(registry).withName("Standard Opaque").asMaterial<editor::StandardOpaqueMaterial>();
				changed = true;
			}

			if (menu.item("Color Material"))
			{
				createdEntity = ecs::build(registry).withName("Color Material").asMaterial<editor::ColorMaterial>();
				changed = true;
			}
		}

		{
			auto menu = ui::Menu{"Create SkyBox"};

			if (menu.item("Default SkyBox"))
			{
				createdEntity = ecs::build(registry).withName("Default SkyBox").asMaterial<editor::SkyBoxMaterial>();
				changed = true;
			}
		}

		if (registry.hasAllComponents<ecs::Mesh>(selectedEntity) && changed)
		{
			ecs::Mesh::addMaterial(registry, selectedEntity, createdEntity);
		}

		if (changed)
			selectedEntity = createdEntity;
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
		newEntity = ecs::build(registry).withName("Scene View").asSceneView();
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

bool EditorMainMenu::fileMenu(const filament::Texture& icons)
{
	bool changed = false;
	auto action = FileMenuAction::Unknown;

	ImGui::SetCursorPosY(1.5f);
	ui::image(icons, math::float2{20.0f}, Icons::logo.uv());
	ImGui::SetCursorPosY(0.0f);

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