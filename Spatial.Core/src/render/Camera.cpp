#include <spatial/render/Camera.h>

namespace spatial
{

Camera::Camera(filament::Engine& engine) : mEngine{engine}, mEntity{}, mResource{}
{
}

Camera::Camera(filament::Engine& engine, utils::Entity entity)
	: mEngine{engine}, mEntity{entity}, mResource{engine.createCamera(entity)}
{
}

Camera::~Camera()
{
	mEngine.destroyCameraComponent(mEntity);
}

Camera::Camera(Camera&& other) noexcept
	: mEngine{other.mEngine},
	  mEntity{other.mEntity},
	  mResource{std::exchange(other.mResource, nullptr)}
{
	other.mEntity.clear();
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	mEntity = other.mEntity;
	other.mEntity.clear();
	mResource = std::exchange(other.mResource, nullptr);

	return *this;
}

} // namespace spatial