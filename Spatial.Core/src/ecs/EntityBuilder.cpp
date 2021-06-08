#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/EntityName.h>

namespace spatial::ecs
{

EntityBuilder::EntityBuilder(Registry& registry) : EntityBuilder{registry, registry.createEntity()}
{
}

EntityBuilder::EntityBuilder(Registry& registry, Entity entity) : mRegistry{registry}, mEntity{entity}
{
}

EntityBuilder& EntityBuilder::withName(std::string name)
{
	mRegistry.addOrReplaceComponent<ecs::EntityName>(mEntity, std::move(name));
	return *this;
}

TransformEntityBuilder EntityBuilder::asTransform()
{
	return {mRegistry, mEntity};
}

PerspectiveCameraEntityBuilder EntityBuilder::asPerspectiveCamera()
{
	return {mRegistry, mEntity};
}

OrthographicCameraEntityBuilder EntityBuilder::asOrthographicCamera()
{
	return {mRegistry, mEntity};
}

CustomCameraEntityBuilder EntityBuilder::asCustomCamera()
{
	return {mRegistry, mEntity};
}

PointLightEntityBuilder EntityBuilder::asPointLight()
{
	return {mRegistry, mEntity};
}

SpotLightEntityBuilder EntityBuilder::asSpotLight()
{
	return {mRegistry, mEntity};
}

DirectionalLightEntityBuilder EntityBuilder::asDirectionalLight()
{
	return {mRegistry, mEntity};
}

SunLightEntityBuilder EntityBuilder::asSunLight()
{
	return {mRegistry, mEntity};
}

TransformEntityBuilder::TransformEntityBuilder(Registry& registry, Entity entity) : EntityBuilder(registry, entity)
{
	mRegistry.addOrReplaceComponent(mEntity, Transform{});
}

TransformEntityBuilder& TransformEntityBuilder::withPosition(math::float3 position)
{
	getComponent().position = std::move(position);
	return *this;
}

TransformEntityBuilder& TransformEntityBuilder::withScale(math::float3 scale)
{
	getComponent().scale = std::move(scale);
	return *this;
}

TransformEntityBuilder& TransformEntityBuilder::withRotation(math::float3 rotation)
{
	getComponent().rotation = std::move(rotation);
	return *this;
}

ecs::Transform& TransformEntityBuilder::getComponent()
{
	return mRegistry.getComponent<Transform>(mEntity);
}


PerspectiveCameraEntityBuilder::PerspectiveCameraEntityBuilder(Registry& registry, Entity entity)
	: EntityBuilder(registry, entity)
{
	mRegistry.addOrReplaceComponent(mEntity, PerspectiveCamera{60.0f, 16.0f / 9.0f, .0f, 10000.0f});
}

PerspectiveCameraEntityBuilder& PerspectiveCameraEntityBuilder::withFieldOfView(double fieldOfView)
{
	getComponent().fieldOfView = fieldOfView;
	return *this;
}

PerspectiveCameraEntityBuilder& PerspectiveCameraEntityBuilder::withAspectRatio(double aspectRatio)
{
	getComponent().aspectRatio = aspectRatio;
	return *this;
}

PerspectiveCameraEntityBuilder& PerspectiveCameraEntityBuilder::withClippingPlanes(double near, double far)
{
	auto& component = getComponent();
	component.near = near;
	component.far = far;
	return *this;
}

ecs::PerspectiveCamera& PerspectiveCameraEntityBuilder::getComponent()
{
	return mRegistry.getComponent<PerspectiveCamera>(mEntity);
}

OrthographicCameraEntityBuilder::OrthographicCameraEntityBuilder(Registry& registry, Entity entity)
	: EntityBuilder(registry, entity)
{
	mRegistry.addOrReplaceComponent(mEntity, OrthographicCamera{-1.0f, 1.0f, -1.0f, 1.0f, .0f, 1.0f});
}

OrthographicCameraEntityBuilder& OrthographicCameraEntityBuilder::withProjection(double left, double right,
																				 double bottom, double top)
{
	auto& component = getComponent();
	component.left = left;
	component.right = right;
	component.bottom = bottom;
	component.top = top;
	return *this;
}

OrthographicCameraEntityBuilder& OrthographicCameraEntityBuilder::withClippingPlanes(double near, double far)
{
	auto& component = getComponent();
	component.near = near;
	component.far = far;
	return *this;
}

ecs::OrthographicCamera& OrthographicCameraEntityBuilder::getComponent()
{
	return mRegistry.getComponent<OrthographicCamera>(mEntity);
}

CustomCameraEntityBuilder::CustomCameraEntityBuilder(Registry& registry, Entity entity)
	: EntityBuilder(registry, entity)
{
	auto defaultProjection = math::mat4::perspective(60.0f, 19.0f / 6.0f, .0f, 10000.0f);
	mRegistry.addOrReplaceComponent(mEntity, CustomCamera{std::move(defaultProjection), .0f, 10000.0f});
}

CustomCameraEntityBuilder& CustomCameraEntityBuilder::withProjection(math::mat4 projectionMatrix)
{
	getComponent().projectionMatrix = projectionMatrix;
	return *this;
}

CustomCameraEntityBuilder& CustomCameraEntityBuilder::withClippingPlanes(double near, double far)
{
	auto& component = getComponent();
	component.near = near;
	component.far = far;
	return *this;
}

ecs::CustomCamera& CustomCameraEntityBuilder::getComponent()
{
	return mRegistry.getComponent<CustomCamera>(mEntity);
}

PointLightEntityBuilder::PointLightEntityBuilder(Registry& registry, Entity entity)
	: EntityBuilder(registry, entity)
{
	mRegistry.addOrReplaceComponent(mEntity, PointLight{});
}

PointLightEntityBuilder& PointLightEntityBuilder::withFalloff(float falloff)
{
	getComponent().falloff = falloff;
	return *this;
}

PointLightEntityBuilder& PointLightEntityBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

PointLightEntityBuilder& PointLightEntityBuilder::withColor(math::float3 color)
{
	getComponent().color = std::move(color);
	return *this;
}

ecs::PointLight& PointLightEntityBuilder::getComponent()
{
	return mRegistry.getComponent<PointLight>(mEntity);
}

SpotLightEntityBuilder::SpotLightEntityBuilder(Registry& registry, Entity entity) : EntityBuilder(registry, entity)
{
	mRegistry.addOrReplaceComponent(mEntity, SpotLight{});
}

SpotLightEntityBuilder& SpotLightEntityBuilder::withDirection(math::float3 direction)
{
	getComponent().direction = std::move(direction);
	return *this;
}

SpotLightEntityBuilder& SpotLightEntityBuilder::withFalloff(float falloff)
{
	getComponent().falloff = falloff;
	return *this;
}

SpotLightEntityBuilder& SpotLightEntityBuilder::withAngles(float innerAngle, float outerAngle)
{
	assert(innerAngle <= outerAngle);
	auto& component = getComponent();
	component.innerAngle = innerAngle;
	component.outerAngle = outerAngle;
	return *this;
}

SpotLightEntityBuilder& SpotLightEntityBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}


SpotLightEntityBuilder& SpotLightEntityBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

SpotLightEntityBuilder& SpotLightEntityBuilder::withColor(math::float3 color)
{
	getComponent().color = std::move(color);
	return *this;
}

SpotLight& SpotLightEntityBuilder::getComponent()
{
	return mRegistry.getComponent<SpotLight>(mEntity);
}

DirectionalLightEntityBuilder::DirectionalLightEntityBuilder(Registry& registry, Entity entity)
	: EntityBuilder(registry, entity)
{
	mRegistry.addOrReplaceComponent(mEntity, DirectionalLight{});
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withColor(math::float3 color)
{
	getComponent().color = std::move(color);
	return *this;
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withDirection(math::float3 direction)
{
	getComponent().direction = std::move(direction);
	return *this;
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}

DirectionalLight& DirectionalLightEntityBuilder::getComponent()
{
	return mRegistry.getComponent<DirectionalLight>(mEntity);
}

} // namespace spatial::ecs