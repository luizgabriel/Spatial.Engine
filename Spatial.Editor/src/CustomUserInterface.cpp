#include "CustomUserInterface.h"
#include "Components.h"
#include "Materials.h"
#include "Tags.h"
#include <boost/algorithm/string/predicate.hpp>
#include <fmt/format.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Icons.h>
#include <spatial/ui/components/MaterialInputs.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/Popup.h>

namespace spatial::ui
{

bool ComponentInputImpl<editor::ColorMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<editor::ColorMaterial>(entity);
	bool changed = false;

	changed |= ImGui::ColorEdit3("Color", &material.baseColor.r);
	changed |= ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	changed |= ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	changed |= ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");

	return changed;
}

bool ComponentInputImpl<editor::EditorCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::EditorCamera>(entity);
	bool changed = false;

	changed |= ImGui::DragFloat("Velocity", &data.velocity);
	changed |= ImGui::DragFloat("Sensitivity", &data.sensitivity);

	return changed;
}

bool ComponentInputImpl<editor::GridMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::GridMaterial>(entity);
	bool changed = false;

	changed |= ImGui::ColorEdit3("Color", &data.color.r);
	changed |= ImGui::DragFloat("Thickness", &data.thickness, 0.001f, .0f, 1.0f);
	changed |= ImGui::DragFloat2("Scale", &data.scale.x, 0.01f, .0f, 100.0f);

	return changed;
}

bool ComponentInputImpl<editor::SkyBoxMaterial, graphics::OptionalTexture>::draw(ecs::Registry& registry,
																				 ecs::Entity entity,
																				 graphics::OptionalTexture icons)
{
	auto& data = registry.getComponent<editor::SkyBoxMaterial>(entity);
	bool changed = false;

	changed |= ui::colorPicker("Background Color", data.color, icons);
	changed |= ui::cubemapInput("Cubemap", registry, data.skybox, icons);
	changed |= ImGui::Checkbox("Show Sun", &data.showSun);

	return changed;
}

bool ComponentInputImpl<editor::StandardOpaqueMaterial, graphics::OptionalTexture>::draw(
	ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons)

{
	auto& data = registry.getComponent<editor::StandardOpaqueMaterial>(entity);
	bool changed = false;

	changed |= ui::albedoInput("Albedo", data.baseColor, registry, data.albedo, icons);

	ui::separator(2);

	changed |= ImGui::DragFloat2("Tiling", &data.tiling.x, .01f, .01f);
	changed |= ImGui::DragFloat2("Offset", &data.offset.x, .01f);

	ui::separator(1);

	changed |= ui::mapInput("Metallic", data.metallic, registry, data.metallicMap, icons);

	ui::separator(1);

	changed |= ui::mapInput("Roughness", data.roughness, registry, data.roughnessMap, icons);

	ui::separator(1);

	changed |= ui::mapInput("Reflectance", data.reflectance, registry, data.reflectanceMap, icons);

	ui::separator(1);

	changed |= ui::mapInput("Ambient Occlusion", registry, data.ambientOcclusionMap, icons);

	ui::separator(1);

	changed |= ui::mapInput("Normal", registry, data.normalMap, icons, Icons::normalMap.uv());

	ui::separator(1);

	changed |= ui::mapInput("Height Map", data.height, registry, data.heightMap, icons);

	ui::separator(1);

	changed |= ui::colorPicker("Emissive", data.emissive, icons);

	return changed;
}

