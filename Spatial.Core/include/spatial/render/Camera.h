#pragma once

#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/Frustum.h>
#include <spatial/common/Math.h>
#include <utils/Entity.h>
#include <variant>

namespace spatial
{

struct CameraProjection
{
	double near;
	double far;

	constexpr CameraProjection(double near, double far) : near{near}, far{far}
	{
	}
};

struct PerspectiveProjection : public CameraProjection
{
	double fieldOfView;
	double aspectRatio;

	constexpr PerspectiveProjection(double fieldOfView, double aspectRatio, double near, double far)
		: CameraProjection(near, far), fieldOfView{fieldOfView}, aspectRatio{aspectRatio}
	{
	}
};

struct OrthographicProjection : public CameraProjection
{
	double left;
	double right;
	double bottom;
	double top;

	constexpr OrthographicProjection(double left, double right, double bottom, double top, double near, double far)
		: CameraProjection(near, far), left{left}, right{right}, bottom{bottom}, top{top}
	{
	}

	constexpr OrthographicProjection(double aspectRatio, double near, double far)
		: OrthographicProjection(-aspectRatio, aspectRatio, -1, 1, near, far)
	{
	}
};

struct CustomProjection : public CameraProjection
{
	math::mat4 projectionMatrix;

	constexpr CustomProjection(math::mat4 projectionMatrix, double near, double far)
		: CameraProjection(near, far), projectionMatrix{std::move(projectionMatrix)}
	{
	}
};

class CameraTarget
{
  public:
	CameraTarget() : mTarget{.0f}
	{
	}

	CameraTarget(math::float3 target) : mTarget{std::move(target)}
	{
	}

	const math::float3& getTarget() const noexcept
	{
		return mTarget;
	}

	void setTarget(const math::float3& target) noexcept
	{
		mTarget = target;
	}

  private:
	math::float3 mTarget{.0f};
};

class Camera
{
  public:
	using Projection = std::variant<PerspectiveProjection, OrthographicProjection, CustomProjection>;

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

	bool isPerspective() const noexcept;

	bool isOrthographic() const noexcept;

	bool isCustomProjection() const noexcept;

	void setProjection(Projection projection) noexcept;

	const Projection& getProjection() const noexcept
	{
		return mProjection;
	}

  private:
	filament::Engine& mEngine;
	utils::Entity mEntity;
	Projection mProjection;
};

} // namespace spatial