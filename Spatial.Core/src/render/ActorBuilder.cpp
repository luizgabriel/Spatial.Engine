#include <spatial/render/ActorBuilder.h>
#include <spatial/render/Camera.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Light.h>
#include <spatial/render/Name.h>
#include <spatial/render/Resources.h>
#include <spatial/render/Transform.h>

namespace spatial
{

ActorBuilder::ActorBuilder(filament::Engine& engine, Actor actor) : mEngine{engine}, mActor{actor}
{
	auto& sceneEntity = mActor.getOrAddComponent<spatial::Entity>(mEngine, utils::EntityManager::get().create());
	mActor.getOrAddComponent<spatial::Transform>(mEngine, sceneEntity.get());
}

ActorBuilder::ActorBuilder(filament::Engine& engine, entt::registry& registry) : ActorBuilder(engine, Actor{&registry})
{
}

ActorBuilder::ActorBuilder(const ActorBuilder& other) : ActorBuilder(other.mEngine, other.mActor)
{
}

ActorBuilder& ActorBuilder::operator=(const ActorBuilder& other)
{
	assert(&mEngine == &other.mEngine);
	mActor = other.mActor;
	return *this;
}

ActorBuilder& ActorBuilder::withPosition(math::float3 position)
{
	auto& transform = mActor.getComponent<spatial::Transform>();
	transform.setPosition(position);

	return *this;
}

ActorBuilder& ActorBuilder::withRotation(const math::float3& rotation)
{
	auto& transform = mActor.getComponent<spatial::Transform>();
	transform.setRotation(rotation);

	return *this;
}

ActorBuilder& ActorBuilder::withScale(const math::float3& scale)
{
	auto& transform = mActor.getComponent<spatial::Transform>();
	transform.setScale(scale);

	return *this;
}

CameraActorBuilder ActorBuilder::asCamera()
{
	return CameraActorBuilder{mEngine, mActor};
}

LightActorBuilder ActorBuilder::asLight(filament::LightManager::Type type)
{
	return LightActorBuilder{mEngine, mActor, type};
}


CameraActorBuilder::CameraActorBuilder(filament::Engine& engine, Actor actor) : ActorBuilder(engine, actor)
{
	auto& sceneEntity = mActor.getComponent<spatial::Entity>();
	mActor.getOrAddComponent<spatial::Camera>(mEngine, sceneEntity.get());
}

CameraActorBuilder& CameraActorBuilder::withPerspectiveProjection(float fieldOfView, float aspectRatio, float near,
																  float far)
{
	auto& camera = mActor.getComponent<spatial::Camera>();
	camera.setPerspectiveProjection(fieldOfView, aspectRatio, near, far);

	return *this;
}

CameraActorBuilder& CameraActorBuilder::withOrthographicProjection(float aspectRatio, float near, float far)
{
	auto& camera = mActor.getComponent<spatial::Camera>();
	camera.setOrthographicProjection(aspectRatio, near, far);

	return *this;
}

LightActorBuilder& LightActorBuilder::withIntensity(float intensity)
{
	auto& light = mActor.getComponent<spatial::Light>();
	light.setIntensity(intensity);

	return *this;
}

LightActorBuilder::LightActorBuilder(filament::Engine& engine, Actor actor, filament::LightManager::Type type)
	: ActorBuilder(engine, actor)
{
	auto& sceneEntity = mActor.getComponent<spatial::Entity>();
	mActor.getOrAddComponent<spatial::Light>(engine, sceneEntity.get(), type);
}

LightActorBuilder& LightActorBuilder::withDirection(const math::float3& direction)
{
	auto& component = mActor.getComponent<spatial::Light>();
	component.setDirection(direction);

	return *this;
}

} // namespace spatial