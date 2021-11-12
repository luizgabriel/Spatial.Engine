#pragma once

#include "EntityHandle.h"
#include "EntityName.h"
#include "Mesh.h"
#include "Tags.h"
#include <spatial/common/Math.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Transform.h>

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

template <typename MaterialComponent>
class MaterialEntityBuilder;

class MeshEntityBuilder;

class EntityBuilder
{
  public:
	EntityBuilder(Registry& registry);

	EntityBuilder(Registry& registry, Entity entity);

	EntityBuilder& withName(std::string name);

	template <typename Component>
	EntityBuilder& with(Component&& component)
	{
		mRegistry.template addComponent<Component>(mEntity, std::move(component));
		return *this;
	}

	template <typename Component, typename... Args>
	EntityBuilder& with(Args&&... args)
	{
		mRegistry.addComponent<Component>(mEntity, std::forward<Args>(args)...);
		return *this;
	}

	template <typename Component>
	EntityBuilder& with()
	{
		mRegistry.addComponent<Component>(mEntity);
		return *this;
	}

	TransformEntityBuilder asTransform();

	PerspectiveCameraEntityBuilder asPerspectiveCamera();
	OrthographicCameraEntityBuilder asOrthographicCamera();
	CustomCameraEntityBuilder asCustomCamera();

	PointLightEntityBuilder asPointLight();
	SpotLightEntityBuilder asSpotLight();
	DirectionalLightEntityBuilder asDirectionalLight();
	SunLightEntityBuilder asSunLight();

	MeshEntityBuilder asMesh(std::filesystem::path path);

	template <typename MaterialComponent, typename... Args>
	MaterialEntityBuilder<MaterialComponent> asMaterial(Args&&... args)
	{
		return MaterialEntityBuilder<MaterialComponent>{mRegistry, mEntity, std::forward<Args>(args)...};
	}

	template <typename MaterialComponent>
	MaterialEntityBuilder<MaterialComponent> asMaterial(MaterialComponent&& component)
	{
		return MaterialEntityBuilder<MaterialComponent>{mRegistry, mEntity, std::forward<MaterialComponent>(component)};
	}

	[[nodiscard]] EntityHandle get() const
	{
		return {mRegistry, mEntity};
	}

	operator Entity() const
	{
		return get().operator entt::entity();
	}

  protected:
	Registry& mRegistry;
	Entity mEntity;
};

template <typename Component>
class BasicEntityBuilder : public EntityBuilder
{
  public:
	BasicEntityBuilder(Registry& registry, Entity entity): EntityBuilder(registry, entity)
	{
	}

  protected:
	Component& getComponent()
	{
		return mRegistry.getComponent<Component>(mEntity);
	}
};

class TransformEntityBuilder : public BasicEntityBuilder<Transform>
{
  public:
	using Base = BasicEntityBuilder<Transform>;

	TransformEntityBuilder(Registry& registry, Entity entity);

	TransformEntityBuilder& withPosition(math::float3 position);
	TransformEntityBuilder& withScale(math::float3 scale);
	TransformEntityBuilder& withRotation(math::float3 rotation);
};

class PerspectiveCameraEntityBuilder : public BasicEntityBuilder<PerspectiveCamera>
{
  public:
	using Base = BasicEntityBuilder<PerspectiveCamera>;

	PerspectiveCameraEntityBuilder(Registry& registry, Entity entity);

	PerspectiveCameraEntityBuilder& withClippingPlanes(double near, double far);
	PerspectiveCameraEntityBuilder& withFieldOfView(double fieldOfView);
	PerspectiveCameraEntityBuilder& withAspectRatio(double aspectRatio);
};

class OrthographicCameraEntityBuilder : public BasicEntityBuilder<OrthographicCamera>
{
  public:
	using Base = BasicEntityBuilder<OrthographicCamera>;

	OrthographicCameraEntityBuilder(Registry& registry, Entity entity);

