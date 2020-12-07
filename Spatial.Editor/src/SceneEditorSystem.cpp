#include "SceneEditorSystem.h"

#include <spatial/input/Input.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/Transform.h>

#include "Components.h"
#include "Editor.h"
#include "ImGuiComponents.h"

namespace fl = filament;
using namespace filament::math;

namespace spatial::editor
{

SceneEditorSystem::SceneEditorSystem(RenderingSystem& renderingSystem)
	: mEngine{renderingSystem.getEngine()},
	  mViewport{0, 0, 1280, 720},

	  mMainStage{renderingSystem},
	  mDefaultMaterial{createMaterial(mEngine, editor::load("editor/materials/default.filamat").value())},

	  mIblTexture{createKtxTexture(mEngine, editor::load("editor/textures/default_skybox/ibl.ktx").value())},
	  mSkyboxTexture{createKtxTexture(mEngine, editor::load("editor/textures/default_skybox/skybox.ktx").value())},
	  mSkyboxLight{createImageBasedLight(mEngine, mIblTexture.ref(),
										 editor::load("editor/textures/default_skybox/sh.txt").value())},
	  mSkybox{createSkybox(mEngine, mSkyboxTexture.ref())},

	  mImGuiSceneWindow{mEngine, {mViewport.width, mViewport.height}},

