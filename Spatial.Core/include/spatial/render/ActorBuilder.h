#pragma once

#include <filament/Engine.h>

#include <spatial/common/Math.h>
#include <spatial/render/Actor.h>
#include <spatial/render/Light.h>

namespace spatial
{

class CameraActorBuilder;
class LightActorBuilder;

class ActorBuilder
{
  protected:
	filament::Engine& mEngine;
	spatial::Actor mActor;

  public:
	ActorBuilder(filament::Engine& engine, entt::registry& registry);

	ActorBuilder(filament::Engine& engine, Actor actor);

	ActorBuilder(const ActorBuilder& other);
	ActorBuilder& operator=(const ActorBuilder& other);

	ActorBuilder(ActorBuilder&& other) noexcept = delete;
	ActorBuilder& operator=(ActorBuilder&& other) noexcept = delete;

	ActorBuilder& withPosition(math::float3 position);

	ActorBuilder& withRotation(const math::float3& rotation);

	ActorBuilder& withScale(const math::float3& scale);

	template <typename Component, typename... Args>
	ActorBuilder& add(Args&&... args)
	{
		mActor.addComponent<Component>(std::forward<Args>(args)...);
		return *this;
	}

	Actor build() noexcept
	{
		return mActor;
	}

	operator Actor() noexcept
	{
		return build();
	}

	CameraActorBuilder asCamera();

	LightActorBuilder asLight(filament::LightManager::Type type = filament::LightManager::Type::POINT);
};

class CameraActorBuilder : public ActorBuilder
{
  public:
	CameraActorBuilder(filament::Engine& engine, spatial::Actor actor);

	CameraActorBuilder& withPerspectiveProjection(float fieldOfView = 45.f, float aspectRatio = 16.0f / 9.0f,
												  float near = 0.1f, float far = 1000000.0f);

	CameraActorBuilder& withOrthographicProjection(float aspectRatio = 16.0f / 9.0f, float near = 0.1f,
												   float far = 1000000.0f);
};

class LightActorBuilder : public ActorBuilder
{
  public:
	LightActorBuilder(filament::Engine& engine, const Actor actor, spatial::Light::Type type);

	LightActorBuilder& withIntensity(float intensity);

	LightActorBuilder& withDirection(const math::float3& direction);
};

} // namespace spatial