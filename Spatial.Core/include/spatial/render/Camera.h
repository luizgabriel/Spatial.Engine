#pragma once

#include <filament/Camera.h>
#include <filament/Frustum.h>
#include <spatial/common/Math.h>
#include <spatial/render/Entity.h>

namespace spatial
{

class Camera
{
  public:
	enum class ProjectionType {
		PERSPECTIVE,
		ORTHOGRAPHIC,
		CUSTOM
	};

	using Fov = filament::Camera::Fov;

	explicit Camera(filament::Engine& engine);
	Camera(filament::Engine& engine, utils::Entity entity);

	~Camera();

	Camera(Camera&& other) noexcept;
	Camera& operator=(Camera&& other) noexcept;

	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

	auto getProjectionType() const noexcept {
		return mType;
	}

	float getFieldOfView() const noexcept {
		return mFieldOfView;
	}

	void setNear(float near) noexcept;

	float getNear() const noexcept
	{
		return mNear;
	}

	void setAspectRatio(float aspectRatio);

	float getAspectRatio() const noexcept
	{
		return mAspectRatio;
	}

	float getFar() const noexcept
	{
		return mFar;
	}

	void setFar(float far) noexcept;

	void setFieldOfView(float fieldOfView);

	const filament::Camera* getInstance() const noexcept;

	filament::Camera* getInstance() noexcept;

	bool isValid() const noexcept {
		return !mEntity.isNull();
	}

	bool isPerspective() const noexcept {
		return mType == ProjectionType::PERSPECTIVE;
	}

	bool isOrthographic() const noexcept {
		return mType == ProjectionType::ORTHOGRAPHIC;
	}

	void setPerspectiveProjection(float fieldOfView = defaultFieldOfView, float aspectRatio = defaultAspectRatio, float near = defaultNear, float far = defaultFar) noexcept;

	void setOrthographicProjection(float left, float right, float bottom, float top, float near = defaultNear, float far = defaultFar);

	void setOrthographicProjection(float aspectRatio = defaultAspectRatio, float near = defaultNear, float far = defaultFar);

	void setCustomProjection(const math::mat4& customProjection, float near, float far) noexcept;

  private:
	constexpr static float defaultFieldOfView = 90.0f;
	constexpr static float defaultAspectRatio = 16.0f/9.0f;
	constexpr static float defaultNear = .1f;
	constexpr static float defaultFar = 100000.0f;

	filament::Engine& mEngine;
	utils::Entity mEntity;
	ProjectionType mType{ProjectionType::PERSPECTIVE};
	float mFieldOfView{defaultFieldOfView};
	float mAspectRatio{defaultAspectRatio};
	float mNear{defaultNear};
	float mFar{defaultFar};

	void recalculateProjection();
};

} // namespace spatial