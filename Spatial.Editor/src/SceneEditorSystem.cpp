#include "SceneEditorSystem.h"

#include <assets/generated.h>
#include <spatial/core/Logger.h>
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

auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window,
									 const desktop::InputState& inputState)
	: mEngine{engine},
	  mWindow{window},
	  mInputState{inputState},

	  mMainStage{mEngine},
	  mDefaultMaterial{createMaterial(mEngine, {ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE})},

	  mIblTexture{createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE})},
	  mSkyboxTexture{
		  createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE})},
	  mSkyboxLight{createImageBasedLight(mEngine, mIblTexture.ref(), {ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE})},
	  mSkybox{createSkybox(mEngine, mSkyboxTexture.ref())},

	  mImGuiSceneWindow{mEngine, mWindow.getSize()},

	  mSelectedInstance{mMainStage, {}},

	  mMeshes{createFilamesh(mEngine, {ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE}),
			  createFilamesh(mEngine, {ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE}),
			  createFilamesh(mEngine, {ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE}),
			  createFilamesh(mEngine, {ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE})},

	  mMaterialInstances{
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
		  createMaterialInstance(mEngine, mDefaultMaterial.ref()),
	  }
{
	mMainStage.getView()->setRenderTarget(mImGuiSceneWindow.getRenderTarget());

	const auto windowSize = mWindow.getSize();
	mMainStage.getView()->setViewport({0, 0, static_cast<uint32_t>(windowSize.x), static_cast<uint32_t>(windowSize.y)});

	mMainStage.getScene().setIndirectLight(mSkyboxLight.get());
	mMainStage.getScene().setSkybox(mSkybox.get());

	mImGuiSceneWindow >> *this; // register imgui window resize events
}

void SceneEditorSystem::onStart()
{
	mSelectedInstance = createInstance(mMainStage, "Main Camera")
							.withPosition({6.0f, 3.0f, 6.0f})
							.asCamera()
							.withPerspectiveProjection(45.0, 19 / 6.0, .1, 1000.0)
							.add<editor::EditorCamera>(.5f, 10.0f);

	mMainStage.setMainCamera(mSelectedInstance);

	createInstance(mMainStage, "Main Light").asLight(spatial::Light::Type::POINT).withPosition({.0f});

	mDefaultMaterial->setDefaultParameter("metallic", .2f);
	mDefaultMaterial->setDefaultParameter("roughness", 0.8f);
	mDefaultMaterial->setDefaultParameter("reflectance", .1f);

	{
		createInstance(mMainStage, "Cube")
			.withPosition({.0f})
			.asMesh(mMeshes[0])
			.withShadowOptions(true, true)
			.withMaterialAt(0, mMaterialInstances[0].get())
			.get();

		mMaterialInstances[0]->setParameter("baseColor", math::float3{.4f, 0.1f, 0.1f});
	}

	{
		createInstance(mMainStage, "Plane")
			.withPosition({3.0f, -1.0f, .0f})
			.withScale({10.0f})
			.asMesh(mMeshes[1])
			.withShadowOptions(false, true)
			.withMaterialAt(0, mMaterialInstances[1].get());

		mMaterialInstances[1]->setParameter("baseColor", math::float3{.8f, .8f, .8f});
	}

	{
		createInstance(mMainStage, "Cylinder")
			.withPosition({6.0f, .0f, .0f})
			.asMesh(mMeshes[2])
			.withShadowOptions(true, true)
			.withMaterialAt(0, mMaterialInstances[2].get());

		mMaterialInstances[2]->setParameter("baseColor", math::float3{.1f, 0.4f, 0.1f});
	}

	{
		createInstance(mMainStage, "Sphere")
			.withPosition({3.0f, .0f, .0f})
			.asMesh(mMeshes[3])
			.withShadowOptions(true, true)
			.withMaterialAt(0, mMaterialInstances[3].get());

		mMaterialInstances[3]->setParameter("baseColor", math::float3{.1f, 0.1f, 0.4f});
	}

	onSceneWindowResized({1280, 720});
	mMainStage.onStart();
}

float3 SceneEditorSystem::getInputAxis()
{
	return {
		mInputState.axis(Key::W, Key::S),
		mInputState.axis(Key::D, Key::A),
		mInputState.axis(Key::Space, Key::LShift),
	};
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	auto mainCamera = handleOf(mMainStage, mMainStage.getFirstInstance<Transform, EditorCamera>());
	if (mainCamera)
	{
		auto& cameraTransform = mainCamera.get<Transform>();
		auto& cameraMovement = mainCamera.get<EditorCamera>();

		const auto keyboardDelta = delta * cameraMovement.velocity * getInputAxis();
		const auto forward = math::forwardVector(cameraTransform.getMatrix());
		const auto right = cross(forward, math::axisY);
		auto position = cameraTransform.getPosition();

		position += forward * keyboardDelta.x;
		position += right * keyboardDelta.y;
		position += math::axisY * keyboardDelta.z;
		cameraTransform.setPosition(position);

		if (mInputState.released(Key::MouseLeft))
		{
			cameraMovement.enabled = false;
			cameraMovement.justStarted = 0;
		}

		if (mInputState.combined(Key::LControl, Key::MouseLeft) || cameraMovement.startPressed)
		{
			mWindow.warpMouse(mWindow.getSize() * .5f);
			cameraMovement.enabled = true;
			cameraMovement.justStarted = 1;
		}

		if (cameraMovement.enabled && cameraMovement.justStarted <= 10)
			cameraMovement.justStarted++;

		if (cameraMovement.enabled && cameraMovement.justStarted >= 10)
		{
			const auto mouseRotation = mInputState.getMouseOffset() * math::pi * cameraMovement.sensitivity * delta;
			const auto rotation = cameraTransform.getRotation();

			cameraTransform.setRotation({
				std::clamp(rotation.x + mouseRotation.y, -math::pi, math::pi), // clamp the pitch
				rotation.y + mouseRotation.x,
				.0f // remove the roll
			});
		}
	}
}

void SceneEditorSystem::onDrawGui()
{
	// ImGui::ShowDemoWindow();
	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;
	static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar
										  | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
										  | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
										  | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

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

void SceneEditorSystem::onSceneWindowResized(const math::int2& size)
{
	auto width = static_cast<double>(size.x);
	auto height = static_cast<double>(size.y);
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

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	mMainStage.render(renderer);
}

} // namespace spatial::editor
