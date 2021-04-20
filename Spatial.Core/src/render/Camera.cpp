#include <spatial/render/Camera.h>

namespace spatial
{

Camera::Camera(filament::Engine& engine) : Camera(engine, {})
{
}

Camera::Camera(filament::Engine& engine, utils::Entity entity)
	: mEngine{engine}, mEntity{entity}, mProjection{PerspectiveProjection{45.0, 19.0 / 6.0, .1, 10000.0}}
{
	if (!entity.isNull())
		engine.createCamera(entity);
}

Camera::~Camera()
{
	if (isValid())
		mEngine.destroyCameraComponent(mEntity);
}

Camera::Camera(Camera&& other) noexcept
	: mEngine{other.mEngine}, mEntity{std::exchange(other.mEntity, {})}, mProjection{std::move(other.mProjection)}
{
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	mEntity = std::exchange(other.mEntity, {});
	mProjection = std::move(other.mProjection);
	return *this;
}

const filament::Camera* Camera::getInstance() const noexcept
{
	return mEngine.getCameraComponent(mEntity);
}

filament::Camera* Camera::getInstance() noexcept
{
	return mEngine.getCameraComponent(mEntity);
}

void Camera::setProjection(Projection projection) noexcept
{
	mProjection = std::move(projection);

	std::visit(
		[this](const auto& projection) {
			using T = std::decay_t<decltype(projection)>;

			if constexpr (std::is_same_v<T, PerspectiveProjection>)
			{
				getInstance()->setProjection(projection.fieldOfView, projection.aspectRatio, projection.near,
											 projection.far);
			}
			else if constexpr (std::is_same_v<T, OrthographicProjection>)
			{
				getInstance()->setProjection(filament::Camera::Projection::ORTHO, projection.left, projection.right,
											 projection.bottom, projection.top, projection.near, projection.far);
			}
			else if constexpr (std::is_same_v<T, CustomProjection>)
			{
				getInstance()->setCustomProjection(projection.projectionMatrix, projection.near, projection.far);
			}
		},
		mProjection);
}

bool Camera::isPerspective() const noexcept
{
	return std::holds_alternative<PerspectiveProjection>(mProjection);
}

bool Camera::isOrthographic() const noexcept
{
	return std::holds_alternative<OrthographicProjection>(mProjection);
}

bool Camera::isCustomProjection() const noexcept
{
	return std::holds_alternative<CustomProjection>(mProjection);
}

} // namespace spatial