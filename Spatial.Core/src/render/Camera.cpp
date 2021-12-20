#include <spatial/render/Camera.h>

namespace spatial::render
{

Camera::Camera(filament::Engine& engine) : Camera(engine, {})
{
}

Camera::Camera(filament::Engine& engine, utils::Entity entity) : mEngine{engine}, mEntity{entity}
{
	if (!entity.isNull())
		engine.createCamera(entity);
}

Camera::~Camera()
{
	reset();
}

void Camera::reset()
{
	if (isValid())
		mEngine.destroyCameraComponent(mEntity);

	mEntity = utils::Entity{};
}

Camera::Camera(Camera&& other) noexcept : mEngine{other.mEngine}, mEntity{other.release()}
{
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	reset();
	mEntity = other.release();
	return *this;
}

const filament::Camera* Camera::getInstance() const
{
	assert(isValid());
	return mEngine.getCameraComponent(mEntity);
}

filament::Camera* Camera::getInstance()
{
	assert(isValid());
	return mEngine.getCameraComponent(mEntity);
}

void Camera::lookAt(const math::float3& eye, const math::float3& center) noexcept
{
	getInstance()->lookAt(eye, center);
}

void Camera::lookAt(const math::float3& eye, const math::float3& center, const math::float3& up) noexcept
{
	getInstance()->lookAt(eye, center, up);
}

void Camera::setPerspectiveProjection(double fovInDegrees, double aspect, double near, double far) noexcept
{
	getInstance()->setProjection(fovInDegrees, aspect, near, far);
}

void Camera::setOrthographicProjection(double left, double right, double bottom, double top, double near,
									   double far) noexcept
{
	getInstance()->setProjection(filament::Camera::Projection::ORTHO, left, right, bottom, top, near, far);
}

void Camera::setLensProjection(double focalLengthInMillimeters, double aspect, double near, double far) noexcept
{
	getInstance()->setLensProjection(focalLengthInMillimeters, aspect, near, far);
}

void Camera::setCustomProjection(const math::mat4& projection, double near, double far) noexcept
{
	getInstance()->setCustomProjection(projection, near, far);
}

void Camera::setCustomProjection(const math::mat4& projection, const math::mat4& projectionForCulling, double near,
								 double far) noexcept
{
	getInstance()->setCustomProjection(projection, projectionForCulling, near, far);
}

void Camera::setScaling(math::double2 scaling) noexcept
{
	getInstance()->setScaling(scaling);
}

utils::Entity Camera::release()
{
	return std::exchange(mEntity, {});
}

} // namespace spatial::render