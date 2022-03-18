#pragma once

#include <algorithm>
#include <filament/Engine.h>
#include <spatial/common/Signal.h>
#include <spatial/ecs/RegistryCollection.h>
#include <spatial/render/CameraController.h>
#include <spatial/render/IndirectLightController.h>
#include <spatial/render/LightController.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/MeshController.h>
#include <spatial/render/SceneController.h>
#include <spatial/render/TransformController.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/script/PlatformContext.h>

namespace spatial::render
{

class RegistryRenderingSystem
{
  public:
	RegistryRenderingSystem(filament::Engine& engine, FileSystem& fileSystem);

	void onStartFrame(float delta);

	void onUpdateFrame(float delta);

	void onRender(filament::Renderer& renderer) const;

	auto& getOnPublishRegistrySignal()
	{
		return mOnPublishRegistry;
	}

  private:
	filament::Engine& mEngine;

	Signal<ecs::RegistryCollection&> mOnPublishRegistry;

	render::SceneController mSceneController;
	render::TransformController mTransformController;
	render::CameraController mCameraController;
	render::LightController mLightController;
	render::MeshController mMeshController;
	render::IndirectLightController mIndirectLightController;
	render::MaterialController mMaterialController;

	[[nodiscard]] ecs::RegistryCollection getPublishedRegistries() const;
};

} // namespace spatial::render