#pragma once

#include <spatial/ecs/Resource.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Script.h>
#include <spatial/ecs/Transform.h>

namespace spatial::ecs
{

class TransformBuilder;

class PerspectiveCameraBuilder;
class OrthographicCameraBuilder;
class CustomCameraBuilder;

class PointLightBuilder;
class SpotLightBuilder;
class DirectionalLightBuilder;
class SunLightBuilder;
class IndirectLightBuilder;

class PrecompiledMaterialBuilder;

class ResourceBuilder;

template <typename MaterialComponent>
class MaterialInstanceBuilder;

class MeshInstanceBuilder;

class MeshMaterialBuilder;

class SceneViewBuilder;

class Builder
{
  public:
	Builder(Registry& registry, Entity entity);

	Builder& withName(std::string name);

	template <typename Component>
	Builder& with(Component&& component)
	{
		mRegistry.template addComponent<Component>(mEntity, std::forward<Component>(component));
		return *this;
	}

	Builder& withParent(Entity parent);

	template <typename Component, typename... Args>
	Builder& with(Args&&... args)
	{
		mRegistry.template addComponent<Component>(mEntity, std::forward<Args>(args)...);
		return *this;
	}

	template <typename Component>
	Builder& with()
	{
		mRegistry.template addComponent<Component>(mEntity);
		return *this;
	}

	TransformBuilder asTransform();

	PerspectiveCameraBuilder asPerspectiveCamera();
	OrthographicCameraBuilder asOrthographicCamera();
	CustomCameraBuilder asCustomCamera();

	PointLightBuilder asPointLight();
	SpotLightBuilder asSpotLight();
	DirectionalLightBuilder asDirectionalLight();
	SunLightBuilder asSunLight();
	IndirectLightBuilder asIndirectLight();
	SceneViewBuilder asSceneView();

	ResourceBuilder asResource();

	MeshInstanceBuilder asMeshInstance();
	MeshMaterialBuilder asMeshMaterial();

	template <typename MaterialComponent>
	MaterialInstanceBuilder<MaterialComponent> asMaterialInstance()
	{
		return MaterialInstanceBuilder<MaterialComponent>{ mRegistry, mEntity };
	}

	[[nodiscard]] Entity get() const
	{
		return mEntity;
	}

	operator Entity() const // NOLINT(google-explicit-constructor)
	{
		return mEntity;
	}

	static Builder create(Registry& registry);

  protected:
	Registry& mRegistry;
	Entity mEntity;
};

template <typename Component>
class BasicBuilder : public Builder
{
  public:
	BasicBuilder(Registry& registry, Entity entity) : Builder(registry, entity)
	{
		with(Component{});
	}

  protected:
	Component& getComponent()
	{
		return mRegistry.getComponent<Component>(mEntity);
	}
};

class TransformBuilder : public BasicBuilder<Transform>
{
  public:
	using Base = BasicBuilder<Transform>;

	TransformBuilder(Registry& registry, Entity entity);

	TransformBuilder& withPosition(math::vec3 position);
	TransformBuilder& withScale(math::vec3 scale);
	TransformBuilder& withRotation(math::vec3 rotation);
};

class PerspectiveCameraBuilder : public BasicBuilder<PerspectiveCamera>
{
  public:
	using Base = BasicBuilder<PerspectiveCamera>;

	PerspectiveCameraBuilder(Registry& registry, Entity entity);

	PerspectiveCameraBuilder& withClippingPlanes(double near, double far);
	PerspectiveCameraBuilder& withFieldOfView(double fieldOfView);
	PerspectiveCameraBuilder& withAspectRatio(double aspectRatio);
};

class OrthographicCameraBuilder : public BasicBuilder<OrthographicCamera>
{
  public:
	using Base = BasicBuilder<OrthographicCamera>;

	OrthographicCameraBuilder(Registry& registry, Entity entity);

	OrthographicCameraBuilder& withClippingPlanes(double near, double far);
	OrthographicCameraBuilder& withProjection(double left, double right, double bottom, double top);
};

class CustomCameraBuilder : public BasicBuilder<CustomCamera>
{
  public:
	using Base = BasicBuilder<CustomCamera>;

	CustomCameraBuilder(Registry& registry, Entity entity);

	CustomCameraBuilder& withClippingPlanes(double near, double far);
	CustomCameraBuilder& withProjection(math::mat4 projectionMatrix);
};

class PointLightBuilder : public BasicBuilder<PointLight>
{
  public:
	using Base = BasicBuilder<PointLight>;

	PointLightBuilder(Registry& registry, Entity entity);

	PointLightBuilder& withIntensity(float intensity);
	PointLightBuilder& withColor(math::vec3 color);
	PointLightBuilder& withFalloff(float falloff);
};

class SpotLightBuilder : public BasicBuilder<SpotLight>
{
  public:
	using Base = BasicBuilder<SpotLight>;

	SpotLightBuilder(Registry& registry, Entity entity);

