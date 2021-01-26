#include "SceneEditorSystem.h"

#include <assets/generated.h>
#include <spatial/input/Input.h>
#include <spatial/render/InstanceBuilder.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/Transform.h>

#include "Components.h"
#include "ImGuiComponents.h"

namespace fl = filament;
using namespace filament::math;

namespace spatial::editor
{

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine)
	: mEngine{engine},
	  mViewport{0, 0, 1280, 720},

	  mMainStage{mEngine},
	  mDefaultMaterial{createMaterial(mEngine, {ASSETS_DEFAULT, ASSETS_DEFAULT_SIZE})},

	  mIblTexture{createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_IBL, ASSETS_DEFAULT_SKYBOX_IBL_SIZE})},
	  mSkyboxTexture{createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_SKYBOX, ASSETS_DEFAULT_SKYBOX_SKYBOX_SIZE})},
	  mSkyboxLight{createImageBasedLight(mEngine, mIblTexture.ref(), {ASSETS_SH, ASSETS_SH_SIZE})},
	  mSkybox{createSkybox(mEngine, mSkyboxTexture.ref())},

	  mImGuiSceneWindow{mEngine, {mViewport.width, mViewport.height}},

	  mSelectedInstance{mMainStage, {}},

	  mMeshes{createFilamesh(mEngine, {ASSETS_CUBE, ASSETS_CUBE_SIZE}),
			  createFilamesh(mEngine, {ASSETS_PLANE, ASSETS_PLANE_SIZE}),
			  createFilamesh(mEngine, {ASSETS_CYLINDER, ASSETS_CYLINDER_SIZE}),
			  createFilamesh(mEngine, {ASSETS_SPHERE, ASSETS_SPHERE_SIZE})},

	  mMaterialInstances{
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
	  }
{
	mMainStage.getView()->setRenderTarget(mImGuiSceneWindow.getRenderTarget());
	mMainStage.getView()->setViewport(mViewport);

	mMainStage.getScene().setIndirectLight(mSkyboxLight.get());
	mMainStage.getScene().setSkybox(mSkybox.get());

	mImGuiSceneWindow >> *this; // register imgui window resize events
}

void SceneEditorSystem::onStart()
{
	mMainStage.setMainCamera(createInstance(mMainStage, "Main Camera")
								 .withPosition({.6f, .3f, .6f})
								 .asCamera()
								 .withPerspectiveProjection(45.0, 19 / 6.0, .1, 1000.0)
								 .add<editor::EditorCamera>(20.0f, 10.0f));

	createInstance(mMainStage, "Main Light").asLight(spatial::Light::Type::DIRECTIONAL).build();
	mDefaultMaterial->setDefaultParameter("metallic", .2f);
	mDefaultMaterial->setDefaultParameter("roughness", 0.3f);
	mDefaultMaterial->setDefaultParameter("reflectance", .1f);

	{
		mSelectedInstance = createInstance(mMainStage, "Cube")
								.withPosition({.0f})
								.asMesh(mMeshes[0])
								.withShadowOptions(true, true)
								.withMaterialAt(0, mMaterialInstances[0].get())
								.build();

		mMaterialInstances[0]->setParameter("baseColor", math::float3{.4f, 0.1f, 0.1f});
	}

	{
		createInstance(mMainStage, "Plane")
			.withPosition({3.0f, -1.0f, .0f})
			.withScale({10.0f})
			.asMesh(mMeshes[1])
			.withShadowOptions(false, true)
			.withMaterialAt(0, mMaterialInstances[1].get())
			.build();

		mMaterialInstances[1]->setParameter("baseColor", math::float3{.8f, .8f, .8f});
	}

	{
		createInstance(mMainStage, "Cylinder")
			.withPosition({6.0f, .0f, .0f})
			.asMesh(mMeshes[2])
			.withShadowOptions(true, true)
			.withMaterialAt(0, mMaterialInstances[2].get())
			.build();

		mMaterialInstances[2]->setParameter("baseColor", math::float3{.1f, 0.4f, 0.1f});
	}

	{
		auto actor = createInstance(mMainStage, "Sphere")
						 .withPosition({3.0f, .0f, .0f})
						 .asMesh(mMeshes[3])
						 .withShadowOptions(true, true)
						 .withMaterialAt(0, mMaterialInstances[3].get())
						 .build();

		auto* mi = actor.get<Renderable>().getMaterialInstanceAt(0);
		mi->setParameter("baseColor", math::float3{.1f, 0.1f, 0.4f});
	}

	onSceneWindowResized({1280, 720});
}

void SceneEditorSystem::onEvent(const MouseMovedEvent&)
{
	auto mainCamera = handleOf(mMainStage, mMainStage.getFirstInstance<Transform, const EditorCamera>());
	if (mMainStage.isValid(mainCamera))
	{
		auto& cameraTransform = mainCamera.get<Transform>();
		const auto& basicCameraMovement = mainCamera.get<EditorCamera>();

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
	auto mainCamera = handleOf(mMainStage, mMainStage.getFirstInstance<Camera, Transform, EditorCamera>());
	if (mainCamera)
	{
		auto& cameraController = mainCamera.get<Camera>();
		auto& cameraTransform = mainCamera.get<Transform>();
		auto& cameraMovement = mainCamera.get<EditorCamera>();

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

	// ImGui::ShowDemoWindow();

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

	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create New Actor"))
		{
			mSelectedInstance = createInstance(mMainStage, "New Actor");
			ImGui::SetWindowFocus("Properties");
		}

		ImGui::EndPopup();
	}

	editor::instancesTreeView(mMainStage, mSelectedInstance);

	ImGui::End();

	ImGui::Begin("Properties");
	if (mSelectedInstance.isValid())
	{
		{
			auto& name = mSelectedInstance.get<SceneNodeName>();
			editor::inputText("##Name", name.value);

			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::BeginPopup("AddComponent"))
		{
			auto builder = InstanceBuilder{mMainStage, mSelectedInstance};

			if (ImGui::MenuItem("Camera"))
			{
				builder.asCamera().withPerspectiveProjection(45.0, mImGuiSceneWindow.getAspectRatio(), .1, 10000.0);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Light"))
			{
				builder.asLight(Light::Type::POINT);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		componentView<Transform>("Transform", mSelectedInstance);
		componentView<Camera>("Camera", mSelectedInstance);
		componentView<EditorCamera>("Editor Camera", mSelectedInstance);
		componentView<Light>("Light", mSelectedInstance);
		componentView<Renderable>("Renderer", mSelectedInstance);
	}
	else
	{
		ImGui::Text("No actor selected.");
	}
	ImGui::End();
}

void SceneEditorSystem::onSceneWindowResized(ImGuiSceneWindow::Size size)
{
	auto width = static_cast<double>(size.first);
	auto height = static_cast<double>(size.second);
	auto aspectRatio = width / height;

	auto& camera = mMainStage.getComponent<Camera>(mMainStage.getMainCamera());
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

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	mMainStage.render(renderer);
}

} // namespace spatial::editor
