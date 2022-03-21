#pragma once

#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/Script.h>

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
class IndirectLightEntityBuilder;

class PrecompiledMaterialEntityBuilder;

template <typename MaterialComponent>
class MaterialInstanceEntityBuilder;

class MeshEntityBuilder;

class MeshInstanceEntityBuilder;

class MeshMaterialEntityBuilder;

class SceneViewEntityBuilder;

class ScriptEntityBuilder;

class EntityBuilder
{
  public:
	EntityBuilder(Registry& registry, Entity entity);

	EntityBuilder& withName(std::string name);

	template <typename Component>
	EntityBuilder& with(Component&& component)
	{
		mRegistry.template addComponent<Component>(mEntity, std::forward<Component>(component));
		return *this;
	}

	EntityBuilder& withParent(Entity parent);

	template <typename Component, typename... Args>
	EntityBuilder& with(Args&&... args)
	{
		mRegistry.template addComponent<Component>(mEntity, std::forward<Args>(args)...);
		return *this;
	}

	template <typename Component>
	EntityBuilder& with()
	{
		mRegistry.template addComponent<Component>(mEntity);
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
	IndirectLightEntityBuilder asIndirectLight();
	SceneViewEntityBuilder asSceneView();

	MeshEntityBuilder asMesh();
	MeshInstanceEntityBuilder asMeshInstance();
	MeshMaterialEntityBuilder asMeshMaterial();

	ScriptEntityBuilder asScript();

	PrecompiledMaterialEntityBuilder asPrecompiledMaterial();

	template <typename MaterialComponent>
	MaterialInstanceEntityBuilder<MaterialComponent> asMaterialInstance()
	{
		return MaterialInstanceEntityBuilder<MaterialComponent>{ mRegistry, mEntity };
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
	BasicEntityBuilder(Registry& registry, Entity entity) : EntityBuilder(registry, entity)
	{
		with(Component{});
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

class IndirectLightEntityBuilder : public BasicEntityBuilder<IndirectLight>
{
  public:
	using Base = BasicEntityBuilder<IndirectLight>;

	IndirectLightEntityBuilder(Registry& registry, Entity entity);

	IndirectLightEntityBuilder& withIntensity(float intensity);
	IndirectLightEntityBuilder& withReflectionsTexturePath(const std::filesystem::path& path);
	IndirectLightEntityBuilder& withIrradianceValuesPath(const std::filesystem::path& path);
};

class PrecompiledMaterialEntityBuilder : public BasicEntityBuilder<PrecompiledMaterial>
{
  public:
	using Base = BasicEntityBuilder<PrecompiledMaterial>;

	PrecompiledMaterialEntityBuilder(Registry& registry, Entity entity);

	PrecompiledMaterialEntityBuilder& withResource(const std::filesystem::path& filamat);
};

template <typename MaterialProps>
class MaterialInstanceEntityBuilder : public EntityBuilder
{
  public:
	MaterialInstanceEntityBuilder(Registry& registry, Entity entity)
		: EntityBuilder(registry, entity)
	{
		with<tags::IsResource>();
		with<tags::IsMaterialInstance>();
		with<MaterialProps>({});
	}

	MaterialInstanceEntityBuilder& withMaterial(Entity material)
	{
		if (mRegistry.hasAllComponents<Name>(material) && !mRegistry.hasAnyComponent<Name>(mEntity))
			withName(mRegistry.getComponent<Name>(material).name);

		withParent(material);
		return *this;
	}

	MaterialInstanceEntityBuilder& withProps(MaterialProps&& params)
	{
		mRegistry.template addOrReplaceComponent(mEntity, std::move(params));
		return *this;
	}
};

class MeshEntityBuilder : public BasicEntityBuilder<Mesh>
{
  public:
	using Base = BasicEntityBuilder<Mesh>;

	MeshEntityBuilder(Registry& registry, Entity entity);

	MeshEntityBuilder& withResource(const std::filesystem::path& filamesh);
};

class MeshInstanceEntityBuilder : public BasicEntityBuilder<MeshInstance>
{
  public:
	using Base = BasicEntityBuilder<MeshInstance>;

	MeshInstanceEntityBuilder(Registry& registry, Entity entity);

	MeshInstanceEntityBuilder& withMesh(Entity meshSource);
	MeshInstanceEntityBuilder& withShadowOptions(bool castShadows, bool receiveShadows);
	MeshInstanceEntityBuilder& withMaterialAt(uint32_t primitiveIndex, Entity materialEntity);
	MeshInstanceEntityBuilder& withSubMesh(std::uint8_t offset, std::uint8_t count);
	MeshInstanceEntityBuilder& withCulling(bool culling);
	MeshInstanceEntityBuilder& withPriority(uint8_t priority);
};

class MeshMaterialEntityBuilder : public BasicEntityBuilder<MeshMaterial>
{
  public:
	using Base = BasicEntityBuilder<MeshMaterial>;

	MeshMaterialEntityBuilder(Registry& registry, Entity entity);

	MeshMaterialEntityBuilder& withPrimitiveIndex(uint32_t primitiveIndex);
	MeshMaterialEntityBuilder& withMaterial(Entity materialInstance);
};

class SceneViewEntityBuilder : public BasicEntityBuilder<SceneView>
{
  public:
	using Base = BasicEntityBuilder<SceneView>;

	SceneViewEntityBuilder(Registry& registry, Entity entity);
	SceneViewEntityBuilder& withCamera(ecs::Entity cameraEntity);
	SceneViewEntityBuilder& withIndirectLight(ecs::Entity indirectLightEntity);
};

class ScriptEntityBuilder : public BasicEntityBuilder<Script>
{
  public:
	using Base = BasicEntityBuilder<Script>;

	ScriptEntityBuilder(Registry& registry, Entity entity);
	ScriptEntityBuilder& withResource(const std::filesystem::path& script);
};

} // namespace spatial::ecs