#include <spatial/render/Camera.h>

namespace spatial
{

Camera::Camera(filament::Engine& engine)
	: mEngine{engine}, mEntity{}, mResource{}
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

}