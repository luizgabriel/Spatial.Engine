#include "Menu.h"
#include "../ecs/Components.h"
#include "../ecs/Materials.h"
#include "Icons.h"
#include <fmt/format.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ui/components/Basic.h>
#include <spatial/ui/components/Menu.h>

namespace spatial::ui
{

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
		auto skyboxMaterial = registry.getFirstEntity<editor::SkyBoxMaterial>();
		if (!registry.isValid(skyboxMaterial))
		{
			skyboxMaterial = ecs::Builder::create(registry)
								 .withName("Skybox Material")
								 .asMaterialInstance()
								 .withMaterial("editor/materials/skybox.filamat")
								 .with(editor::SkyBoxMaterial{
									 false,
									 {math::vec3{.0f}, 1.0f},
									 ecs::FileSystemResource::findOrCreate<ecs::tags::IsCubeMapTexture>(
										 registry, "editor/textures/skybox/texture.ktx"),
								 });
		}

		createdEntity = ecs::Builder::create(registry)
							.withName("Skybox")
							.asMeshInstance()
							.withMesh(registry.getFirstEntity<editor::tags::IsSkyboxMesh>())
							.withDefaultMaterialInstance(skyboxMaterial)
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
		auto viewSize = gStandardAspectRatio.toVector() * 240.0;
		createdEntity = ecs::Builder::create(registry)
							.withName("View")
							.with<ecs::tags::IsRenderedToTarget>()
							.asView()
							.withDimensions(viewSize)
							.withAttachment(ecs::Builder::create(registry)
												.withName("Color Attachment")
												.asAttachmentTexture()
												.withDimensions(viewSize)
												.withFormat(ecs::AttachmentTexture::Type::Color))
							.withAttachment(ecs::Builder::create(registry)
												.withName("Depth Attachment")
												.asAttachmentTexture()
												.withDimensions(viewSize)
												.withFormat(ecs::AttachmentTexture::Type::Depth));
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