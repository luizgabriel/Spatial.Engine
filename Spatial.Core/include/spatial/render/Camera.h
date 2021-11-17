#pragma once

#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/Frustum.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Transform.h>
#include <utils/Entity.h>
#include <variant>

namespace spatial::render
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

	[[nodiscard]] const filament::Camera* getInstance() const;

	filament::Camera* getInstance();

	[[nodiscard]] bool isValid() const noexcept
	{
		return !mEntity.isNull();
	}

	void lookAt(const math::float3& eye, const math::float3& center) noexcept;

	void lookAt(const math::float3& eye, const math::float3& center, const math::float3& up) noexcept;

	void setPerspectiveProjection(double fovInDegrees, double aspect, double near, double far) noexcept;

	void setOrthographicProjection(
					   double left, double right,
					   double bottom, double top,
					   double near, double far) noexcept;

	void setLensProjection(double focalLengthInMillimeters, double aspect, double near, double far) noexcept;

	void setCustomProjection(math::mat4 const& projection, double near, double far) noexcept;

	void setCustomProjection(math::mat4 const& projection, math::mat4 const& projectionForCulling,
							 double near, double far) noexcept;

	void setScaling(math::double2 scaling) noexcept;

  private:
	filament::Engine& mEngine;
	utils::Entity mEntity;

	void reset();

	utils::Entity release();
};

} // namespace spatial::render