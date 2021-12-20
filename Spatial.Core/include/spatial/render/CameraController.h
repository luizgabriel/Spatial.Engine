#pragma once

#include "TextureView.h"
#include <filament/Engine.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Camera.h>
#include <spatial/render/Entity.h>

namespace spatial::render
{

class CameraController
{
  public:
	explicit CameraController(filament::Engine& engine);

	void onUpdateFrame(ecs::Registry& registry) const;

  private:
	filament::Engine& mEngine;

	void update(const ecs::PerspectiveCamera& data, Camera& camera) const;

	void update(const ecs::OrthographicCamera& data, Camera& camera) const;

	void update(const ecs::CustomCamera& data, Camera& camera) const;
	void createCameras(ecs::Registry& registry) const;
	void updateCameras(ecs::Registry& registry) const;
	void deleteCameras(ecs::Registry& registry) const;
};

} // namespace spatial::render