#include "EditorSystem.h"
#include "Serialization.h"
#include "ecs/Components.h"
#include "ecs/Materials.h"
#include "ui/Components.h"
#include "ui/DragAndDrop.h"
#include "ui/EntityProperties.h"
#include "ui/FilesExplorer.h"
#include "ui/Modals.h"
#include "ui/Search.h"

#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/ecs/View.h>
#include <spatial/graphics/MaterialController.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/resources/PhysicalFileSystem.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/Popup.h>
#include <spatial/ui/components/PopupModal.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Window.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/styles/WindowPaddingStyle.h>

namespace spatial::editor
{

static auto gLogger = createDefaultLogger();

EditorSystem::EditorSystem() : mFileSystem{}
// , mIsolate{mPlatformContext.createIsolate()}
{
}

void EditorSystem::onStart()
{
	mJobQueue.connect<OpenProjectEvent>(*this);
	mJobQueue.connect<ClearSceneEvent>(*this);
	mJobQueue.connect<LoadSceneEvent>(*this);
	mJobQueue.connect<SaveSceneEvent>(*this);

	createDefaultEditorEntities();

	mIconTexture =
		ecs::FileSystemResource::create<ecs::tags::IsTexture, ecs::tags::IsImageTexture>(mEditorRegistry, "editor/textures/icons/icons.png");
}

void EditorSystem::onStartFrame(float)
{
	mJobQueue.update();
}

void EditorSystem::onUpdateFrame(float delta)
{
	// script::ScriptController::loadScripts(mRegistry, mFileSystem, mIsolate);

	if (mIsCameraViewWindowHovered && mIsCameraControlEnabled)
		EditorCameraController::updateCameraTransforms(mRegistry, delta);

	graphics::MaterialController::updateMaterial<GridMaterial>(mRegistry);
	graphics::MaterialController::updateMaterial<ColorMaterial>(mRegistry);
	graphics::MaterialController::updateMaterial<StandardOpaqueMaterial>(mRegistry);
	graphics::MaterialController::updateMaterial<SkyBoxMaterial>(mRegistry);
}

void EditorSystem::onDrawGui()
{
	static bool showDebugEntities{false};
	static bool showDebugComponents{false};
	static std::string rootPath = {};

	auto icons = graphics::getTexture(mEditorRegistry, mIconTexture);
	const auto cameraEntity = mRegistry.getFirstEntity<const ecs::Transform, EditorCamera>();
	const auto* cameraTransform = mRegistry.tryGetComponent<const ecs::Transform>(cameraEntity);
	const auto createEntityPosition =
		cameraTransform != nullptr
			? (cameraTransform->position + (cameraTransform->getForwardVector() * 10.0F) - (math::axisY * 0.1f))
			: math::vec3{};

	ui::MenuBar::show([&]() {
		ui::EditorMainMenu::fileMenu(icons, mMenuAction);
		ui::EditorMainMenu::createMenu(mRegistry, createEntityPosition);
		ui::EditorMainMenu::viewOptionsMenu(showDebugEntities, showDebugComponents);
	});

	if (mMenuAction != ui::EditorMainMenu::Action::None)
	{
		if (mMenuAction == ui::EditorMainMenu::Action::NewScene)
			ui::PopupModal::open("New Scene");

		if (mMenuAction == ui::EditorMainMenu::Action::OpenScene)
			ui::PopupModal::open("Open Scene");

		if (mMenuAction == ui::EditorMainMenu::Action::SaveScene)
			ui::PopupModal::open("Save Scene");

		if (mMenuAction == ui::EditorMainMenu::Action::OpenProject)
			ui::PopupModal::open("Open Project");

		mMenuAction = ui::EditorMainMenu::Action::None;
	}

	ui::PopupModal::show("New Scene", [&]() {
		if (ui::EditorModals::newScene())
			mJobQueue.enqueue<ClearSceneEvent>();
	});

	ui::PopupModal::show("Open Scene", [&]() {
		if (ui::EditorModals::openScene(mScenePath))
			mJobQueue.enqueue<OpenProjectEvent>(mScenePath);
	});

	ui::PopupModal::show("Save Scene", [&]() {
		if (ui::EditorModals::saveScene(mScenePath))
			mJobQueue.enqueue<SaveSceneEvent>(mScenePath);
	});

	ui::PopupModal::show("Open Project", [&]() {
		if (ui::EditorModals::openProject(rootPath))
			mJobQueue.enqueue<OpenProjectEvent>(rootPath);
	});

	{
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View", ui::WindowFlags::NoScrollbar};
		mIsCameraViewWindowHovered = ImGui::IsWindowHovered();

		auto selectedView = mRegistry.getFirstEntity<editor::tags::IsSelectedView>();
		if (!mRegistry.isValid(selectedView))
			EditorCamera::replaceView(mRegistry, selectedView);

		const auto imageSize = window.getSize() - math::vec2{0, 24};
		ui::SceneView::image(mRegistry, selectedView, imageSize);

		if (ui::EditorDragAndDrop::loadScene(mScenePath))
			mJobQueue.enqueue<LoadSceneEvent>(mScenePath);

		ui::EditorDragAndDrop::loadMeshInstance(mRegistry, createEntityPosition);
		if (auto style2 = ui::WindowPaddingStyle{math::vec2{10.0f}}; ui::SceneView::selector(mRegistry, selectedView))
			EditorCamera::replaceView(mRegistry, selectedView);
	}

	ui::Window::show("Registry", [&]() {
		static std::string search;

		ui::Search::text(search, icons);

		static ui::ComponentFilter componentFilter = ui::ComponentFilter::Renderables;
		ui::Search::componentFilter(componentFilter);

		ui::RegistryManager::list(mRegistry, search, componentFilter, showDebugEntities);
		ui::EditorDragAndDrop::loadMeshInstance(mRegistry, createEntityPosition);

		ui::Popup::show("Registry Popup", [&]() {
			ui::Menu::show("Create", [&]() {
				ui::CreateMenu::createMeshMenu(mRegistry, createEntityPosition);
				ui::CreateMenu::createCameraMenu(mRegistry, createEntityPosition);
				ui::CreateMenu::createLightMenu(mRegistry, createEntityPosition);
				ui::CreateMenu::createViewMenu(mRegistry);
				ui::CreateMenu::createMaterialsMenu(mRegistry);
			});

			ui::CreateMenu::removeMenu(mRegistry);
		});
	});

	ui::Window::show("Components", [&]() {
		auto selected = mRegistry.getFirstEntity<editor::tags::IsSelected>();

		ui::EntityProperties::popup(mRegistry, selected);
		ui::EntityProperties::displayComponents(mRegistry, selected, icons, showDebugComponents);
	});

	ui::Window::show("Assets Explorer", [&]() {
		ui::FilesExplorer::displayFiles(mFileSystem, mCurrentPath, icons);
		if (mFileSystem.list(PROJECT_DIR).empty())
		{
			if (ImGui::Button("Open Project"))
				mMenuAction = ui::EditorMainMenu::Action::OpenProject;
			ImGui::SameLine();
			ImGui::Text("No files inside this project.");
		}
	});
}

void EditorSystem::onPublishRegistry(const std::function<void(ecs::Registry&)>& publisher)
{
	publisher(mRegistry);
	publisher(mEditorRegistry);
}

void EditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mIsCameraControlEnabled = input.pressed(Key::MouseRight);
	EditorCameraController::readCameraInputs(mRegistry, input);

