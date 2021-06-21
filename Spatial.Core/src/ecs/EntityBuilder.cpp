#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/EntityName.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Tags.h>

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

MeshEntityBuilder EntityBuilder::asMesh(std::string resourceName)
{
	return {mRegistry, mEntity, std::move(resourceName)};
}

TransformEntityBuilder::TransformEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with(Transform{});
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

PerspectiveCameraEntityBuilder::PerspectiveCameraEntityBuilder(Registry& registry, Entity entity)
	: Base(registry, entity)
{
	with(PerspectiveCamera{60.0f, 16.0f / 9.0f, .1f, 10000.0f}).with<ecs::tags::IsRenderable>();
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

OrthographicCameraEntityBuilder::OrthographicCameraEntityBuilder(Registry& registry, Entity entity)
	: Base(registry, entity)
{
	with(OrthographicCamera{-1.0f, 1.0f, -1.0f, 1.0f, .1f, 1.0f}).with<ecs::tags::IsRenderable>();
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

CustomCameraEntityBuilder::CustomCameraEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	auto defaultProjection = math::mat4::perspective(60.0f, 19.0f / 6.0f, .0f, 10000.0f);
	with(CustomCamera{std::move(defaultProjection), .0f, 10000.0f}).with<ecs::tags::IsRenderable>();
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

PointLightEntityBuilder::PointLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with(PointLight{}).with<ecs::tags::IsRenderable>();
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

SpotLightEntityBuilder::SpotLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with(SpotLight{}).with<ecs::tags::IsRenderable>();
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

DirectionalLightEntityBuilder::DirectionalLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with(DirectionalLight{}).with<ecs::tags::IsRenderable>();
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

SunLightEntityBuilder::SunLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with(SunLight{}).with<ecs::tags::IsRenderable>();
}

SunLightEntityBuilder& SunLightEntityBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

SunLightEntityBuilder& SunLightEntityBuilder::withColor(math::float3 color)
{
	getComponent().color = color;
	return *this;
}

SunLightEntityBuilder& SunLightEntityBuilder::withAngularRadius(float angularRadius)
{
	getComponent().angularRadius = angularRadius;
	return *this;
}

SunLightEntityBuilder& SunLightEntityBuilder::withHaloFalloff(float haloFalloff)
{
	getComponent().haloFalloff = haloFalloff;
	return *this;
}

SunLightEntityBuilder& SunLightEntityBuilder::withHaloSize(float haloSize)
{
	getComponent().haloSize = haloSize;
	return *this;
}

SunLightEntityBuilder& SunLightEntityBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}

MeshEntityBuilder::MeshEntityBuilder(Registry& registry, Entity entity, std::string resourceName)
	: Base(registry, entity)
{
	with(Mesh{resourceName}).with<ecs::tags::IsRenderable>();
}

MeshEntityBuilder& MeshEntityBuilder::withShadowOptions(float castShadows, float receiveShadows)
{
	auto& component = getComponent();
	component.castShadows = castShadows;
	component.receiveShadows = receiveShadows;
	return *this;
}

MeshEntityBuilder& MeshEntityBuilder::withMaterialAt(std::uint8_t index, Entity materialEntity)
{
	assert(mRegistry.hasAllComponents<ecs::tags::IsMeshMaterial>(materialEntity));
	getComponent().materials.at(index) = materialEntity;
	return *this;
}

MeshEntityBuilder& MeshEntityBuilder::withSubMesh(std::uint8_t offset, std::uint8_t count)
{
	assert(count > 0); // there is no sub-mesh with size zero
	auto& component = getComponent();
	component.partsCount = count;
	component.partsOffset = offset;
	return *this;
}

} // namespace spatial::ecs