bool EntityProperties::displayComponents(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons,
										 bool showDebugComponents)
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
	componentCollapse<ecs::IndirectLight>(registry, entity, icons);
	componentCollapse<ecs::SunLight>(registry, entity);
	componentCollapse<ecs::Resource>(registry, entity);
	componentCollapse<ecs::MeshInstance>(registry, entity, icons);
	componentCollapse<ecs::MeshMaterial>(registry, entity, icons);
	componentCollapse<ecs::Scene>(registry, entity, icons);
	componentCollapse<ecs::AttachmentTexture>(registry, entity);
	componentCollapse<ecs::tags::IsImageTexture>(registry, entity);

	componentCollapse<editor::EditorCamera>(registry, entity);
	componentCollapse<editor::ColorMaterial>(registry, entity);
	componentCollapse<editor::SkyBoxMaterial>(registry, entity, icons);
	componentCollapse<editor::GridMaterial>(registry, entity);
	componentCollapse<editor::StandardOpaqueMaterial>(registry, entity, icons);

	if (showDebugComponents)
	{
		componentCollapse<ecs::Parent>(registry, entity, icons);
		componentCollapse<ecs::Child>(registry, entity, icons);
	}

	ui::separator(2);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Tags: ");

	componentTag<ecs::tags::IsRenderedToTarget>(registry, entity);
	componentTag<ecs::tags::IsMaterialInstance>(registry, entity);
	componentTag<ecs::tags::IsMainView>(registry, entity);
	componentTag<ecs::tags::IsRenderable>(registry, entity);
	componentTag<ecs::tags::IsMesh>(registry, entity);
	componentTag<ecs::tags::IsIrradianceValues>(registry, entity);
	componentTag<ecs::tags::IsDummyCubeMapTexture>(registry, entity);
	componentTag<ecs::tags::IsMaterial>(registry, entity);
	componentTag<ecs::tags::IsImageTexture>(registry, entity);
	componentTag<ecs::tags::IsCubeMapTexture>(registry, entity);
	componentTag<ecs::tags::IsScript>(registry, entity);
	componentTag<ecs::tags::IsLight>(registry, entity);

	componentTag<editor::tags::IsEditorView>(registry, entity);
	componentTag<editor::tags::IsEditorEntity>(registry, entity);

	return changed;
}

bool EntityProperties::displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	bool changed = false;

	if (registry.hasAllComponents<ecs::Name>(selectedEntity))
	{
		auto& name = registry.getComponent<ecs::Name>(selectedEntity);

		ui::spanToAvailWidth();
		changed = inputText("##Name", name.name);
	}

	return changed;
}

void EntityProperties::addComponentMenu(ecs::Registry& registry, ecs::Entity entity)
{
	auto menu = ui::Menu{"Add Component"};
	if (!menu.isOpen())
		return;

	if (!registry.hasAnyComponent<ecs::Transform>(entity) && menu.item("Transform"))
		registry.addComponent<ecs::Transform>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::MeshInstance>(entity) && menu.item("Mesh Instance"))
		registry.addComponent<ecs::MeshInstance>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::DirectionalLight>(entity) && menu.item("Directional Light"))
		registry.addComponent<ecs::DirectionalLight>(entity);

	if (!registry.hasAnyComponent<ecs::SpotLight>(entity) && menu.item("Spot Light"))
		registry.addComponent<ecs::SpotLight>(entity);

	if (!registry.hasAnyComponent<ecs::PointLight>(entity) && menu.item("Point Light"))
		registry.addComponent<ecs::PointLight>(entity);

	if (!registry.hasAnyComponent<ecs::SunLight>(entity) && menu.item("Sun Light"))
		registry.addComponent<ecs::SunLight>(entity);

	if (!registry.hasAnyComponent<ecs::IndirectLight>(entity) && menu.item("Indirect Light"))
		registry.addComponent<ecs::IndirectLight>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::PerspectiveCamera>(entity) && menu.item("Perspective Camera"))
		registry.addComponent<ecs::PerspectiveCamera>(entity);

	if (!registry.hasAnyComponent<ecs::OrthographicCamera>(entity) && menu.item("Orthographic Camera"))
		registry.addComponent<ecs::OrthographicCamera>(entity);

	if (!registry.hasAnyComponent<ecs::CustomCamera>(entity) && menu.item("Custom Camera"))
		registry.addComponent<ecs::CustomCamera>(entity);

	ImGui::Separator();

	if (!registry.hasAnyComponent<ecs::Scene>(entity) && menu.item("Scene View"))
		registry.addComponent<ecs::Scene>(entity);
}

void EntityProperties::popup(ecs::Registry& registry, ecs::Entity entity)
{
	if (!registry.isValid(entity))
		return;

	if (registry.hasAnyComponent<ecs::tags::IsMaterialInstance>(entity))
		return;

	{
		auto popup = ui::Popup{"Properties Popup"};
		if (popup.isOpen())
			EntityProperties::addComponentMenu(registry, entity);
	}
}