	if (input.combined(gControlKey, Key::N))
		mMenuAction = ui::EditorMainMenu::Action::NewScene;

	if (input.combined(gControlKey, Key::S))
		mMenuAction = ui::EditorMainMenu::Action::SaveScene;

	if (input.combined(gControlKey, Key::LShift, Key::O))
		mMenuAction = ui::EditorMainMenu::Action::OpenProject;

	if (input.combined(gControlKey, Key::O))
		mMenuAction = ui::EditorMainMenu::Action::OpenScene;

	if (input.combined(gControlKey, Key::C))
		ImGui::LogToClipboard();

	if (input.combined(gControlKey, Key::V))
		ImGui::LogToTTY();
}

void EditorSystem::setScenePath(const std::string& path)
{
	mScenePath = path;
}

void EditorSystem::clearScene()
{
	mRegistry = ecs::Registry{};
	createDefaultEditorEntities();
}

void EditorSystem::loadScene()
{
	auto path = getScenePath();

	auto stream = mFileSystem.openReadStream(path);
	if (stream->fail())
	{
		gLogger.error("Could not open scene file: {}", path);
		return;
	}

	try
	{
		mRegistry = parseRegistry(std::move(*stream));
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not load scene: {}", e.what());
	}
}

void EditorSystem::saveScene()
{
	auto path = getScenePath();

	auto stream = mFileSystem.openWriteStream(path);
	if (stream->fail())
	{
		gLogger.error("Could not open scene file: {}", path);
		return;
	}

	try
	{
		writeRegistry(mRegistry, *stream);
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not save scene: {}", e.what());
	}
}

std::string EditorSystem::getScenePath() const
{
	auto path = mScenePath;
	auto projectPath = std::string{"project"} + FileSystem::SEPARATOR;
	if (!boost::starts_with(mScenePath, projectPath))
		path = projectPath + path;

	if (!boost::ends_with(mScenePath, ".spatial.json"))
		path = path + ".spatial.json";

	return path;
}

void EditorSystem::setRootPath(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
		return;

	mCurrentPath = PROJECT_DIR;
	mFileSystem.mount(PROJECT_DIR, std::make_shared<PhysicalFileSystem>(path));
}

void EditorSystem::onEvent(const ClearSceneEvent&)
{
	clearScene();
}

void EditorSystem::onEvent(const LoadSceneEvent& event)
{
	setScenePath(event.path);
	loadScene();
}

