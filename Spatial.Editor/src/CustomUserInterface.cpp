#include "CustomUserInterface.h"
#include "Components.h"
#include "Materials.h"
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

void ComponentInputImpl<editor::SkyBoxMaterial, graphics::OptionalTexture>::draw(ecs::Registry& registry,
																				 ecs::Entity entity,
																				 graphics::OptionalTexture icons)
{
	auto& data = registry.getComponent<editor::SkyBoxMaterial>(entity);

	ui::colorPicker("Background Color", data.color, icons);
	ui::cubemapInput("Cubemap", registry, data.skybox, icons);
	ImGui::Checkbox("Show Sun", &data.showSun);
}

void ComponentInputImpl<editor::StandardOpaqueMaterial, graphics::OptionalTexture>::draw(
	ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons)

{
	auto& data = registry.getComponent<editor::StandardOpaqueMaterial>(entity);

	ui::albedoInput("Albedo", data.baseColor, registry, data.albedo, icons);

	ui::separator(2);

	ImGui::DragFloat2("Tiling", &data.tiling.x, .01f, .01f);
	ImGui::DragFloat2("Offset", &data.offset.x, .01f);

	ui::separator(1);

	ui::mapInput("Metallic", data.metallic, registry, data.metallicMap, icons);

	ui::separator(1);

	ui::mapInput("Roughness", data.roughness, registry, data.roughnessMap, icons);

	ui::separator(1);

	ui::mapInput("Reflectance", data.reflectance, registry, data.reflectanceMap, icons);

	ui::separator(1);

	ui::mapInput("Ambient Occlusion", registry, data.ambientOcclusionMap, icons);

	ui::separator(1);

	ui::mapInput("Normal", registry, data.normalMap, icons, Icons::normalMap.uv());

	ui::separator(1);

	ui::mapInput("Height Map", data.height, registry, data.heightMap, icons);

	ui::separator(1);

	ui::colorPicker("Emissive", data.emissive, icons);
}

void EntityProperties::displayComponents(ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons,
										 bool showDebugComponents)
{
	bool isValid = registry.isValid(entity);
	if (!isValid)
	{
		ImGui::Text("No entity selected.");
		return;
	}

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
	componentCollapse<ecs::FileSystemResource>(registry, entity);
	componentCollapse<ecs::MeshInstance>(registry, entity, icons);
	componentCollapse<ecs::MeshPrimitive>(registry, entity, icons);
	componentCollapse<ecs::MaterialInstance>(registry, entity, icons);
	componentCollapse<ecs::View>(registry, entity, icons);
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

	// componentTag<editor::tags::IsSelected>(registry, entity);
	componentTag<editor::tags::IsSelectedView>(registry, entity);
	componentTag<editor::tags::IsEditorEntity>(registry, entity);
}

