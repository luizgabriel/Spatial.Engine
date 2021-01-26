#pragma once

#include <filament/Engine.h>

#include <spatial/common/Math.h>
#include <spatial/render/Camera.h>
#include <spatial/render/Entity.h>
#include <spatial/render/FilameshFile.h>
#include <spatial/render/InstanceHandle.h>
#include <spatial/render/Light.h>
#include <spatial/render/Renderable.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SceneNodeName.h>
#include <spatial/render/Stage.h>
#include <spatial/render/Transform.h>

namespace spatial
{

class CameraInstanceBuilder;
class LightInstanceBuilder;
class RenderableInstanceBuilder;
class MeshInstanceBuilder;

class InstanceBuilder
{
  protected:
	Stage& mStage;
	Instance mInstance;

  public:
	InstanceBuilder(Stage& stage, Instance instance);

	InstanceBuilder(const InstanceBuilder& other);
	InstanceBuilder& operator=(const InstanceBuilder& other);

	InstanceBuilder& withPosition(const math::float3& position);

	InstanceBuilder& withRotation(const math::float3& rotation);

	InstanceBuilder& withScale(const math::float3& scale);

	template <typename Component, typename... Args>
	InstanceBuilder& add(Args&&... args)
	{
		getInstance().add<Component>(std::forward<Args>(args)...);
		return *this;
	}

	InstanceHandle build() noexcept
	{
		return *this;
	}

	operator Instance() noexcept
	{
		return mInstance;
	}

	operator InstanceHandle() noexcept
	{
		return {mStage, mInstance};
	}

	operator ConstInstanceHandle() const noexcept
	{
		return {mStage, mInstance};
	}

	CameraInstanceBuilder asCamera();

	LightInstanceBuilder asLight(Light::Type type = Light::Type::POINT);

	RenderableInstanceBuilder asRenderable(std::size_t primitivesCount);

	MeshInstanceBuilder asMesh(const FilameshFile& filameshFile);

  protected:
	InstanceHandle getInstance()
	{
		return *this;
	}

	Transform& getTransform();
};

class CameraInstanceBuilder : public InstanceBuilder
{
  public:
	CameraInstanceBuilder(Stage& stage, spatial::Instance instance);

	CameraInstanceBuilder& withPerspectiveProjection(double fieldOfView, double aspectRatio, double near, double far);

	CameraInstanceBuilder& withOrthographicProjection(double aspectRatio, double near, double far);

	CameraInstanceBuilder& withOrthographicProjection(double left, double right, double bottom, double top, double near,
													  double far);

	CameraInstanceBuilder& withCustomProjection(math::mat4 projectionMatrix, double near, double far);

  protected:
	Camera& getComponent();
};

class LightInstanceBuilder : public InstanceBuilder
{
  public:
	LightInstanceBuilder(Stage& stage, Instance instance, Light::Type type);

	LightInstanceBuilder& withIntensity(float intensity);

	LightInstanceBuilder& withDirection(const math::float3& direction);

  protected:
	Light& getComponent();
};

class RenderableInstanceBuilder : public InstanceBuilder
{
  public:
	RenderableInstanceBuilder(Stage& stage, Instance instance, std::size_t primitivesCount);

	RenderableInstanceBuilder& withShadowOptions(bool castShadows, bool receiveShadows);

	RenderableInstanceBuilder& withGeometryAt(size_t primitiveIndex, Renderable::PrimitiveType type,
											  filament::VertexBuffer* vertexBuffer, filament::IndexBuffer* indexBuffer,
											  size_t offset, size_t count);

	RenderableInstanceBuilder& withMaterialAt(std::size_t primitiveIndex,
											  const filament::MaterialInstance* materialInstance);

	RenderableInstanceBuilder& withBoundingBox(const filament::Box& box);

  protected:
	Renderable& getComponent();
};

class MeshInstanceBuilder : public RenderableInstanceBuilder
{
  public:
	MeshInstanceBuilder(Stage& stage, Instance instance, const FilameshFile& filameshFile);
};

InstanceBuilder createInstance(Stage& stage);

InstanceBuilder createInstance(Stage& stage, const std::string_view name);

} // namespace spatial