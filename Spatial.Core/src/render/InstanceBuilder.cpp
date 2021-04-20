#include <fmt/format.h>
#include <spatial/render/InstanceBuilder.h>

namespace spatial
{

InstanceBuilder::InstanceBuilder(Stage& stage, Instance instance) : mStage{stage}, mInstance{instance}
{
	if (!getInstance().has<Entity>())
	{
		auto entity = createEntity(mStage.getEngine());
		getInstance().add<Entity>(std::move(entity));
	}

	if (!getInstance().has<Transform>())
	{
		auto& sceneEntity = getInstance().get<Entity>();
		getInstance().add<Transform>(mStage.getEngine(), sceneEntity.get());
	}
}

InstanceBuilder::InstanceBuilder(const InstanceBuilder& other) : InstanceBuilder(other.mStage, other.mInstance)
{
}

InstanceBuilder& InstanceBuilder::operator=(const InstanceBuilder& other)
{
	assert(&mStage == &other.mStage);
	mInstance = other.mInstance;
	return *this;
}

Transform& InstanceBuilder::getTransform()
{
	return getInstance().get<Transform>();
}

InstanceBuilder& InstanceBuilder::withPosition(const math::float3& position)
{
	getTransform().setPosition(position);

	return *this;
}

InstanceBuilder& InstanceBuilder::withRotation(const math::float3& rotation)
{
	getTransform().setRotation(rotation);

	return *this;
}

InstanceBuilder& InstanceBuilder::withScale(const math::float3& scale)
{
	getTransform().setScale(scale);

	return *this;
}

CameraInstanceBuilder InstanceBuilder::asCamera()
{
	return CameraInstanceBuilder{mStage, mInstance};
}

LightInstanceBuilder InstanceBuilder::asLight(filament::LightManager::Type type)
{
	return LightInstanceBuilder{mStage, mInstance, type};
}

RenderableInstanceBuilder InstanceBuilder::asRenderable(std::size_t primitivesCount)
{
	return RenderableInstanceBuilder{mStage, mInstance, primitivesCount};
}

MeshInstanceBuilder InstanceBuilder::asMesh(const FilameshFile& filameshFile)
{
	return MeshInstanceBuilder{mStage, mInstance, filameshFile};
}

CameraInstanceBuilder::CameraInstanceBuilder(Stage& stage, Instance instance) : InstanceBuilder(stage, instance)
{
	if (!getInstance().has<Camera>())
	{
		auto& sceneEntity = getInstance().get<spatial::Entity>();
		getInstance().add<spatial::Camera>(mStage.getEngine(), sceneEntity.get());
	}
}

CameraInstanceBuilder& CameraInstanceBuilder::withPerspectiveProjection(double fieldOfView, double aspectRatio,
																		double near, double far)
{
	getComponent().setProjection(PerspectiveProjection{fieldOfView, aspectRatio, near, far});
	return *this;
}

CameraInstanceBuilder& CameraInstanceBuilder::withTarget(const math::float3& target)
{
	if (!getInstance().has<CameraTarget>())
	{
		getInstance().add<spatial::CameraTarget>(target);
	}

	getInstance().get<spatial::CameraTarget>().setTarget(target);
	return *this;
}

CameraInstanceBuilder& CameraInstanceBuilder::withOrthographicProjection(double aspectRatio, double near, double far)
{
	getComponent().setProjection(OrthographicProjection{aspectRatio, near, far});
	return *this;
}

CameraInstanceBuilder& CameraInstanceBuilder::withOrthographicProjection(double left, double right, double bottom,
																		 double top, double near, double far)
{
	getComponent().setProjection(OrthographicProjection{left, right, bottom, top, near, far});

	return *this;
}

CameraInstanceBuilder& CameraInstanceBuilder::withCustomProjection(math::mat4 projectionMatrix, double near, double far)
{
	getComponent().setProjection(CustomProjection{std::move(projectionMatrix), near, far});
	return *this;
}

Camera& CameraInstanceBuilder::getComponent()
{
	return getInstance().get<Camera>();
}

LightInstanceBuilder::LightInstanceBuilder(Stage& stage, Instance instance, filament::LightManager::Type type)
	: InstanceBuilder(stage, instance)
{
	if (!getInstance().has<Light>())
	{
		auto& sceneEntity = getInstance().get<spatial::Entity>();
		getInstance().add<Light>(mStage.getEngine(), sceneEntity.get(), type);
	}
}

LightInstanceBuilder& LightInstanceBuilder::withIntensity(float intensity)
{
	getComponent().setIntensity(intensity);
	return *this;
}

LightInstanceBuilder& LightInstanceBuilder::withDirection(const math::float3& direction)
{
	getComponent().setDirection(direction);
	return *this;
}

Light& LightInstanceBuilder::getComponent()
{
	return getInstance().get<spatial::Light>();
}

RenderableInstanceBuilder::RenderableInstanceBuilder(Stage& stage, Instance instance, std::size_t primitivesCount)
	: InstanceBuilder(stage, instance)
{
	if (!getInstance().has<Renderable>())
	{
		auto& sceneEntity = getInstance().get<spatial::Entity>();
		getInstance().add<spatial::Renderable>(mStage.getEngine(), sceneEntity.get(), primitivesCount);
	}
}

Renderable& RenderableInstanceBuilder::getComponent()
{
	return getInstance().get<Renderable>();
}

RenderableInstanceBuilder& RenderableInstanceBuilder::withShadowOptions(bool castShadows, bool receiveShadows)
{
	auto& renderable = getComponent();
	renderable.setCastShadows(castShadows);
	renderable.setReceiveShadows(receiveShadows);
	return *this;
}

RenderableInstanceBuilder& RenderableInstanceBuilder::withGeometryAt(size_t primitiveIndex,
																	 Renderable::PrimitiveType type,
																	 filament::VertexBuffer* vertexBuffer,
																	 filament::IndexBuffer* indexBuffer, size_t offset,
																	 size_t count)
{
	getComponent().setGeometryAt(primitiveIndex, type, vertexBuffer, indexBuffer, offset, count);
	return *this;
}

RenderableInstanceBuilder& RenderableInstanceBuilder::withMaterialAt(std::size_t primitiveIndex,
																	 const filament::MaterialInstance* materialInstance)
{
	getComponent().setMaterialInstanceAt(primitiveIndex, materialInstance);
	return *this;
}

RenderableInstanceBuilder& RenderableInstanceBuilder::withBoundingBox(const filament::Box& box)
{
	getComponent().setAxisAlignedBoundingBox(box);
	return *this;
}

InstanceBuilder createInstance(Stage& stage)
{
	return {stage, stage.createInstance()};
}

InstanceBuilder createInstance(Stage& stage, const std::string_view name)
{
	return {stage, stage.createInstance(name)};
}

MeshInstanceBuilder::MeshInstanceBuilder(Stage& stage, Instance instance, const FilameshFile& filameshFile)
	: RenderableInstanceBuilder(stage, instance, filameshFile.getPartsCount())
{
	withBoundingBox(filameshFile.getBoundingBox());

	size_t idx = 0;
	for (const auto& part : filameshFile.getParts())
	{
		withGeometryAt(idx++, Renderable::PrimitiveType::TRIANGLES, filameshFile.getVertexBuffer().get(),
					   filameshFile.getIndexBuffer().get(), part.offset, part.count);
	}
}

} // namespace spatial