	  mSelectedActor{}
{
	mMainStage.getView()->setRenderTarget(mImGuiSceneWindow.getRenderTarget());
	mMainStage.getView()->setViewport(mViewport);

	mMainStage.getScene().setIndirectLight(mSkyboxLight.get());
	mMainStage.getScene().setSkybox(mSkybox.get());

	mImGuiSceneWindow >> *this; // register imgui window resize events
}

void SceneEditorSystem::onStart()
{
	mMainStage.setMainCamera(mMainStage.createActor("Main Camera")
								 .withPosition({.6f, .3f, .6f})
								 .asCamera()
								 .withPerspectiveProjection(45.0, 19 / 6.0, .1, 1000.0)
								 .add<editor::BasicCameraMovement>(20.0f, 10.0f));

	mMainStage.createActor("Main Light").asLight(spatial::Light::Type::DIRECTIONAL).build();

	/*
	auto material = createMaterial(mEngine, editor::load("editor/materials/default.filamat").value());
	material->setDefaultParameter("metallic", .2f);
	material->setDefaultParameter("roughness", 0.3f);
	material->setDefaultParameter("reflectance", .1f);
	*/

	mSelectedActor = mMainStage.createActor("Plane").withPosition({3.0f, -1.0f, .0f}).withScale(10.0f).asRenderable(1);
	//.asMesh(editor::load("editor/meshes/plane.filamesh").value())
	//.withMaterial(material, {{"baseColor", {.8f, .8f, .8f, 1.0f}}});

	mMainStage.createActor("Cube").withPosition({.0f, .0f, .0f});
	//.asMesh(editor::load("editor/meshes/cube.filamesh").value())
	//.withMaterial(material, {{"baseColor", {.4f, 0.1f, 0.1f, 1.0f}}});

	mMainStage.createActor("Cylinder").withPosition({6.0f, .0f, .0f});
	//.asMesh(editor::load("editor/meshes/cylinder.filamesh").value())
	//.withMaterial(material, {{"baseColor", {.1f, 0.4f, 0.1f, 1.0f}}});

	mMainStage.createActor("Sphere").withPosition({3.0f, .0f, .0f});
	//.asMesh(editor::load("editor/meshes/sphere.filamesh").value())
	//.withMaterial(material, {{"baseColor", {.1f, 0.1f, 0.4f, 1.0f}}});

	onSceneWindowResized({1280, 720});

	mMainStage.enable();
}

void SceneEditorSystem::onEvent(const MouseMovedEvent&)
{
	auto mainCamera = mMainStage.getFirstActorWith<Transform, const BasicCameraMovement>();
	if (mainCamera)
	{
		auto& cameraTransform = mainCamera.getComponent<Transform>();
		auto& basicCameraMovement = mainCamera.getComponent<BasicCameraMovement>();

		if (basicCameraMovement.enabled)
		{
			constexpr auto center = float2{0.5f};
			const auto mouseRelativePosition = Input::mouse() / math::float2{mViewport.width, mViewport.height};
			const auto mouseDelta = (center - mouseRelativePosition) * basicCameraMovement.sensitivity;
			auto rotation = cameraTransform.getRotation();
			rotation.x -= mouseDelta.x;
			rotation.y = std::clamp(rotation.y + mouseDelta.y, -90.0f, 90.0f);
			cameraTransform.setRotation(rotation);

			Input::warpMouse({.5f, .5f});
		}
	}
}

float3 defaultInputAxis()
{
	return {
		Input::axis(Key::W, Key::S),
		Input::axis(Key::D, Key::A),
		Input::axis(Key::Space, Key::LShift),
	};
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	auto mainCamera = mMainStage.getFirstActorWith<Camera, Transform, BasicCameraMovement>();
	if (mainCamera)
	{
		auto& cameraController = mainCamera.getComponent<Camera>();
		auto& cameraTransform = mainCamera.getComponent<Transform>();
		auto& cameraMovement = mainCamera.getComponent<BasicCameraMovement>();

		const auto keyboardDelta = delta * cameraMovement.velocity * defaultInputAxis();
		const auto rot = math::toRadians(cameraTransform.getRotation());
		const auto direction = normalize(float3{cos(rot.x) * cos(rot.y), sin(rot.y), sin(rot.x) * cos(rot.y)});

		cameraTransform.translate(direction * keyboardDelta.x + cross(direction, math::axisY) * keyboardDelta.y +
								  math::axisY * keyboardDelta.z);

		cameraController.lookAt(cameraTransform.getPosition(), cameraTransform.getPosition() + direction);

		if (Input::released(Key::MouseLeft))
			cameraMovement.enabled = false;

		if (Input::combined(Key::LControl, Key::MouseLeft))
			cameraMovement.enabled = true;
	}
}

void SceneEditorSystem::onDrawGui()
{
	static bool showEngineGui = true;

	if (Input::released(Key::G))
		showEngineGui = !showEngineGui;

	if (!showEngineGui)
		return;

	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;
	static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar |
										  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
										  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
										  ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, windowFlags);
	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	// DockSpace
	static ImGuiID dockSpaceId = ImGui::GetID("SpatialDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockFlags);

	ImGui::BeginMainMenuBar();
	ImGui::Text("Spatial Engine");
	ImGui::Separator();

	if (ImGui::BeginMenu("Options"))
	{
		// ImGui::MenuItem("Properties", NULL, openedPropertiesPtr);
		// ImGui::MenuItem("Console", NULL, &gOpenedLogging);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	mImGuiSceneWindow.draw("Scene View");

	ImGui::Begin("Scene Graph");

	editor::sceneHierarchy(mMainStage, mSelectedActor);

	ImGui::End();

	if (mSelectedActor.isValid())
	{
		ImGui::Begin("Properties");

		{
			auto& name = mSelectedActor.getComponent<Name>();
			auto nameValue = name.getValue();
			if (editor::inputText("Name", nameValue))
			{
				name.setValue(nameValue);
			}
		}

		{
			auto& transform = mSelectedActor.getComponent<Transform>();
			if (ImGui::CollapsingHeader("Transform", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				if (mSelectedActor.hasComponent<Camera>())
					editor::transformInput(transform, "py");
				else if (mSelectedActor.hasComponent<Light>())
					editor::transformInput(transform, "p");
				else
					editor::transformInput(transform, "prs");

				ImGui::Unindent();
			}
		}

		if (mSelectedActor.hasComponent<Camera>())
		{
			if (ImGui::CollapsingHeader("Camera", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				auto& camera = mSelectedActor.getComponent<Camera>();
				editor::cameraInput(camera);

				if (mSelectedActor.hasComponent<editor::BasicCameraMovement>())
				{
					ImGui::Separator();

					auto& movement = mSelectedActor.getComponent<editor::BasicCameraMovement>();
					ImGui::DragFloat("Velocity", &movement.velocity);
					ImGui::DragFloat("Sensitivity", &movement.sensitivity);
				}

				ImGui::Unindent();
			}
		}

		if (mSelectedActor.hasComponent<Light>())
		{
			if (ImGui::CollapsingHeader("Light", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				auto& light = mSelectedActor.getComponent<Light>();
				editor::lightInput(light);

				ImGui::Unindent();
			}
		}

		ImGui::End();
	}
}

void SceneEditorSystem::onSceneWindowResized(ImGuiSceneWindow::Size size)
{
	auto width = static_cast<double>(size.first);
	auto height = static_cast<double>(size.second);
	auto aspectRatio = width / height;

	auto& camera = mMainStage.getMainCamera().getComponent<Camera>();
	std::visit(
		[&](auto projection) {
			using T = std::decay_t<decltype(projection)>;

			if constexpr (std::is_same_v<T, PerspectiveProjection>)
			{
				projection.aspectRatio = aspectRatio;
			}
			else if constexpr (std::is_same_v<T, OrthographicProjection>)
			{
				projection.left = -aspectRatio;
				projection.right = aspectRatio;
			}

			camera.setProjection(projection);
		},
		camera.getProjection());
}

void SceneEditorSystem::onEvent(const WindowResizedEvent& e)
{
	setViewport(e.windowSize);
}

void SceneEditorSystem::setViewport(const std::pair<int, int> windowSize)
{
	mViewport = {0, 0, static_cast<uint32_t>(windowSize.first), static_cast<uint32_t>(windowSize.second)};
}

} // namespace spatial::editor
