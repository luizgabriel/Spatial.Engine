#pragma once

#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/Frustum.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Components.h>
#include <utils/Entity.h>
#include <variant>

namespace spatial
{


class Camera
{
  public:
	explicit Camera(filament::Engine& engine);
	Camera(filament::Engine& engine, utils::Entity entity);

	~Camera();

	Camera(Camera&& other) noexcept;
	Camera& operator=(Camera&& other) noexcept;

	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

	const filament::Camera* getInstance() const noexcept;

	filament::Camera* getInstance() noexcept;

	bool isValid() const noexcept
	{
		return !mEntity.isNull();
	}

	void lookAt(const math::float3& eye, const math::float3& center) noexcept;

	void lookAt(const math::float3& eye, const math::float3& center, const math::float3& up) noexcept;

	bool isPerspective() const noexcept;

	bool isOrthographic() const noexcept;

	bool isCustomProjection() const noexcept;

	void setProjection(CameraProjection projection) noexcept;

	const CameraProjection& getProjection() const noexcept
	{
		return mProjection;
	}

  private:
	filament::Engine& mEngine;
	utils::Entity mEntity;
	CameraProjection mProjection;
};

CameraComponent toComponent(const Camera& camera);

Camera fromComponent(const CameraComponent& component, filament::Engine& engine, utils::Entity entity);

} // namespace spatial