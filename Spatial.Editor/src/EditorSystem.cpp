#include "EditorSystem.h"

#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ui/ImGuiComponents.h>

#include <variant>

#include "Components.h"
#include "Resources.h"

namespace fl = filament;
using namespace filament::math;

template <typename ValueType>
constexpr ValueType pi = static_cast<ValueType>(3.14159265359L);

template <typename ValueType>
constexpr ValueType halfPi = static_cast<ValueType>(1.57079632679L);

constexpr auto up = float3{.0f, 1.0f, .0f};

namespace spatial
{

EditorSystem::EditorSystem(fl::Engine& engine)
	: mEngine{engine},
	  mSceneView{toShared(createView(mEngine))},

	  mScene{createScene(mEngine)},

	  mIblTexture{createKtxTexture(mEngine, assets::loadResource("editor/textures/default_skybox/ibl.ktx").value())},
	  mSkyboxTexture{
		  createKtxTexture(mEngine, assets::loadResource("editor/textures/default_skybox/skybox.ktx").value())},
	  mSkyboxLight{createImageBasedLight(mEngine, mIblTexture.ref(),
										 assets::loadResource("editor/textures/default_skybox/sh.txt").value())},
	  mSkybox{createSkybox(mEngine, mSkyboxTexture.ref())},

	  mImGuiSceneWindow{mEngine, {1280, 720}},

	  mRegistry{},
	  mTransformSystem{mEngine},
	  mMeshSystem{mEngine, assets::sResourceLoader},
	  mMaterialSystem{mEngine, assets::sResourceLoader},
	  mRenderableSystem{mEngine, mScene.ref()},
	  mCameraSystem{mEngine},

	  mCameraEntity{},

	  mMovement{true, 1.0f, 100.0f}
{
	connect<ecs::Mesh>(mRegistry, mMeshSystem);
	connect<ecs::Renderable>(mRegistry, mRenderableSystem);
	connect<ecs::Transform>(mRegistry, mTransformSystem);
	connect<ecs::Material>(mRegistry, mMaterialSystem);
	connect<ecs::Camera>(mRegistry, mCameraSystem);

	mSceneView->setRenderTarget(mImGuiSceneWindow.getRenderTarget());
	mSceneView->setScene(mScene.get());
	mSceneView->setBlendMode(fl::View::BlendMode::OPAQUE);

	mScene->setIndirectLight(mSkyboxLight.get());
	mScene->setSkybox(mSkybox.get());

	mSceneView->setViewport({0, 0, 1280, 720});

	mImGuiSceneWindow >> *this; // register imgui window resize events
}

void EditorSystem::onStart()
{
	mCameraEntity = mRegistry.create();
	mRegistry.emplace<ecs::Name>(mCameraEntity, "Main Camera");
	mRegistry.emplace<ecs::Transform>(mCameraEntity);
	mRegistry.emplace<ecs::Camera>(mCameraEntity);

	onSceneWindowResized({1280, 720});

	auto& cameraComponent = mCameraSystem.get(mCameraEntity);
	mSceneView->setCamera(&cameraComponent);

	auto cubeEntity = mRegistry.create();
	mRegistry.emplace<ecs::Name>(cubeEntity, "Cube");
	mRegistry.emplace<ecs::Transform>(cubeEntity);
	mRegistry.emplace<ecs::Mesh>(cubeEntity, "editor/meshes/cube.filamesh");

	auto materialEntity = mRegistry.create();
	mRegistry.emplace<ecs::Material>(materialEntity, cubeEntity, "editor/materials/default.filamat");

	mRenderableSystem.buildMaterialInstances(mRegistry, mMaterialSystem);
	mRenderableSystem.buildShapeRenderables(mRegistry, mMeshSystem);

	mRenderableSystem.update(cubeEntity, [](auto* materialInstance) {
		materialInstance->setParameter("baseColor", fl::math::float4{1.0f, 1.0f, 1.0f, 1.0f});
		materialInstance->setParameter("metallic", .1f);
		materialInstance->setParameter("roughness", .5f);
		materialInstance->setParameter("reflectance", .1f);
	});
}

void EditorSystem::onEvent(const MouseMovedEvent&)
{
	if (mMovement.warpMouse)
	{
		constexpr auto center = float2{0.5f, 0.5f};
		const auto delta = center - Input::mouse();

		auto& transform = mRegistry.get<ecs::Transform>(mCameraEntity);
		transform.rotation.x += delta.x * pi<float> * -mMovement.sensitivity;
		transform.rotation.y = std::clamp(transform.rotation.y + delta.y * pi<float> * mMovement.sensitivity, -halfPi<float>, halfPi<float>);

		Input::warpMouse({.5f, .5f});
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

void EditorSystem::onUpdateFrame(float delta)
{
	if (Input::released(Key::MouseLeft))
		mMovement.warpMouse = false;

	if (Input::combined(Key::LControl, Key::MouseLeft))
		mMovement.warpMouse = true;

	if (Input::released(Key::G))
		showEngineGui = !showEngineGui;

	mTransformSystem.onUpdate(mRegistry);

	if (!mMovement.warpMouse)
		delta = 0;

	const auto deltaMouseMovement = delta * mMovement.velocity * defaultInputAxis();
	auto& cameraTransform = mRegistry.get<ecs::Transform>(mCameraEntity);

	auto& rot = cameraTransform.rotation;
	const auto direction = normalize(float3{cos(rot.x) * cos(rot.y), sin(rot.y), sin(rot.x) * cos(rot.y)});
	cameraTransform.position += direction * deltaMouseMovement.x;
	cameraTransform.position += cross(direction, up) * deltaMouseMovement.y;
	cameraTransform.position += up * deltaMouseMovement.z;

	auto& camera = mRegistry.get<ecs::Camera>(mCameraEntity);
	camera.target = cameraTransform.position + direction;

	mTransformSystem.onUpdate(mRegistry);
	mCameraSystem.onUpdate(mRegistry);
}

void EditorSystem::onDrawGui()
{
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
	ImGuiID dockSpaceId = ImGui::GetID("SpatialDockSpace");
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

	mImGuiSceneWindow.draw("Scene View (Editor)");

	ImGui::Begin("Main Camera");

	auto& transform = mRegistry.get<ecs::Transform>(mCameraEntity);
	ui::transformInput(transform, "pr");

	ImGui::Separator();

	ImGui::Checkbox("Warp Mouse", &mMovement.warpMouse);
	ImGui::InputFloat("Velocity", &mMovement.velocity);
	ImGui::InputFloat("Sensitivity", &mMovement.sensitivity);

	ImGui::Separator();

	auto& camera = mRegistry.get<ecs::Camera>(mCameraEntity);
	ui::cameraInput(camera);

	ImGui::End();
}

void EditorSystem::onSceneWindowResized(ui::ImGuiSceneWindow::Size size)
{
	auto width = static_cast<float>(size.first);
	auto height = static_cast<float>(size.second);

	auto& camera = mRegistry.get<ecs::Camera>(mCameraEntity);
	if (auto* proj = std::get_if<ecs::Camera::Perspective>(&camera.projection)) {
		proj->aspectRatio = width / height;
	}
}

EditorSystem::EditorSystem(RenderingSystem& renderingSystem) : EditorSystem(renderingSystem.getEngine())
{
	renderingSystem.pushBackView(mSceneView);
}

} // namespace spatial
