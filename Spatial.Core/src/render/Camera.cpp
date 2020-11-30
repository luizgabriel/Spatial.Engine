#include <spatial/render/Camera.h>

namespace spatial
{

Camera::Camera(filament::Engine& engine) : mEngine{engine}, mEntity{}
{
}

Camera::Camera(filament::Engine& engine, utils::Entity entity)
	: mEngine{engine}, mEntity{entity}
{
	engine.createCamera(entity);
}

Camera::~Camera()
{
	if (isValid())
		mEngine.destroyCameraComponent(mEntity);
}

Camera::Camera(Camera&& other) noexcept
	: mEngine{other.mEngine},
	  mEntity{std::exchange(other.mEntity, {})}
{
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	mEntity = std::exchange(other.mEntity, {});
	return *this;
}

void Camera::setNear(float near) noexcept
{
	mNear = near;
	recalculateProjection();
}

void Camera::setAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
	recalculateProjection();
}

const filament::Camera* Camera::getInstance() const noexcept
{
	assert(isValid());
	return mEngine.getCameraComponent(mEntity);
}

filament::Camera* Camera::getInstance() noexcept
{
	assert(isValid());
	return mEngine.getCameraComponent(mEntity);
}

void Camera::recalculateProjection()
{
	if (isPerspective()) {
		getInstance()->setProjection(getFieldOfView(), getAspectRatio(), getNear(), getFar());
	} else if (isOrthographic()) {
		getInstance()->setProjection(filament::Camera::Projection::ORTHO, -getAspectRatio(), getAspectRatio(), -1, 1, getNear(), getFar());
	}
}

void Camera::setFar(float far) noexcept
{
	mFar = far;
	recalculateProjection();
}

void Camera::setFieldOfView(float fieldOfView)
{
	assert(isPerspective());
	mFieldOfView = fieldOfView;
	recalculateProjection();
}

void Camera::setCustomProjection(const math::mat4& customProjection, float near, float far) noexcept
{
	mType = ProjectionType::CUSTOM;
	mNear = near;
	mFar = far;

	getInstance()->setCustomProjection(customProjection, near, far);
}

void Camera::setPerspectiveProjection(float fieldOfView, float aspectRatio, float near, float far) noexcept
{
	mType = ProjectionType::PERSPECTIVE;
	mFieldOfView = fieldOfView;
	mAspectRatio = aspectRatio;
	mNear = near;
	mFar = far;

	getInstance()->setProjection(fieldOfView, aspectRatio, near, far);
}

void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	mType = ProjectionType::ORTHOGRAPHIC;
	mNear = near;
	mFar = far;

	getInstance()->setProjection(filament::Camera::Projection::ORTHO, left, right, bottom, top, near, far);
}

void Camera::setOrthographicProjection(float aspectRatio, float near, float far)
{
	mAspectRatio = aspectRatio;
	setOrthographicProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f, near, far);
}

} // namespace spatial