#pragma once

#include <filament/Engine.h>

#include <spatial/common/Math.h>
#include <spatial/render/Actor.h>
#include <spatial/render/Light.h>
#include <spatial/render/Camera.h>
#include <spatial/render/Entity.h>
#include <spatial/render/Name.h>
#include <spatial/render/Resources.h>
#include <spatial/render/Transform.h>
#include <spatial/render/Renderable.h>

namespace spatial
{

class CameraActorBuilder;
class LightActorBuilder;
class RenderableActorBuilder;

class ActorBuilder
{
  protected:
	filament::Engine& mEngine;
	Actor mActor;

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

	LightActorBuilder asLight(Light::Type type = Light::Type::POINT);

	RenderableActorBuilder asRenderable(size_t primitivesCount);
};

class CameraActorBuilder : public ActorBuilder
{
  public:
	CameraActorBuilder(filament::Engine& engine, spatial::Actor actor);

	CameraActorBuilder& withPerspectiveProjection(double fieldOfView, double aspectRatio, double near, double far);

	CameraActorBuilder& withOrthographicProjection(double aspectRatio, double near, double far);

	CameraActorBuilder& withOrthographicProjection(double left, double right, double bottom, double top, double near, double far);

	CameraActorBuilder& withCustomProjection(math::mat4 projectionMatrix, double near, double far);
};

class LightActorBuilder : public ActorBuilder
{
  public:
	LightActorBuilder(filament::Engine& engine, Actor actor, Light::Type type);

	LightActorBuilder& withIntensity(float intensity);

	LightActorBuilder& withDirection(const math::float3& direction);
};

class RenderableActorBuilder : public ActorBuilder
{
  public:
	RenderableActorBuilder(filament::Engine& engine, Actor actor, size_t primitivesCount);


};

} // namespace spatial