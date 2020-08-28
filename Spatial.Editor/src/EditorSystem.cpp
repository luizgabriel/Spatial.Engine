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
	  mTransformSystem{mEngine},
	  mMeshSystem{mEngine, assets::sResourceLoader},
	  mMaterialSystem{mEngine, assets::sResourceLoader},
	  mRenderableSystem{mEngine, mScene.ref()},
	  mCameraSystem{mEngine},
	  mLightSystem{mEngine},

	  mCameraEntity{},

	  mMovement{true, 1.0f, 10.0f}
{
	connect<ecs::Mesh>(mRegistry, mMeshSystem);
	connect<ecs::Renderable>(mRegistry, mRenderableSystem);
	connect<ecs::Transform>(mRegistry, mTransformSystem);
	connect<ecs::Material>(mRegistry, mMaterialSystem);
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

	auto planeEntity = createObject("Plane", "editor/meshes/plane.filamesh", {3.0f, -1.0f, .0f}, {10.0f, .0f, 10.0f});
	auto cubeEntity = createObject("Cube", "editor/meshes/cube.filamesh", {.0f, .0f, .0f}, {1.0f});
	auto cylinderEntity = createObject("Cylinder", "editor/meshes/cylinder.filamesh", {3.0f, .0f, .0f}, {1.0f});
	auto sphereEntity = createObject("Sphere", "editor/meshes/sphere.filamesh", {6.0f, .0f, .0f}, {1.0f});
	createLight("Main Light");

	mRenderableSystem.buildMaterialInstances(mRegistry, mMaterialSystem);
	mRenderableSystem.buildShapeRenderables(mRegistry, mMeshSystem);

	mRenderableSystem.update(planeEntity, [](auto* materialInstance) {
		materialInstance->setParameter("baseColor", math::float4{1.0f, 1.0f, 1.0f, 1.0f});
		materialInstance->setParameter("metallic", .0f);
		materialInstance->setParameter("roughness", 1.0f);
		materialInstance->setParameter("reflectance", .2f);
	});

	mRenderableSystem.update(sphereEntity, [](auto* materialInstance) {
		materialInstance->setParameter("baseColor", math::float4{0.2f, 0.5f, 0.3f, 1.0f});
		materialInstance->setParameter("metallic", .5f);
		materialInstance->setParameter("roughness", 0.2f);
		materialInstance->setParameter("reflectance", .0f);
	});

	mRenderableSystem.update(cubeEntity, [](auto* materialInstance) {
		materialInstance->setParameter("baseColor", math::float4{0.5f, 0.2f, 0.3f, 1.0f});
		materialInstance->setParameter("metallic", .5f);
		materialInstance->setParameter("roughness", 0.2f);
		materialInstance->setParameter("reflectance", .0f);
	});

	mRenderableSystem.update(cylinderEntity, [](auto* materialInstance) {
		materialInstance->setParameter("baseColor", math::float4{0.3f, 0.3f, 0.8f, 1.0f});
		materialInstance->setParameter("metallic", .5f);
		materialInstance->setParameter("roughness", 0.2f);
		materialInstance->setParameter("reflectance", .0f);
	});
}

entt::entity EditorSystem::createObject(std::string name, std::string resourcePath, math::float3 position,
										math::float3 scale)
{
	auto objectEntity = mRegistry.create();

	mRegistry.emplace<ecs::Name>(objectEntity, std::move(name));
	mRegistry.emplace<ecs::Transform>(objectEntity, std::move(position), std::move(scale));
	mRegistry.emplace<ecs::Mesh>(objectEntity, std::move(resourcePath));

	auto materialEntity = mRegistry.create();
	mRegistry.emplace<ecs::Material>(materialEntity, objectEntity, "editor/materials/default.filamat");

	return objectEntity;
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
		auto* camera = mRegistry.try_get<ecs::Camera>(mSelectedEntity);
		auto* mesh = mRegistry.try_get<ecs::Mesh>(mSelectedEntity);
		auto* material = mRegistry.try_get<ecs::Material>(mSelectedEntity);
		auto* renderable = mRegistry.try_get<ecs::Renderable>(mSelectedEntity);
		auto* light = mRegistry.try_get<ecs::Light>(mSelectedEntity);

		if (transform)
		{
			if (ImGui::CollapsingHeader("Transform", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Indent();
				if (camera)
					ui::transformInput(*transform, "py");
				else if (light)
					ui::transformInput(*transform, "p");
				else
					ui::transformInput(*transform, "prs");

				ImGui::Unindent();
			}
		}

		if (mesh)
		{
			if (ImGui::CollapsingHeader("Mesh"))
			{
				ImGui::Indent();
				ui::inputText("Resource", mesh->resourceFilePath);
				ImGui::Unindent();
			}
		}

		if (material)
		{
			if (ImGui::CollapsingHeader("Material"))
			{
				ImGui::Indent();
				ui::inputText("Resource", material->resourceFilePath);
				ImGui::Unindent();
			}
		}

		if (camera)
		{
			if (ImGui::CollapsingHeader("Camera", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				ui::cameraInput(*camera);
				ImGui::Unindent();
			}
		}

		if (light)
		{
			if (ImGui::CollapsingHeader("Light", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();

				ui::lightInput(*light);

				ImGui::Unindent();
			}
		}

		if (renderable && !camera)
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
	mRegistry.emplace<ecs::Light>(entity, ecs::Light{
		.type = ecs::Light::SpotLight{}
	});

	return entity;
}

} // namespace spatial