	OrthographicCameraEntityBuilder& withClippingPlanes(double near, double far);
	OrthographicCameraEntityBuilder& withProjection(double left, double right, double bottom, double top);
};

class CustomCameraEntityBuilder : public BasicEntityBuilder<CustomCamera>
{
  public:
	using Base = BasicEntityBuilder<CustomCamera>;

	CustomCameraEntityBuilder(Registry& registry, Entity entity);

	CustomCameraEntityBuilder& withClippingPlanes(double near, double far);
	CustomCameraEntityBuilder& withProjection(math::mat4 projectionMatrix);
};

class PointLightEntityBuilder : public BasicEntityBuilder<PointLight>
{
  public:
	using Base = BasicEntityBuilder<PointLight>;

	PointLightEntityBuilder(Registry& registry, Entity entity);

	PointLightEntityBuilder& withIntensity(float intensity);
	PointLightEntityBuilder& withColor(math::float3 color);
	PointLightEntityBuilder& withFalloff(float falloff);
};

class SpotLightEntityBuilder : public BasicEntityBuilder<SpotLight>
{
  public:
	using Base = BasicEntityBuilder<SpotLight>;

	SpotLightEntityBuilder(Registry& registry, Entity entity);

	SpotLightEntityBuilder& withIntensity(float intensity);
	SpotLightEntityBuilder& withColor(math::float3 color);
	SpotLightEntityBuilder& withDirection(math::float3 direction);
	SpotLightEntityBuilder& withFalloff(float falloff);
	SpotLightEntityBuilder& withAngles(float innerAngle, float outerAngle);
	SpotLightEntityBuilder& withCastShadows(bool castShadows);
};

class DirectionalLightEntityBuilder : public BasicEntityBuilder<DirectionalLight>
{
  public:
	using Base = BasicEntityBuilder<DirectionalLight>;

	DirectionalLightEntityBuilder(Registry& registry, Entity entity);

	DirectionalLightEntityBuilder& withIntensity(float intensity);
	DirectionalLightEntityBuilder& withColor(math::float3 color);
	DirectionalLightEntityBuilder& withDirection(math::float3 direction);
	DirectionalLightEntityBuilder& withCastShadows(bool castShadows);
};

class SunLightEntityBuilder : public BasicEntityBuilder<SunLight>
{
  public:
	using Base = BasicEntityBuilder<SunLight>;

	SunLightEntityBuilder(Registry& registry, Entity entity);

	SunLightEntityBuilder& withIntensity(float intensity);
	SunLightEntityBuilder& withColor(math::float3 color);
	SunLightEntityBuilder& withAngularRadius(float angularRadius);
	SunLightEntityBuilder& withHaloFalloff(float haloFalloff);
	SunLightEntityBuilder& withHaloSize(float haloSize);
	SunLightEntityBuilder& withCastShadows(bool castShadows);
};

template <typename MaterialComponent>
class MaterialEntityBuilder : public EntityBuilder
{
  public:
	template <typename... Args>
	MaterialEntityBuilder(Registry& registry, Entity entity, Args&&... args) : EntityBuilder(registry, entity)
	{
		this->with<ecs::tags::IsMeshMaterial>();
		this->template with<MaterialComponent>(std::forward<Args>(args)...);
	}

	MaterialEntityBuilder(Registry& registry, Entity entity, MaterialComponent&& params) : EntityBuilder(registry, entity)
	{
		this->with<ecs::tags::IsMeshMaterial>();
		this->template with<MaterialComponent>(std::move(params));
	}
};

class MeshEntityBuilder : public BasicEntityBuilder<Mesh>
{
  public:
	using Base = BasicEntityBuilder<Mesh>;

	MeshEntityBuilder(Registry& registry, Entity entity, std::filesystem::path resourcePath);

	MeshEntityBuilder& withShadowOptions(bool castShadows, bool receiveShadows);

	MeshEntityBuilder& withMaterial(Entity materialEntity);

	MeshEntityBuilder& withSubMesh(std::uint8_t offset, std::uint8_t count);
};

} // namespace spatial::ecs