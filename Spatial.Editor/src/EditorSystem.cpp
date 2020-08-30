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
	  mMeshRegistry{mEngine},
	  mMaterialSystem{mEngine},

	  mTransformSystem{mEngine},
	  mRenderableSystem{mEngine, mScene.ref()},
	  mCameraSystem{mEngine},
	  mLightSystem{mEngine},

	  mCameraEntity{},

	  mMovement{true, 1.0f, 10.0f}
{
	connect<ecs::SceneEntity>(mRegistry, mRenderableSystem);
	connect<ecs::Transform>(mRegistry, mTransformSystem);
	connect<ecs::Camera>(mRegistry, mCameraSystem);
	connect<ecs::Light>(mRegistry, mLightSystem);

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
	mRegistry.emplace<ecs::Transform>(mCameraEntity, math::float3{.0f, .0f, .0f});
	mRegistry.emplace<ecs::Camera>(mCameraEntity, math::float3{.0f}, ecs::Camera::Perspective{});

	onSceneWindowResized({1280, 720});

	auto& cameraComponent = mCameraSystem.get(mCameraEntity);
	mSceneView->setCamera(&cameraComponent);

	createObject("Plane", "editor/meshes/plane.filamesh", {3.0f, -1.0f, .0f}, {.8f, .8f, .8f, 1.0f});
	createObject("Cube", "editor/meshes/cube.filamesh", {.0f, .0f, .0f}, {.4f, 0.1f, 0.1f, 1.0f});
	createObject("Cylinder", "editor/meshes/cylinder.filamesh", {3.0f, .0f, .0f}, {.1f, 0.4f, 0.1f, 1.0f});
	createObject("Sphere", "editor/meshes/sphere.filamesh", {6.0f, .0f, .0f}, {.1f, 0.1f, 0.4f, 1.0f});
	createLight("Main Light");
}