void EntityProperties::displayEntityName(ecs::Registry& registry, ecs::Entity selectedEntity)
{
	if (registry.hasAllComponents<ecs::Name>(selectedEntity))
	{
		auto& name = registry.getComponent<ecs::Name>(selectedEntity);
		inputText("Name", name.name);
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

	if (!registry.hasAnyComponent<ecs::View>(entity) && menu.item("View"))
		registry.addComponent<ecs::View>(entity);
}

void EntityProperties::popup(ecs::Registry& registry, ecs::Entity entity)
{
	if (!registry.isValid(entity))
		return;

	if (registry.hasAnyComponent<ecs::MaterialInstance>(entity))
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

void SceneTree::displayTree(ecs::Registry& registry, bool showDebugEntities, std::string_view search)
{
	using namespace boost::algorithm;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
	{
		registry.removeComponentFromEntities<editor::tags::IsSelected>();
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

	if (ImGui::IsItemClicked() && !shiftPressed)
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
			auto shiftPressed = ImGui::IsKeyPressed(ImGuiKey_LeftShift);
			if (ImGui::IsItemClicked() && shiftPressed)
				registry.addComponent<editor::tags::IsSelected>(entity);

			if (ImGui::IsItemClicked() && !shiftPressed)
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

			auto shiftPressed = ImGui::IsKeyPressed(ImGuiKey_LeftShift);
			if (ImGui::IsItemClicked() && shiftPressed)
				registry.addComponent<editor::tags::IsSelected>(entity);

			if (ImGui::IsItemClicked() && !shiftPressed)
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

			auto shiftPressed = ImGui::IsKeyPressed(ImGuiKey_LeftShift);
			if (ImGui::IsItemClicked() && shiftPressed)
				registry.addComponent<editor::tags::IsSelected>(entity);

			if (ImGui::IsItemClicked() && !shiftPressed)
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

bool EditorDragAndDrop::loadScene(std::string& scenePath)
{
	auto dnd = DragAndDropTarget{};
	const auto result = dnd.getPayload();
	if (result && boost::algorithm::ends_with(result->c_str(), ".spatial.json"))
	{
		scenePath = std::string{result.value()};
		return true;
	}

	return false;
}

void EditorDragAndDrop::loadMeshInstance(ecs::Registry& registry, math::vec3 createEntityPosition)
{
	auto dnd = DragAndDropTarget{};
	auto result = dnd.getPayload();
	ecs::Entity createdEntity = ecs::NullEntity;

	if (result && boost::algorithm::ends_with(result->c_str(), ".filamesh"))
	{
		createdEntity = ecs::Builder::create(registry)
							.asTransform()
							.withPosition(createEntityPosition)
							.asMeshInstance()
							.withMesh(*result);
	}

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

void EditorDragAndDrop::loadScriptResource(ecs::Registry& registry, ResourceManager::ResourceType& type)
{
	auto dnd = DragAndDropTarget{};
	auto result = dnd.getPayload();
	ecs::Entity createdEntity = ecs::NullEntity;

	if (result && boost::algorithm::ends_with(result->c_str(), ".js"))
	{
		createdEntity = ecs::Builder::create(registry) //
							.asFileSystemResource()
							.withPath(result.value())
							.with<ecs::tags::IsScript>();

		if (type != ResourceManager::ResourceType::All)
			type = ResourceManager::ResourceType::Script;
	}

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

void CreateMenu::createMeshMenu(ecs::Registry& registry, math::vec3 createEntitiesPosition)
{
	auto menu = Menu{"Mesh"};
	if (!menu.isOpen())
		return;

	ecs::Entity createdEntity = ecs::NullEntity;

	if (menu.item("Cube"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Cube")
							.asTransform()
							.withPosition(createEntitiesPosition)
							.asMeshInstance()
							.withMesh("editor/meshes/cube.filamesh")
							.withShadowOptions(true, true);
	}

	if (menu.item("Plane"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Plane")
							.asTransform()
							.withPosition(createEntitiesPosition)
							.asMeshInstance()
							.withMesh("editor/meshes/plane.filamesh")
							.withShadowOptions(true, true);
	}

	if (menu.item("Sphere"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Sphere")
							.asTransform()
							.withPosition(createEntitiesPosition)
							.asMeshInstance()
							.withMesh("editor/meshes/sphere.filamesh")
							.withShadowOptions(true, true);
	}

	if (menu.item("Cylinder"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Cylinder")
							.asTransform()
							.withPosition(createEntitiesPosition)
							.asMeshInstance()
							.withMesh("editor/meshes/cylinder.filamesh")
							.withShadowOptions(true, true);
	}

	if (menu.item("Skybox"))
	{
		createdEntity =
			ecs::Builder::create(registry)
				.withName("Skybox")
				.asMeshInstance()
				.withMesh("engine/skybox")
				.withDefaultMaterialInstance(ecs::Builder::create(registry)
												 .withName("Skybox Material")
												 .asMaterialInstance()
												 .withMaterial("editor/materials/skybox.filamat")
												 .with(editor::SkyBoxMaterial{
													 false,
													 {math::vec3{.0f}, 1.0f},
													 ecs::FileSystemResource::findOrCreate<ecs::tags::IsCubeMapTexture>(
														 registry, "editor/textures/skybox/texture.ktx"),
												 }))
				.withCulling(false)
				.withPriority(0x7);
	}

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

void CreateMenu::createLightMenu(ecs::Registry& registry, math::vec3 createEntitiesPosition)
{
	auto menu = Menu{"Light"};
	if (!menu.isOpen())
		return;

	ecs::Entity createdEntity = ecs::NullEntity;

	if (menu.item("Point Light"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Point Light")
							.asTransform()
							.withPosition(createEntitiesPosition)
							.asPointLight();
	}

	if (menu.item("Directional Light"))
	{
		createdEntity = ecs::Builder::create(registry).withName("Directional Light").asDirectionalLight();
	}

	if (menu.item("Spot Light"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("Spot Light")
							.asTransform()
							.withPosition(createEntitiesPosition)
							.asSpotLight();
	}

	if (menu.item("Sun Light"))
	{
		createdEntity = ecs::Builder::create(registry).withName("Sun Light").asSunLight();
	}

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

void CreateMenu::createCameraMenu(ecs::Registry& registry, math::vec3 createEntitiesPosition)
{
	auto menu = Menu{"Camera"};
	if (!menu.isOpen())
		return;

	ecs::Entity cameraEntity = ecs::NullEntity;

	if (menu.item("Perspective Camera"))
	{
		cameraEntity = ecs::Builder::create(registry)
						   .withName("Perspective Camera")
						   .asTransform()
						   .withPosition(createEntitiesPosition)
						   .asPerspectiveCamera();
	}

	if (menu.item("Orthographic Camera"))
	{
		cameraEntity = ecs::Builder::create(registry)
						   .withName("Orthographic Camera")
						   .asTransform()
						   .withPosition(createEntitiesPosition)
						   .asOrthographicCamera();
	}

	if (menu.item("Custom Camera"))
	{
		cameraEntity = ecs::Builder::create(registry)
						   .withName("Custom Camera")
						   .asCustomCamera()
						   .withProjection(math::perspective(45.0, 1280.0 / 720.0, .1, 1000.0));
	}

	if (registry.isValid(cameraEntity))
		editor::tags::IsSelected::replace(registry, cameraEntity);
}

void CreateMenu::createMaterialsMenu(ecs::Registry& registry)
{
	auto selectedEntity = registry.getFirstEntity<editor::tags::IsSelected>();
	ecs::Entity createdEntity = ecs::NullEntity;

	const auto isMeshInstanceSelected = registry.hasComponent<ecs::MeshInstance>(selectedEntity);
	const auto* selectedMeshInstanceName = registry.tryGetComponent<ecs::Name>(selectedEntity);

	const auto mergeMaterialName = [&](const char* materialName) {
		if (selectedMeshInstanceName && isMeshInstanceSelected)
			return fmt::format("{} ({})", materialName, selectedMeshInstanceName->c_str());
		else
			return std::string{materialName};
	};

	auto menu = ui::Menu{"Material"};

	if (menu.item("Standard Opaque"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName(mergeMaterialName("Standard Opaque"))
							.asMaterialInstance()
							.withMaterial("editor/materials/standard_lit.filamat")
							.with(editor::StandardOpaqueMaterial{});
	}

	if (menu.item("Color Material"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName(mergeMaterialName("Color Material"))
							.asMaterialInstance()
							.withMaterial("editor/materials/color.filamat")
							.with(editor::ColorMaterial{});
	}

	if (menu.item("SkyBox Material"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("SkyBox Material")
							.asMaterialInstance()
							.withMaterial("editor/materials/skybox.filamat")
							.with(editor::SkyBoxMaterial{});
	}

	if (isMeshInstanceSelected && registry.hasAnyComponent<ecs::MaterialInstance>(createdEntity))
		ecs::MeshInstance::addMaterial(registry, selectedEntity, createdEntity);

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

void CreateMenu::createViewMenu(ecs::Registry& registry)
{
	ecs::Entity createdEntity = ecs::NullEntity;

	if (Menu::itemButton("View"))
	{
		createdEntity = ecs::Builder::create(registry)
							.withName("View")
							.with<ecs::tags::IsRenderedToTarget>()
							.asView()
							.withDefaultAttachments();
	}

	if (registry.isValid(createdEntity))
		editor::tags::IsSelected::replace(registry, createdEntity);
}

void CreateMenu::removeMenu(ecs::Registry& registry)
{
	registry.getEntities<editor::tags::IsSelected>().each([&](ecs::Entity selectedEntity) {
		auto* name = registry.tryGetComponent<ecs::Name>(selectedEntity);
		if (Menu::itemButton(name ? fmt::format("Remove \"{}\"", name->c_str()) : "Remove Entity"))
		{
			registry.addComponent<ecs::tags::CanDestroy>(selectedEntity);
			registry.removeComponent<editor::tags::IsSelected>(selectedEntity);
		}
	});
}

void EditorMainMenu::fileMenu(graphics::OptionalTexture icons, EditorMainMenu::Action& action)
{
	ui::image(std::move(icons), math::vec2{20.0f}, Icons::logo.uv());

	auto menu = ui::Menu{"File"};
	if (menu.isOpen())
	{
		if (menu.item("Open Project", fmt::format("CTRL+SHIFT+O")))
			action = Action::OpenProject;

		if (menu.item("New Scene", "CTRL+N"))
			action = Action::NewScene;

		if (menu.item("Open Scene", "CTRL+O"))
			action = Action::OpenScene;

		if (menu.item("Save Scene", "CTRL+S"))
			action = Action::SaveScene;
	}
}

void EditorMainMenu::viewOptionsMenu(bool& isEditorEntitiesShowing, bool& isEditorComponentsShowing)
{
	auto menu = Menu{"View"};
	if (!menu.isOpen())
		return;

	if (menu.item("Toggle Debug Entities"))
		isEditorEntitiesShowing = !isEditorEntitiesShowing;

	if (menu.item("Toggle Debug Components"))
		isEditorComponentsShowing = !isEditorComponentsShowing;
}

void EditorMainMenu::createMenu(ecs::Registry& registry, const math::vec3& createEntitiesPosition)
{
	auto menu = Menu{"Create"};
	if (!menu.isOpen())
		return;

	if (Menu::itemButton("Empty"))
		ecs::Builder::create(registry).withName("Empty Entity").with<ecs::tags::IsRenderable>();

	CreateMenu::createMeshMenu(registry, createEntitiesPosition);
	CreateMenu::createLightMenu(registry, createEntitiesPosition);
	CreateMenu::createCameraMenu(registry, createEntitiesPosition);
	CreateMenu::createMaterialsMenu(registry);
	CreateMenu::createViewMenu(registry);
}

} // namespace spatial::ui