void EditorSystem::onEvent(const SaveSceneEvent& event)
{
	setScenePath(event.path);
	saveScene();
}

void EditorSystem::onEvent(const OpenProjectEvent& event)
{
	clearScene();
	setRootPath(event.path);
	setScenePath("scenes/default.spatial.json");
}

void EditorSystem::createDefaultEditorEntities()
{
	ecs::Builder::create(mRegistry)
		.withName("editor/dummy_cubemap")
		.with<tags::IsEditorEntity>()
		.with<ecs::tags::IsTexture>()
		.with<ecs::tags::IsCubeMapTexture>()
		.with<ecs::tags::IsResource>()
		.with<ecs::tags::IsDummyCubeMapTexture>();

	ecs::Builder::create(mRegistry)
		.withName("editor/white_texture")
		.with<ecs::tags::IsTexture>()
		.with<ecs::tags::IsImageTexture>()
		.with<tags::IsEditorWhiteTexture>()
		.with<ecs::tags::IsResource>()
		.with<tags::IsEditorEntity>()
		.with(ecs::RuntimeTexture{{0xFFFFFFFF}, 1});

	ecs::Builder::create(mRegistry)
		.withName("editor/black_texture")
		.with<ecs::tags::IsTexture>()
		.with<ecs::tags::IsImageTexture>()
		.with<tags::IsEditorBlackTexture>()
		.with<ecs::tags::IsResource>()
		.with<tags::IsEditorEntity>()
		.with(ecs::RuntimeTexture{{0xFF000000}, 1});

	ecs::Builder::create(mRegistry)
		.withName("Grid Plane")
		.with<tags::IsEditorEntity>()
		.asTransform()
		.withScale({100.0F, 1.0f, 100.0F})
		.withPosition({.0f, -0.01f, .0f})
		.asMeshInstance()
		.withMesh("editor/meshes/plane.filamesh")
		.withDefaultMaterialInstance(ecs::Builder::create(mRegistry)
										 .withName("Grid Material")
										 .with<tags::IsEditorEntity>()
										 .asMaterialInstance()
										 .withMaterial("editor/materials/grid.filamat")
										 .with(GridMaterial{}));

	ecs::Builder::create(mRegistry)
		.withName("SkyBox")
		.asMeshInstance()
		.withMesh(ecs::Builder::create(mRegistry)
					  .withName("editor/skybox_mesh")
					  .with<tags::IsEditorEntity>()
					  .with<tags::IsSkyboxMesh>()
					  .asMesh()
					  .withVertexData(gSkyboxVertexData)
					  .withIndexData(gSkyboxIndexData))
		.withDefaultMaterialInstance(ecs::Builder::create(mRegistry)
										 .withName("SkyBox Material")
										 .asMaterialInstance()
										 .withMaterial("editor/materials/skybox.filamat")
										 .with(SkyBoxMaterial{
											 false,
											 math::vec4{math::vec3{0.3f}, 1.0f},
											 ecs::FileSystemResource::findOrCreate<ecs::tags::IsCubeMapTexture>(
												 mRegistry, "editor/textures/skybox/texture.ktx"),
										 }))
		.withShadowOptions(false, false)
		.withCulling(false)
		.withPriority(0x7);

	auto viewSize = gUltraWideAspectRatio.toVector() * 240.0;
	ecs::Builder::create(mRegistry)
		.withName("Editor View")
		.with<tags::IsEditorEntity>()
		.with<tags::IsSelectedView>()
		.with<ecs::tags::IsRenderedToTarget>()
		.asView()
		.withDimensions(viewSize)
		.withAttachment(ecs::Builder::create(mRegistry)
							.withName("Color Attachment (Editor View)")
							.with<tags::IsEditorEntity>()
							.asAttachmentTexture()
							.withDimensions(viewSize)
							.withFormat(ecs::AttachmentTexture::Type::Color))
		.withAttachment(ecs::Builder::create(mRegistry)
							.withName("Depth Attachment (Editor View)")
							.with<tags::IsEditorEntity>()
							.asAttachmentTexture()
							.withDimensions(viewSize)
							.withFormat(ecs::AttachmentTexture::Type::Depth))
		.withIndirectLight(ecs::Builder::create(mRegistry)
							   .withName("Indirect Light")
							   .with<tags::IsEditorEntity>()
							   .asIndirectLight()
							   .withReflectionsTexture("editor/textures/skybox/ibl.ktx")
							   .withIrradianceValues("editor/textures/skybox/sh.txt"))
		.withCamera(ecs::Builder::create(mRegistry)
						.withName("Editor Camera")
						.with(EditorCamera{})
						.with<tags::IsEditorEntity>()
						.asTransform()
						.withPosition({3.0F, 3.0F, 20.0F})
						.asPerspectiveCamera()
						.withAspectRatio(gUltraWideAspectRatio));
}

} // namespace spatial::editor
