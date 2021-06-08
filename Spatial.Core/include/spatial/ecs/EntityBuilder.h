#pragma once

#include "Light.h"
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>
#include <spatial/render/Transform.h>

namespace spatial::ecs
{

class TransformEntityBuilder;

class PerspectiveCameraEntityBuilder;
class OrthographicCameraEntityBuilder;
class CustomCameraEntityBuilder;

class PointLightEntityBuilder;
class SpotLightEntityBuilder;
class DirectionalLightEntityBuilder;
class SunLightEntityBuilder;

class EntityBuilder
{
  public:
	EntityBuilder(Registry& registry);

	EntityBuilder(Registry& registry, Entity entity);

	EntityBuilder& withName(std::string name);

	TransformEntityBuilder asTransform();

	PerspectiveCameraEntityBuilder asPerspectiveCamera();
	OrthographicCameraEntityBuilder asOrthographicCamera();
	CustomCameraEntityBuilder asCustomCamera();

	PointLightEntityBuilder asPointLight();
	SpotLightEntityBuilder asSpotLight();
	DirectionalLightEntityBuilder asDirectionalLight();
	SunLightEntityBuilder asSunLight();

	Entity build()
	{
		return mEntity;
	}

	operator Entity()
	{
		return mEntity;
	}

  protected:
	Registry& mRegistry;
	Entity mEntity;
};

class TransformEntityBuilder : public EntityBuilder
{
  public:
	TransformEntityBuilder(Registry& registry, Entity entity);

	TransformEntityBuilder& withPosition(math::float3 position);
	TransformEntityBuilder& withScale(math::float3 scale);
	TransformEntityBuilder& withRotation(math::float3 rotation);

  private:
	Transform& getComponent();
};

class PerspectiveCameraEntityBuilder : public EntityBuilder
{
  public:
	PerspectiveCameraEntityBuilder(Registry& registry, Entity entity);

	PerspectiveCameraEntityBuilder& withClippingPlanes(double near, double far);
	PerspectiveCameraEntityBuilder& withFieldOfView(double fieldOfView);
	PerspectiveCameraEntityBuilder& withAspectRatio(double aspectRatio);

  private:
	PerspectiveCamera& getComponent();
};

class OrthographicCameraEntityBuilder : public EntityBuilder
{
  public:
	OrthographicCameraEntityBuilder(Registry& registry, Entity entity);

	OrthographicCameraEntityBuilder& withClippingPlanes(double near, double far);
	OrthographicCameraEntityBuilder& withProjection(double left, double right, double bottom, double top);

  private:
	OrthographicCamera& getComponent();
};

class CustomCameraEntityBuilder : public EntityBuilder
{
  public:
	CustomCameraEntityBuilder(Registry& registry, Entity entity);

	CustomCameraEntityBuilder& withClippingPlanes(double near, double far);
	CustomCameraEntityBuilder& withProjection(math::mat4 projectionMatrix);

  private:
	CustomCamera& getComponent();
};

class PointLightEntityBuilder : public EntityBuilder
{
  public:
	PointLightEntityBuilder(Registry& registry, Entity entity);

	PointLightEntityBuilder& withIntensity(float intensity);
	PointLightEntityBuilder& withColor(math::float3 color);
	PointLightEntityBuilder& withFalloff(float falloff);

  private:
	PointLight& getComponent();
};

class SpotLightEntityBuilder : public EntityBuilder
{
  public:
	SpotLightEntityBuilder(Registry& registry, Entity entity);

	SpotLightEntityBuilder& withIntensity(float intensity);
	SpotLightEntityBuilder& withColor(math::float3 color);
	SpotLightEntityBuilder& withDirection(math::float3 direction);
	SpotLightEntityBuilder& withFalloff(float falloff);
	SpotLightEntityBuilder& withAngles(float innerAngle, float outerAngle);
	SpotLightEntityBuilder& withCastShadows(bool castShadows);

  private:
	SpotLight& getComponent();
};

class DirectionalLightEntityBuilder : public EntityBuilder
{
  public:
	DirectionalLightEntityBuilder(Registry& registry, Entity entity);

	DirectionalLightEntityBuilder& withIntensity(float intensity);
	DirectionalLightEntityBuilder& withColor(math::float3 color);
	DirectionalLightEntityBuilder& withDirection(math::float3 direction);
	DirectionalLightEntityBuilder& withCastShadows(bool castShadows);

  private:
	DirectionalLight& getComponent();
};

class SunLightEntityBuilder : public EntityBuilder
{
  public:
	SunLightEntityBuilder(Registry& registry, Entity entity);

	SunLightEntityBuilder& withIntensity(float intensity);
	SunLightEntityBuilder& withColor(math::float3 color);
	SunLightEntityBuilder& withAngularRadius(float angularRadius);
	SunLightEntityBuilder& withHaloFalloff(float haloFalloff);
	SunLightEntityBuilder& withHaloSize(float haloSize);
	SunLightEntityBuilder& withCastShadows(bool castShadows);

  private:
	SunLight& getComponent();
};


} // namespace spatial::ecs