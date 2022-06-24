#include "spatial/ecs/Texture.h"
#include <boost/algorithm/string/predicate.hpp>
#include <cassert>
#include <fmt/format.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Tags.h>

namespace spatial::ecs
{

Builder::Builder(Registry& registry, Entity entity) : mRegistry{registry}, mEntity{entity}
{
}

Builder& Builder::withName(std::string name)
{
	with<ecs::Name>(std::move(name));
	return *this;
}

TransformBuilder Builder::asTransform()
{
	return {mRegistry, mEntity};
}

PerspectiveCameraBuilder Builder::asPerspectiveCamera()
{
	return {mRegistry, mEntity};
}

OrthographicCameraBuilder Builder::asOrthographicCamera()
{
	return {mRegistry, mEntity};
}

CustomCameraBuilder Builder::asCustomCamera()
{
	return {mRegistry, mEntity};
}

PointLightBuilder Builder::asPointLight()
{
	return {mRegistry, mEntity};
}

SpotLightBuilder Builder::asSpotLight()
{
	return {mRegistry, mEntity};
}

DirectionalLightBuilder Builder::asDirectionalLight()
{
	return {mRegistry, mEntity};
}

SunLightBuilder Builder::asSunLight()
{
	return {mRegistry, mEntity};
}

MeshInstanceBuilder Builder::asMeshInstance()
{
	return {mRegistry, mEntity};
}

IndirectLightBuilder Builder::asIndirectLight()
{
	return {mRegistry, mEntity};
}

SceneViewBuilder Builder::asSceneView()
{
	return {mRegistry, mEntity};
}

MeshMaterialBuilder Builder::asMeshMaterial()
{
	return {mRegistry, mEntity};
}

ResourceBuilder Builder::asResource()
{
	return {mRegistry, mEntity};
}

Builder& Builder::withParent(Entity parent)
{
	ecs::Parent::addChild(mRegistry, parent, mEntity);
	return *this;
}

Builder Builder::create(Registry& registry)
{
	return {registry, registry.createEntity()};
}

TransformBuilder::TransformBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
}

TransformBuilder& TransformBuilder::withPosition(math::vec3 position)
{
	getComponent().position = position;
	return *this;
}

TransformBuilder& TransformBuilder::withScale(math::vec3 scale)
{
	getComponent().scale = scale;
	return *this;
}

TransformBuilder& TransformBuilder::withRotation(math::vec3 rotation)
{
	getComponent().rotation = rotation;
	return *this;
}

PerspectiveCameraBuilder::PerspectiveCameraBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsCamera>();
}

PerspectiveCameraBuilder& PerspectiveCameraBuilder::withFieldOfView(double fieldOfView)
{
	getComponent().fieldOfView = fieldOfView;
	return *this;
}

PerspectiveCameraBuilder& PerspectiveCameraBuilder::withAspectRatio(const AspectRatio& aspectRatio)
{
	getComponent().aspectRatio = aspectRatio.get();
	return *this;
}

PerspectiveCameraBuilder& PerspectiveCameraBuilder::withClippingPlanes(double near, double far)
{
	auto& component = getComponent();
	component.near = near;
	component.far = far;
	return *this;
}

OrthographicCameraBuilder::OrthographicCameraBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsCamera>();
}

OrthographicCameraBuilder& OrthographicCameraBuilder::withProjection(double left, double right, double bottom,
																	 double top)
{
	auto& component = getComponent();
	component.left = left;
	component.right = right;
	component.bottom = bottom;
	component.top = top;
	return *this;
}

OrthographicCameraBuilder& OrthographicCameraBuilder::withClippingPlanes(double near, double far)
{
	auto& component = getComponent();
	component.near = near;
	component.far = far;
	return *this;
}

CustomCameraBuilder::CustomCameraBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsCamera>();
}

CustomCameraBuilder& CustomCameraBuilder::withProjection(math::mat4 projectionMatrix)
{
	getComponent().projectionMatrix = projectionMatrix;
	return *this;
}

CustomCameraBuilder& CustomCameraBuilder::withClippingPlanes(double near, double far)
{
	auto& component = getComponent();
	component.near = near;
	component.far = far;
	return *this;
}

PointLightBuilder::PointLightBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

PointLightBuilder& PointLightBuilder::withFalloff(float falloff)
{
	getComponent().falloff = falloff;
	return *this;
}

PointLightBuilder& PointLightBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

PointLightBuilder& PointLightBuilder::withColor(math::vec3 color)
{
	getComponent().color = color;
	return *this;
}

SpotLightBuilder::SpotLightBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