bool EditorModals::newScene()
{
	ImGui::Text("Are you sure?");
	ImGui::Text("If you continue, all unsaved changes will be discarded.");

	ui::separator(2);

	bool isConfirmed = ImGui::Button("Continue");
	if (isConfirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return isConfirmed;
}

bool EditorModals::openScene(std::string& openPath)
{
	ImGui::Text("Are you sure?");
	ImGui::Text("If you continue, all unsaved changes will be discarded.");

	ui::spacing(2);

	ui::inputText("Path", openPath, "scenes/*");

	ui::separator(2);

	bool confirmed = ImGui::Button("Continue");
	if (confirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return confirmed;
}

bool EditorModals::saveScene(std::string& savePath)
{
	ImGui::Text("Save your current scene to this file:");

	ui::spacing(2);

	ui::inputText("Scene File Path", savePath, "scenes/*");

	ui::separator(2);

	bool confirmed = ImGui::Button("Continue");
	if (confirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return confirmed;
}

bool EditorModals::openProject(std::string& openPath)
{
	ImGui::Text("Open you project source in the specified folder:");

	ui::spacing(2);

	ui::inputText("Path", openPath);

	ui::separator(2);

	bool confirmed = ImGui::Button("Continue");
	if (confirmed)
		ImGui::CloseCurrentPopup();

	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		ImGui::CloseCurrentPopup();

	return confirmed;
}

bool SceneTree::displayTree(ecs::Registry& registry, ecs::Entity& selectedEntity, bool showDebugEntities,
							std::string_view search)
{
	using namespace boost::algorithm;

	bool changed = false;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
	{
		selectedEntity = ecs::NullEntity;
		changed = true;
	}

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

			changed |= displayNode(registry, entity, selectedEntity);
		};

		if (showDebugEntities)
			registry.getEntities<const ecs::Name>(ecs::ExcludeComponents<ecs::Resource, ecs::Child>).each(onEachNodeFn);
		else
			registry
				.getEntities<const ecs::Name>(
					ecs::ExcludeComponents<ecs::Resource, editor::tags::IsEditorEntity, ecs::Child>)
				.each(onEachNodeFn);

		ImGui::EndTable();
	}

	return changed;
}

bool SceneTree::displayNode(ecs::Registry& registry, ecs::Entity entity, ecs::Entity& selectedEntity)
{
	const auto& name = registry.getComponent<ecs::Name>(entity);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	const auto* relation = registry.tryGetComponent<ecs::Parent>(entity);
	const auto hasChildren = relation && relation->childrenCount > 0;

	const auto selectedFlags = selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
	const auto childrenFlags =
		hasChildren
			? ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
			: ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
				  | ImGuiTreeNodeFlags_SpanFullWidth;

	bool open = ImGui::TreeNodeEx(name.c_str(), childrenFlags | selectedFlags);
	if (ImGui::IsItemClicked())
		selectedEntity = entity;

	if (auto dnd = ui::DragAndDropSource{}; dnd.isStarted())
		dnd.setPayload(fmt::to_string(static_cast<uint32_t>(entity)));

	if (auto dnd = ui::DragAndDropTarget{}; dnd.isStarted())
	{
		if (auto payload = dnd.getPayload(); payload)
		{
			auto draggedEntity = static_cast<ecs::Entity>(std::stoi(*payload));
			assert(registry.isValid(draggedEntity));
			if (draggedEntity != entity) {
				ecs::Child::changeParent(registry, draggedEntity, entity);
			}
		}
	}

	ImGui::TableNextColumn();

	if (registry.hasAnyComponent<ecs::tags::IsMainView>(entity))
		ImGui::TextDisabled("Main View");
	else if (registry.hasAnyComponent<editor::tags::IsEditorView>(entity))
		ImGui::TextDisabled("Editor View");
	else if (registry.hasAnyComponent<ecs::tags::IsCamera>(entity))
		ImGui::TextDisabled("Camera");
	else if (registry.hasAnyComponent<ecs::tags::IsLight>(entity))
		ImGui::TextDisabled("Light");
	else if (registry.hasAnyComponent<editor::tags::IsEditorEntity>(entity))
		ImGui::TextDisabled("Editor");
	else if (registry.hasAnyComponent<ecs::tags::IsRenderable>(entity))
		ImGui::TextDisabled("Renderable");

	if (open && hasChildren)
	{
		ecs::Parent::forEachChild(registry, entity,
								  [&](auto child) { SceneTree::displayNode(registry, child, selectedEntity); });

		ImGui::TreePop();
	}

	return open;
}

bool ResourceManager::header(std::string& search, ResourceManager::ResourceType& filter,
							 graphics::OptionalTexture icons)
{
	bool changed = false;

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ui::spanToAvailWidth();

	changed |= ui::Combo::fromEnum("##ResourceTypeFilter", filter);

	ImGui::NextColumn();
	changed |= ui::Search::text(search, std::move(icons));

	ImGui::Columns(1);

	return changed;
}

bool ResourceManager::list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
						   ResourceManager::ResourceType type, bool showEditorEntities)
{
	using namespace boost::algorithm;
	using namespace std::string_view_literals;

	bool changed = false;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("AssetsManagerTable", 2, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Type");
		ImGui::TableHeadersRow();

		const auto eachFn = [&](ecs::Entity entity, const auto& resource, const auto& name) {
			if (!showEditorEntities && starts_with(resource.relativePath.c_str(), "editor/"))
				return;

			if (!showEditorEntities && starts_with(resource.relativePath.c_str(), "engine/"))
				return;

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

			const auto selectedFlags = selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen
												| ImGuiTreeNodeFlags_SpanFullWidth | selectedFlags);
			if (ImGui::IsItemClicked())
			{
				selectedEntity = entity;
				changed |= true;
			}

			ui::Popup::show("ResourcePopup", [&]() { ImGui::Text("%s", resource.relativePath.c_str()); });

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

		registry.getEntities<const ecs::Resource, const ecs::Name>().each(eachFn);

		ImGui::EndTable();
	}

	return changed;
}