void EditorSystem::onEvent(const MouseMovedEvent&)
{
	if (mMovement.warpMouse)
	{
		constexpr auto center = float2{0.5f, 0.5f};
		const auto delta = center - Input::mouse();

		auto& transform = mRegistry.get<ecs::Transform>(mCameraEntity);
		transform.rotation.x += delta.x * pi<float> * -mMovement.sensitivity;
		transform.rotation.y = std::clamp(transform.rotation.y + delta.y * pi<float> * mMovement.sensitivity,
										  -pi<float> / 2.0f, pi<float> / 2.0f);

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
	mRenderableSystem.onUpdate(mRegistry);
	mLightSystem.onUpdate(mRegistry);
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

	mImGuiSceneWindow.draw("Scene View (Editor)");

	ImGui::Begin("Scene Graph");

	ui::sceneHierarchy(mRegistry, mSelectedEntity);

	ImGui::End();

	if (mRegistry.valid(mSelectedEntity))
	{
		ImGui::Begin("Properties");

		auto& name = mRegistry.get<ecs::Name>(mSelectedEntity);
		ui::inputText("Name", name.value);

		auto* transform = mRegistry.try_get<ecs::Transform>(mSelectedEntity);

		auto* perspectiveCamera = mRegistry.try_get<ecs::PerspectiveCamera>(mSelectedEntity);
		auto* orthographicCamera = mRegistry.try_get<ecs::OrtographicCamera>(mSelectedEntity);
		auto* customCamera = mRegistry.try_get<ecs::CustomCamera>(mSelectedEntity);

		auto* mesh = mRegistry.try_get<ecs::Renderable>(mSelectedEntity);
		auto* material = mRegistry.try_get<ecs::Material>(mSelectedEntity);
		auto* renderable = mRegistry.try_get<ecs::SceneEntity>(mSelectedEntity);

		auto* pointLight = mRegistry.try_get<ecs::PointLight>(mSelectedEntity);
		auto* directionalLight = mRegistry.try_get<ecs::DirectionalLight>(mSelectedEntity);
		auto* spotLight = mRegistry.try_get<ecs::SpotLight>(mSelectedEntity);

		if (transform)
		{
			if (ImGui::CollapsingHeader("Transform", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				if (perspectiveCamera || orthographicCamera || customCamera)
					ui::transformInput(*transform, "py");
				else if (pointLight || directionalLight || spotLight)
					ui::transformInput(*transform, "p");
				else
					ui::transformInput(*transform, "prs");

				ImGui::Unindent();
			}
		}

		if (perspectiveCamera)
		{
			if (ImGui::CollapsingHeader("Perspective Camera", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::cameraInput(*perspectiveCamera);
				ImGui::Unindent();
			}
		}

		if (orthographicCamera)
		{
			if (ImGui::CollapsingHeader("Orthographic Camera", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::cameraInput(*orthographicCamera);
				ImGui::Unindent();
			}
		}

		if (customCamera)
		{
			if (ImGui::CollapsingHeader("Custom Camera", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::cameraInput(*customCamera);
				ImGui::Unindent();
			}
		}

		if (pointLight)
		{
			if (ImGui::CollapsingHeader("Point Light", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::lightInput(*pointLight);
				ImGui::Unindent();
			}
		}

		if (directionalLight)
		{
			if (ImGui::CollapsingHeader("Directional Light", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::lightInput(*directionalLight);
				ImGui::Unindent();
			}
		}

		if (spotLight)
		{
			if (ImGui::CollapsingHeader("Spot Light", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::lightInput(*spotLight);
				ImGui::Unindent();
			}
		}

		if (renderable && !(perspectiveCamera || orthographicCamera || customCamera))
		{
			if (ImGui::CollapsingHeader("Renderable"))
			{
				ImGui::Indent();

				ImGui::Checkbox("Cast Shadows", &renderable->castShadows);
				ImGui::Checkbox("Receive Shadows", &renderable->receiveShadows);

				ImGui::Unindent();
			}
		}

		if (mSelectedEntity == mCameraEntity)
		{
			ImGui::Separator();

			ImGui::Checkbox("Warp Mouse", &mMovement.warpMouse);
			ImGui::InputFloat("Velocity", &mMovement.velocity);
			ImGui::InputFloat("Sensitivity", &mMovement.sensitivity);
		}

		ImGui::End();
	}
}

void EditorSystem::onSceneWindowResized(ui::ImGuiSceneWindow::Size size)
{
	auto width = static_cast<float>(size.first);
	auto height = static_cast<float>(size.second);
	auto aspectRatio = width / height;

	auto& camera = mRegistry.get<ecs::Camera>(mCameraEntity);
	if (camera.aspectRatio())
	{
		*camera.aspectRatio() = aspectRatio;
	}
}

EditorSystem::EditorSystem(RenderingSystem& renderingSystem) : EditorSystem(renderingSystem.getEngine())
{
	renderingSystem.pushBackView(mSceneView);
}

entt::entity EditorSystem::createLight(std::string name)
{
	auto entity = mRegistry.create();
	mRegistry.emplace<ecs::Name>(entity, std::move(name));
	mRegistry.emplace<ecs::Transform>(entity, math::float3{.0f, 10.0f, .0f});
	mRegistry.emplace<ecs::DirectionalLight>(entity);

	return entity;
}

entt::entity EditorSystem::createObject(std::string name, const std::string_view shape, math::float3 position,
										math::float4 color)
{
	auto entity = mRegistry.create();
	mRegistry.emplace<ecs::Name>(entity, std::move(name));
	mRegistry.emplace<ecs::Transform>(entity, std::move(position));

	auto materialId = mMaterialRegistry.load("editor/materials/default.filamat", [=](auto& instance) {
		instance.setParameter("baseColor", color);
		instance.setParameter("metallic", .2f);
		instance.setParameter("roughness", 0.3f);
		instance.setParameter("reflectance", .1f);
	});

	mRegistry.emplace<ecs::Renderable>(entity, ecs::Renderable{.meshId = mMeshRegistry.load(shape),
															   .materialId = materialId,
															   .castShadows = true,
															   .receiveShadows = true});

	return entity;
}

} // namespace spatial