	SpotLightBuilder& withIntensity(float intensity);
	SpotLightBuilder& withColor(math::vec3 color);
	SpotLightBuilder& withDirection(math::vec3 direction);
	SpotLightBuilder& withFalloff(float falloff);
	SpotLightBuilder& withAngles(float innerAngle, float outerAngle);
	SpotLightBuilder& withCastShadows(bool castShadows);
};

class DirectionalLightBuilder : public BasicBuilder<DirectionalLight>
{
  public:
	using Base = BasicBuilder<DirectionalLight>;

	DirectionalLightBuilder(Registry& registry, Entity entity);

	DirectionalLightBuilder& withIntensity(float intensity);
	DirectionalLightBuilder& withColor(math::vec3 color);
	DirectionalLightBuilder& withDirection(math::vec3 direction);
	DirectionalLightBuilder& withCastShadows(bool castShadows);
};

class SunLightBuilder : public BasicBuilder<SunLight>
{
  public:
	using Base = BasicBuilder<SunLight>;

	SunLightBuilder(Registry& registry, Entity entity);

	SunLightBuilder& withIntensity(float intensity);
	SunLightBuilder& withColor(math::vec3 color);
	SunLightBuilder& withAngularRadius(float angularRadius);
	SunLightBuilder& withHaloFalloff(float haloFalloff);
	SunLightBuilder& withHaloSize(float haloSize);
	SunLightBuilder& withCastShadows(bool castShadows);
};

class IndirectLightBuilder : public BasicBuilder<IndirectLight>
{
  public:
	using Base = BasicBuilder<IndirectLight>;

	IndirectLightBuilder(Registry& registry, Entity entity);

	IndirectLightBuilder& withIntensity(float intensity);
	IndirectLightBuilder& withReflectionsTexture(std::string_view path);
	IndirectLightBuilder& withReflectionsTexture(ecs::Entity resource);

	IndirectLightBuilder& withIrradianceValues(std::string_view path);
	IndirectLightBuilder& withIrradianceValues(ecs::Entity resource);
};

template <typename MaterialProps>
class MaterialInstanceBuilder : public Builder
{
  public:
	MaterialInstanceBuilder(Registry& registry, Entity entity)
		: Builder(registry, entity)
	{
		with<tags::IsMaterialInstance>();
		with<MaterialProps>({});
	}

	MaterialInstanceBuilder& withMaterial(std::string_view relativeResourcePath)
	{
		withMaterial(ecs::Resource::findOrCreate<ecs::tags::IsMaterial>(mRegistry, relativeResourcePath));
		return *this;
	}

	MaterialInstanceBuilder& withMaterial(Entity resource)
	{
		if (mRegistry.hasAllComponents<Resource>(resource) && !mRegistry.hasAnyComponent<Name>(mEntity))
			withName(mRegistry.getComponent<Resource>(resource).stem());

		withParent(resource);
		return *this;
	}

	MaterialInstanceBuilder& withProps(MaterialProps&& params)
	{
		mRegistry.addOrReplaceComponent<MaterialProps>(mEntity, std::move(params));
		return *this;
	}
};

class ResourceBuilder : public BasicBuilder<Resource>
{
  public:
	using Base = BasicBuilder<Resource>;

	ResourceBuilder(Registry& registry, Entity entity);

	ResourceBuilder& withPath(std::string_view relativePath);
};

class MeshInstanceBuilder : public BasicBuilder<MeshInstance>
{
  public:
	using Base = BasicBuilder<MeshInstance>;

	MeshInstanceBuilder(Registry& registry, Entity entity);

	MeshInstanceBuilder& withMesh(std::string_view resource);
	MeshInstanceBuilder& withMesh(Entity resource);

	MeshInstanceBuilder& withShadowOptions(bool castShadows, bool receiveShadows);
	MeshInstanceBuilder& withDefaultMaterialInstance(Entity materialEntity);
	MeshInstanceBuilder& withMaterialAt(uint32_t primitiveIndex, Entity materialEntity);
	MeshInstanceBuilder& withSubMesh(std::uint8_t offset, std::uint8_t count);
	MeshInstanceBuilder& withCulling(bool culling);
	MeshInstanceBuilder& withPriority(uint8_t priority);
};

class MeshMaterialBuilder : public BasicBuilder<MeshMaterial>
{
  public:
	using Base = BasicBuilder<MeshMaterial>;

	MeshMaterialBuilder(Registry& registry, Entity entity);

	MeshMaterialBuilder& withPrimitiveIndex(uint32_t primitiveIndex);
	MeshMaterialBuilder& withMaterial(Entity materialInstance);
};

class SceneViewBuilder : public BasicBuilder<SceneView>
{
  public:
	using Base = BasicBuilder<SceneView>;

	SceneViewBuilder(Registry& registry, Entity entity);
	SceneViewBuilder& withDimensions(math::uvec2 dimensions);
	SceneViewBuilder& withCamera(ecs::Entity cameraEntity);
	SceneViewBuilder& withIndirectLight(ecs::Entity indirectLightEntity);
};

} // namespace spatial::ecs