bool MaterialsManager::createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity)
{
	bool changed = false;
	ecs::Entity createdEntity = ecs::NullEntity;

	const auto isMeshInstanceSelected = registry.hasAllComponents<ecs::MeshInstance>(selectedEntity);
	const auto* selectedMeshInstanceName = registry.tryGetComponent<ecs::Name>(selectedEntity);

	const auto mergeMaterialName = [&](const char* materialName) {
		if (selectedMeshInstanceName && isMeshInstanceSelected)
			return fmt::format("{} ({})", materialName, selectedMeshInstanceName->c_str());
		else
			return std::string{materialName};
	};

	auto menu = ui::Menu("New");
	if (!menu.isOpen())
		return false;

	{
		auto menu2 = ui::Menu{"Material"};

		if (menu2.item("Standard Opaque"))
		{
			createdEntity = ecs::Builder::create(registry)
								.withName(mergeMaterialName("Standard Opaque"))
								.asMaterialInstance<editor::StandardOpaqueMaterial>()
								.withMaterial("editor/materials/standard_lit.filamat");
			changed = true;
		}

		if (menu2.item("Color Material"))
		{
			createdEntity = ecs::Builder::create(registry)
								.withName(mergeMaterialName("Color Material"))
								.asMaterialInstance<editor::ColorMaterial>()
								.withMaterial("editor/materials/color.filamat");
			changed = true;
		}
	}

	if (menu.item("SkyBox"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Default SkyBox")
							.asMaterialInstance<editor::SkyBoxMaterial>()
							.withMaterial("editor/materials/skybox.filamat");
		changed = true;
	}

	if (isMeshInstanceSelected && registry.hasAnyComponent<ecs::tags::IsMaterialInstance>(createdEntity))
		ecs::MeshInstance::addMaterial(registry, selectedEntity, createdEntity);

	if (changed)
		selectedEntity = createdEntity;

	return changed;
}

bool MaterialsManager::list(const ecs::Registry& registry, ecs::Entity& selectedEntity, std::string_view search,
							bool showEditorEntities)
{
	using namespace boost::algorithm;
	using namespace std::string_view_literals;

	bool changed = false;

	static const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
										 | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
										 | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("MaterialInstancesTable", 1, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		const auto eachFn = [&](ecs::Entity entity, const auto& name) {
			if (!icontains(name.name, search))
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
			registry.getEntities<const ecs::tags::IsMaterialInstance, const ecs::Name>().each(eachFn);
		else
			registry
				.getEntities<const ecs::tags::IsMaterialInstance, const ecs::Name>(
					ecs::ExcludeComponents<editor::tags::IsEditorEntity>)
				.each(eachFn);

		ImGui::EndTable();
	}

	return changed;
}

bool EditorDragAndDrop::loadScene(std::string& scenePath, ecs::Entity& selectedEntity)
{
	auto dnd = DragAndDropTarget{};
	const auto result = dnd.getPayload();
	if (result && boost::algorithm::ends_with(result->c_str(), ".spatial.json"))
	{
		selectedEntity = ecs::NullEntity;
		scenePath = std::string{result.value()};
		return true;
	}

	return false;
}

bool EditorDragAndDrop::loadMeshInstance(ecs::Registry& registry, ecs::Entity& selectedEntity,
										 math::vec3 createEntityPosition)
{
	auto dnd = DragAndDropTarget{};
	auto result = dnd.getPayload();

	if (result && boost::algorithm::ends_with(result->c_str(), ".filamesh"))
	{
		selectedEntity = ecs::Builder::create(registry)
							 .asTransform()
							 .withPosition(createEntityPosition)
							 .asMeshInstance()
							 .withMesh(*result);

		return true;
	}

	return false;
}

bool EditorDragAndDrop::loadScriptResource(ecs::Registry& registry, ecs::Entity& selectedEntity,
										   ResourceManager::ResourceType& type)
{
	auto dnd = DragAndDropTarget{};
	auto result = dnd.getPayload();

	if (result && boost::algorithm::ends_with(result->c_str(), ".js"))
	{
		selectedEntity =
			ecs::Builder::create(registry).asResource().withPath(result.value()).with<ecs::tags::IsScript>();

		if (type != ResourceManager::ResourceType::All)
		{
			type = ResourceManager::ResourceType::Script;
		}

		return true;
	}

	return false;
}

bool SceneOptionsMenu::createEntitiesMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
										  math::vec3 createEntitiesPosition, bool addAsChild)
{
	bool changed = false;
	ecs::Entity newEntity = ecs::NullEntity;

	if (Menu::itemButton("Empty"))
	{
		newEntity = ecs::Builder::create(registry).withName("Empty Entity").with<ecs::tags::IsRenderable>();
		changed = true;
	}

	if (Menu::itemButton("Scene View"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Scene View")
						.with<ecs::tags::IsRenderedToTarget>()
						.asScene()
						.withDefaultAttachments();
		changed = true;
	}

	changed |= SceneOptionsMenu::createMeshMenu(registry, selectedEntity, createEntitiesPosition, addAsChild);
	changed |= SceneOptionsMenu::createLightMenu(registry, selectedEntity, createEntitiesPosition, addAsChild);
	changed |= SceneOptionsMenu::createCameraMenu(registry, selectedEntity, createEntitiesPosition, addAsChild);

	if (registry.isValid(newEntity))
	{
		if (registry.isValid(selectedEntity) && addAsChild)
			ecs::Parent::addChild(registry, selectedEntity, newEntity);

		selectedEntity = newEntity;
	}

	return changed;
}

bool SceneOptionsMenu::createMeshMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
									  math::vec3 createEntitiesPosition, bool addAsChild)
{
	auto menu = Menu{"Mesh"};
	if (!menu.isOpen())
		return false;

	bool changed = false;
	ecs::Entity newEntity = ecs::NullEntity;

	if (menu.item("Cube"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Cube")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMeshInstance()
						.withMesh("editor/meshes/cube.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Plane"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Plane")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMeshInstance()
						.withMesh("editor/meshes/plane.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Sphere"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Sphere")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMeshInstance()
						.withMesh("editor/meshes/sphere.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Cylinder"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Cylinder")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asMeshInstance()
						.withMesh("editor/meshes/cylinder.filamesh")
						.withShadowOptions(true, true)
						.withSubMesh(0, 1);
		changed = true;
	}

	if (menu.item("Skybox"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Skybox")
						.asMeshInstance()
						.withMesh("engine/skybox")
						.withDefaultMaterialInstance(ecs::Builder::create(registry)
														 .withName("Skybox Material")
														 .asMaterialInstance<editor::SkyBoxMaterial>()
														 .withMaterial("editor/materials/skybox.filamat")
														 .withProps({
															 false,
															 {math::vec3{.0f}, 1.0f},
															 ecs::Resource::findOrCreate<ecs::tags::IsCubeMapTexture>(
																 registry, "editor/textures/skybox/texture.ktx"),
														 }))
						.withCulling(false)
						.withPriority(0x7);
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

bool SceneOptionsMenu::createLightMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
									   math::vec3 createEntitiesPosition, bool addAsChild)
{
	auto menu = Menu{"Light"};
	if (!menu.isOpen())
		return false;

	bool changed = false;
	ecs::Entity newEntity = ecs::NullEntity;

	if (menu.item("Point Light"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Point Light")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asPointLight();
		changed = true;
	}

	if (menu.item("Directional Light"))
	{
		newEntity = ecs::Builder::create(registry).withName("Directional Light").asDirectionalLight();
		changed = true;
	}

	if (menu.item("Spot Light"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Spot Light")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asSpotLight();
		changed = true;
	}

	if (menu.item("Sun Light"))
	{
		newEntity = ecs::Builder::create(registry).withName("Sun Light").asSunLight();
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

bool SceneOptionsMenu::createCameraMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
										math::vec3 createEntitiesPosition, bool addAsChild)
{
	auto menu = Menu{"Camera"};
	if (!menu.isOpen())
		return false;

	bool changed = false;
	ecs::Entity newEntity = ecs::NullEntity;

	if (menu.item("Perspective Camera"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Perspective Camera")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asPerspectiveCamera();
		changed = true;
	}

	if (menu.item("Orthographic Camera"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Orthographic Camera")
						.asTransform()
						.withPosition(createEntitiesPosition)
						.asOrthographicCamera();
		changed = true;
	}

	if (menu.item("Custom Camera"))
	{
		newEntity = ecs::Builder::create(registry)
						.withName("Custom Camera")
						.asCustomCamera()
						.withProjection(math::perspective(45.0, 1280.0 / 720.0, .1, 1000.0));
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
									math::vec3 createEntitiesPosition)
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
		registry.addComponent<ecs::tags::CanDestroy>(selectedEntity);
		selectedEntity = ecs::NullEntity;
		changed = true;
	}

	return changed;
}

bool EditorMainMenu::fileMenu(graphics::OptionalTexture icons, EditorMainMenu::Action& action)
{
	bool changed = false;

	ImGui::SetCursorPosY(1.5f);
	ui::image(std::move(icons), math::vec2{20.0f}, Icons::logo.uv());
	ImGui::SetCursorPosY(0.0f);

	{
		auto menu = ui::Menu{"File"};
		if (menu.item("Open Project", "CTRL+SHIFT+O"))
		{
			action = Action::OpenProject;
			changed = true;
		}

		if (menu.item("New Scene", "CTRL+N"))
		{
			action = Action::NewScene;
			changed = true;
		}

		if (menu.item("Open Scene", "CTRL+O"))
		{
			action = Action::OpenScene;
			changed = true;
		}

		if (menu.item("Save Scene", "CTRL+S"))
		{
			action = Action::SaveScene;
			changed = true;
		}
	}

	return changed;
}

bool EditorMainMenu::viewOptionsMenu(bool& isEditorEntitiesShowing, bool& isEditorComponentsShowing)
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

	if (menu.item("Toggle Debug Components"))
	{
		isEditorComponentsShowing = !isEditorComponentsShowing;
		changed = true;
	}

	return changed;
}

bool EditorMainMenu::createMenu(ecs::Registry& registry, ecs::Entity& selectedEntity,
								const math::vec3& createEntitiesPosition)
{
	auto menu = Menu{"Create"};
	if (!menu.isOpen())
		return false;

	return SceneOptionsMenu::createEntitiesMenu(registry, selectedEntity, createEntitiesPosition);
}

} // namespace spatial::ui