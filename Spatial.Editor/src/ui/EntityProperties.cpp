#include "EntityProperties.h"
#include "Components.h"
#include <imgui.h>
#include <spatial/ui/components/Basic.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/Popup.h>

namespace spatial::ui
{

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

} // namespace spatial::ui