SpotLightBuilder& SpotLightBuilder::withDirection(math::vec3 direction)
{
	getComponent().direction = direction;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::withFalloff(float falloff)
{
	getComponent().falloff = falloff;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::withAngles(float innerAngle, float outerAngle)
{
	assert(innerAngle <= outerAngle);
	auto& component = getComponent();
	component.innerAngle = innerAngle;
	component.outerAngle = outerAngle;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::withColor(math::vec3 color)
{
	getComponent().color = color;
	return *this;
}

DirectionalLightBuilder::DirectionalLightBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

DirectionalLightBuilder& DirectionalLightBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

DirectionalLightBuilder& DirectionalLightBuilder::withColor(math::vec3 color)
{
	getComponent().color = color;
	return *this;
}

DirectionalLightBuilder& DirectionalLightBuilder::withDirection(math::vec3 direction)
{
	getComponent().direction = direction;
	return *this;
}

DirectionalLightBuilder& DirectionalLightBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}

SunLightBuilder::SunLightBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

SunLightBuilder& SunLightBuilder::withIntensity(float intensity)
{
	getComponent().intensity = intensity;
	return *this;
}

SunLightBuilder& SunLightBuilder::withColor(math::vec3 color)
{
	getComponent().color = color;
	return *this;
}

SunLightBuilder& SunLightBuilder::withAngularRadius(float angularRadius)
{
	getComponent().angularRadius = angularRadius;
	return *this;
}

SunLightBuilder& SunLightBuilder::withHaloFalloff(float haloFalloff)
{
	getComponent().haloFalloff = haloFalloff;
	return *this;
}

SunLightBuilder& SunLightBuilder::withHaloSize(float haloSize)
{
	getComponent().haloSize = haloSize;
	return *this;
}

SunLightBuilder& SunLightBuilder::withCastShadows(bool castShadows)
{
	getComponent().castShadows = castShadows;
	return *this;
}

IndirectLightBuilder::IndirectLightBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>().with<ecs::tags::IsLight>();
}

IndirectLightBuilder& IndirectLightBuilder::withIntensity(float intensity)
{
	auto& component = getComponent();
	component.intensity = intensity;
	return *this;
}

IndirectLightBuilder& IndirectLightBuilder::withReflectionsTexture(std::string_view path)
{
	return withReflectionsTexture(ecs::Resource::findOrCreate<ecs::tags::IsCubeMapTexture>(mRegistry, path));
}

IndirectLightBuilder& IndirectLightBuilder::withReflectionsTexture(ecs::Entity resource)
{
	getComponent().reflectionsTexture = resource;
	return *this;
}

IndirectLightBuilder& IndirectLightBuilder::withIrradianceValues(std::string_view path)
{
	return withIrradianceValues(ecs::Resource::findOrCreate<ecs::tags::IsIrradianceValues>(mRegistry, path));
}

IndirectLightBuilder& IndirectLightBuilder::withIrradianceValues(ecs::Entity resource)
{
	getComponent().irradianceValues = resource;
	return *this;
}

MeshInstanceBuilder::MeshInstanceBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
	with<ecs::tags::IsRenderable>();
}

MeshInstanceBuilder& MeshInstanceBuilder::withMesh(std::string_view resource)
{
	return withMesh(ecs::Resource::findOrCreate<ecs::tags::IsMesh>(mRegistry, resource));
}

MeshInstanceBuilder& MeshInstanceBuilder::withMesh(Entity resource)
{
	if (mRegistry.hasAllComponents<Resource>(resource) && !mRegistry.hasAllComponents<Name>(mEntity))
		withName(mRegistry.getComponent<Resource>(resource).stem());

	getComponent().meshSource = resource;
	return *this;
}

MeshInstanceBuilder& MeshInstanceBuilder::withDefaultMaterialInstance(Entity materialEntity)
{
	getComponent().defaultMaterial = materialEntity;
	return *this;
}

MeshInstanceBuilder& MeshInstanceBuilder::withShadowOptions(bool castShadows, bool receiveShadows)
{
	auto& component = getComponent();
	component.castShadows = castShadows;
	component.receiveShadows = receiveShadows;
	return *this;
}

MeshInstanceBuilder& MeshInstanceBuilder::withSubMesh(std::uint8_t offset, std::uint8_t count)
{
	auto& component = getComponent();
	component.slice.count = count;
	component.slice.offset = offset;
	return *this;
}

MeshInstanceBuilder& MeshInstanceBuilder::withCulling(bool culling)
{
	getComponent().culling = culling;
	return *this;
}

MeshInstanceBuilder& MeshInstanceBuilder::withPriority(uint8_t priority)
{
	getComponent().priority = priority;
	return *this;
}

MeshInstanceBuilder& MeshInstanceBuilder::withMaterialAt(uint32_t primitiveIndex, Entity materialEntity)
{
	ecs::MeshInstance::addMaterial(mRegistry, mEntity, materialEntity, primitiveIndex);
	return *this;
}

SceneViewBuilder::SceneViewBuilder(Registry& registry, Entity entity) : Base(registry, entity)
{
}

SceneViewBuilder& SceneViewBuilder::withCamera(ecs::Entity cameraEntity)
{
	getComponent().camera = cameraEntity;
	return *this;
}

SceneViewBuilder& SceneViewBuilder::withIndirectLight(ecs::Entity indirectLightEntity)
{
	getComponent().indirectLight = indirectLightEntity;
	return *this;
}

SceneViewBuilder& SceneViewBuilder::withDimensions(math::uvec2 dimensions)
{
	getComponent().size = dimensions;
	return *this;
}

MeshMaterialBuilder::MeshMaterialBuilder(Registry& registry, Entity entity) : BasicBuilder(registry, entity)
{
}

MeshMaterialBuilder& MeshMaterialBuilder::withPrimitiveIndex(uint32_t primitiveIndex)
{
	if (!mRegistry.hasAllComponents<Name>(mEntity))
		withName(fmt::format("Primitive {}", primitiveIndex));

	getComponent().primitiveIndex = primitiveIndex;
	return *this;
}

MeshMaterialBuilder& MeshMaterialBuilder::withMaterial(Entity materialInstance)
{
	getComponent().materialInstanceEntity = materialInstance;
	return *this;
}

ResourceBuilder::ResourceBuilder(Registry& registry, Entity entity) : BasicBuilder(registry, entity)
{
}

ResourceBuilder& ResourceBuilder::withPath(std::string_view relativePath)
{
	getComponent().relativePath = relativePath;

	const auto fileName = getComponent().filename();

	if (!relativePath.empty() && !mRegistry.hasAllComponents<ecs::Name>(mEntity))
		withName(fileName);

	return *this;
}

} // namespace spatial::ecs