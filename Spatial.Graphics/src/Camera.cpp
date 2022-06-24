#include <spatial/graphics/Camera.h>
#include <spatial/graphics/MathConversionUtils.h>

namespace spatial::graphics
{

Camera::Camera(filament::Engine& engine, utils::Entity entity) : mEngine{engine}, mEntity{entity}
{
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

void Camera::lookAt(const math::vec3& eye, const math::vec3& center) noexcept
{
	getInstance()->lookAt(toFilament(eye), toFilament(center));
}

void Camera::lookAt(const math::vec3& eye, const math::vec3& center, const math::vec3& up) noexcept
{
	getInstance()->lookAt(toFilament(eye), toFilament(center), toFilament(up));
}

void Camera::setPerspectiveProjection(double fov, double aspect, double near, double far) noexcept
{
	getInstance()->setProjection(fov * math::rad2deg_v<double>, aspect, near, far);
}

void Camera::setOrthographicProjection(double left, double right, double bottom, double top, double near,
									   double far) noexcept
{
	getInstance()->setProjection(filament::Camera::Projection::ORTHO, left, right, bottom, top, near, far);
}

void Camera::setLensProjection(double focalLengthInMillimeters, double aspect, double near, double far) noexcept
{
	getInstance()->setLensProjection(focalLengthInMillimeters, aspect,
									 near, far);
}

void Camera::setCustomProjection(const math::dmat4& projection, double near, double far) noexcept
{
	getInstance()->setCustomProjection(toFilament(projection), near, far);
}

void Camera::setCustomProjection(const math::dmat4& projection, const math::dmat4& projectionForCulling, double near,
								 double far) noexcept
{
	getInstance()->setCustomProjection(toFilament(projection), toFilament(projectionForCulling), near, far);
}

void Camera::setScaling(math::dvec2 scaling) noexcept
{
	getInstance()->setScaling(toFilament(scaling));
}

utils::Entity Camera::release()
{
	return std::exchange(mEntity, {});
}

} // namespace spatial::graphics