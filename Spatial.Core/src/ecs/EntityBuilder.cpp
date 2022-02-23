#include <fmt/format.h>
#include <spatial/ecs/EntityBuilder.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <cassert>

namespace spatial::ecs
{

EntityBuilder::EntityBuilder(Registry& registry, Entity entity) : mRegistry{registry}, mEntity{entity}
{
}

EntityBuilder& EntityBuilder::withName(std::string name)
{
	with<ecs::Name>(std::move(name));
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

MeshEntityBuilder EntityBuilder::asMesh()
{
	return {mRegistry, mEntity};
}

MeshInstanceEntityBuilder EntityBuilder::asMeshInstance()
{
	return {mRegistry, mEntity};
}

IndirectLightEntityBuilder EntityBuilder::asIndirectLight()
{
	return {mRegistry, mEntity};
}

SceneViewEntityBuilder EntityBuilder::asSceneView()
{
	return {mRegistry, mEntity};
}

TransformEntityBuilder::TransformEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
}

TransformEntityBuilder& TransformEntityBuilder::withPosition(math::float3 position)
{
	getComponent().position = position;
	return *this;
}

TransformEntityBuilder& TransformEntityBuilder::withScale(math::float3 scale)
{
	getComponent().scale = scale;
	return *this;
}

TransformEntityBuilder& TransformEntityBuilder::withRotation(math::float3 rotation)
{
	getComponent().rotation = rotation;
	return *this;
}

PerspectiveCameraEntityBuilder::PerspectiveCameraEntityBuilder(Registry& registry, Entity entity)
	: Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsCamera>();
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
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsCamera>();
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
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsCamera>();
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
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
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
	getComponent().color = color;
	return *this;
}

SpotLightEntityBuilder::SpotLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

SpotLightEntityBuilder& SpotLightEntityBuilder::withDirection(math::float3 direction)
{
	getComponent().direction = direction;
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
	getComponent().color = color;
	return *this;
}

DirectionalLightEntityBuilder::DirectionalLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withColor(math::float3 color)
{
	getComponent().color = color;
	return *this;
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withDirection(math::float3 direction)
{
	getComponent().direction = direction;
	return *this;
}

DirectionalLightEntityBuilder& DirectionalLightEntityBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}

SunLightEntityBuilder::SunLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
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

IndirectLightEntityBuilder::IndirectLightEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

IndirectLightEntityBuilder& IndirectLightEntityBuilder::withIntensity(float intensity)
{
	auto& component = getComponent();
	component.intensity = intensity;
	return *this;
}

IndirectLightEntityBuilder& IndirectLightEntityBuilder::withReflectionsTexturePath(const std::filesystem::path& path)
{
	getComponent().reflectionsTexturePath.relativePath = path;
	return *this;
}

IndirectLightEntityBuilder& IndirectLightEntityBuilder::withIrradianceValuesPath(const std::filesystem::path& path)
{
	getComponent().irradianceValuesPath.relativePath = path;
	return *this;
}

MeshInstanceEntityBuilder::MeshInstanceEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>();
}

MeshInstanceEntityBuilder& MeshInstanceEntityBuilder::withMesh(Entity meshSource)
{
	getComponent().meshSource = meshSource;
	return *this;
}

MeshInstanceEntityBuilder& MeshInstanceEntityBuilder::withShadowOptions(bool castShadows, bool receiveShadows)
{
	auto& component = getComponent();
	component.castShadows = castShadows;
	component.receiveShadows = receiveShadows;
	return *this;
}

MeshInstanceEntityBuilder& MeshInstanceEntityBuilder::withSubMesh(std::uint8_t offset, std::uint8_t count)
{
	auto& component = getComponent();
	component.slice.count = count;
	component.slice.offset = offset;
	return *this;
}

MeshInstanceEntityBuilder& MeshInstanceEntityBuilder::withCulling(bool culling)
{
	getComponent().culling = culling;
	return *this;
}

MeshInstanceEntityBuilder& MeshInstanceEntityBuilder::withPriority(uint8_t priority)
{
	getComponent().priority = priority;
	return *this;
}

MeshInstanceEntityBuilder& MeshInstanceEntityBuilder::withMaterialAt(uint32_t primitiveIndex, Entity materialEntity)
{
	ecs::MeshInstance::addMaterial(mRegistry, mEntity, materialEntity, primitiveIndex);
	return *this;
}

MeshEntityBuilder::MeshEntityBuilder(Registry& registry, Entity entity) : BasicEntityBuilder(registry, entity)
{
	with<ecs::tags::IsResource>().with<ecs::tags::IsMesh>();
}

MeshEntityBuilder& MeshEntityBuilder::withResource(const std::filesystem::path& filamesh)
{
	if (!filamesh.empty() && !mRegistry.hasAllComponents<ecs::Name>(mEntity))
		withName(filamesh.filename());

	getComponent().resource.relativePath = filamesh;
	return *this;
}

SceneViewEntityBuilder::SceneViewEntityBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
}

SceneViewEntityBuilder& SceneViewEntityBuilder::withCamera(ecs::Entity cameraEntity)
{
	ecs::Parent::addChild(mRegistry, mEntity, cameraEntity);
	getComponent().camera = cameraEntity;
	return *this;
}

SceneViewEntityBuilder& SceneViewEntityBuilder::withIndirectLight(ecs::Entity indirectLightEntity)
{
	ecs::Parent::addChild(mRegistry, mEntity, indirectLightEntity);
	getComponent().indirectLight = indirectLightEntity;
	return *this;
}

} // namespace spatial::ecs