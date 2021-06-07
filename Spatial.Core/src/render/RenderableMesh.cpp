#include <filament/Engine.h>
#include <spatial/render/RenderableMesh.h>

namespace fl = filament;

namespace spatial::render
{

RenderableMesh::RenderableMesh(filament::Engine& engine, utils::Entity entity, std::size_t primitivesCount)
	: mEngine{engine}, mEntity{entity}
{
	if (Builder(primitivesCount)
			.castShadows(true)
			.receiveShadows(true)
			.build(engine, entity)
		!= Builder::Result::Success)
	{
		throw std::runtime_error("Could not create renderable");
	}
}

RenderableMesh::~RenderableMesh()
{
	if (isValid())
		getManager().destroy(mEntity);
}

bool RenderableMesh::isValid() const noexcept
{
	return getInstance().isValid();
}

void RenderableMesh::setAxisAlignedBoundingBox(const fl::Box& aabb) noexcept
{
	getManager().setAxisAlignedBoundingBox(getInstance(), aabb);
}

void RenderableMesh::setLayerMask(uint8_t select, uint8_t values) noexcept
{
	getManager().setLayerMask(getInstance(), select, values);
}

void RenderableMesh::setPriority(uint8_t priority) noexcept
{
	getManager().setPriority(getInstance(), priority);
}

void RenderableMesh::setCulling(bool enable) noexcept
{
	getManager().setCulling(getInstance(), enable);
}

void RenderableMesh::setCastShadows(bool enable) noexcept
{
	getManager().setCastShadows(getInstance(), enable);
}

void RenderableMesh::setReceiveShadows(bool enable) noexcept
{
	getManager().setReceiveShadows(getInstance(), enable);
}

void RenderableMesh::setScreenSpaceContactShadows(bool enable) noexcept
{
	getManager().setScreenSpaceContactShadows(getInstance(), enable);
}

bool RenderableMesh::isShadowCaster() const noexcept
{
	return getManager().isShadowCaster(getInstance());
}

bool RenderableMesh::isShadowReceiver() const noexcept
{
	return getManager().isShadowReceiver(getInstance());
}

void RenderableMesh::setBones(const Bone* transforms, size_t boneCount, size_t offset) noexcept
{
	getManager().setBones(getInstance(), transforms, boneCount, offset);
}

void RenderableMesh::setBones(const math::mat4f* transforms, size_t boneCount, size_t offset) noexcept
{
	getManager().setBones(getInstance(), transforms, boneCount, offset);
}

void RenderableMesh::setMorphWeights(const math::float4& weights) noexcept
{
	getManager().setMorphWeights(getInstance(), weights);
}

const filament::Box& RenderableMesh::getAxisAlignedBoundingBox() const noexcept
{
	return getManager().getAxisAlignedBoundingBox(getInstance());
}

void RenderableMesh::setMaterialInstanceAt(size_t primitiveIndex, const fl::MaterialInstance* materialInstance) noexcept
{
	return getManager().setMaterialInstanceAt(getInstance(), primitiveIndex, materialInstance);
}

fl::MaterialInstance* RenderableMesh::getMaterialInstanceAt(size_t primitiveIndex) const noexcept
{
	return getManager().getMaterialInstanceAt(getInstance(), primitiveIndex);
}

void RenderableMesh::setGeometryAt(size_t primitiveIndex, RenderableMesh::PrimitiveType type, fl::VertexBuffer* vertices,
							   fl::IndexBuffer* indices, size_t offset, size_t count) noexcept
{
	getManager().setGeometryAt(getInstance(), primitiveIndex, type, vertices, indices, offset, count);
}

void RenderableMesh::setGeometryAt(size_t primitiveIndex, RenderableMesh::PrimitiveType type, size_t offset,
							   size_t count) noexcept
{
	getManager().setGeometryAt(getInstance(), primitiveIndex, type, offset, count);
}

uint8_t RenderableMesh::getLayerMask() const noexcept
{
	return getManager().getLayerMask(getInstance());
}

size_t RenderableMesh::getPrimitiveCount() const noexcept
{
	return getManager().getPrimitiveCount(getInstance());
}

void RenderableMesh::setBlendOrderAt(size_t primitiveIndex, uint16_t order) noexcept
{
	getManager().setBlendOrderAt(getInstance(), primitiveIndex, order);
}

filament::AttributeBitset RenderableMesh::getEnabledAttributesAt(size_t primitiveIndex) const noexcept
{
	return getManager().getEnabledAttributesAt(getInstance(), primitiveIndex);
}

RenderableMesh::Instance RenderableMesh::getInstance() const noexcept
{
	return getManager().getInstance(mEntity);
}

const RenderableMesh::Manager& RenderableMesh::getManager() const noexcept
{
	return mEngine.getRenderableManager();
}

RenderableMesh::Manager& RenderableMesh::getManager() noexcept
{
	return mEngine.getRenderableManager();
}

RenderableMesh::RenderableMesh(RenderableMesh&& other) : mEngine{other.mEngine}, mEntity(std::exchange(other.mEntity, {}))
{
}

RenderableMesh& RenderableMesh::operator=(RenderableMesh&& other)
{
	assert(&other.mEngine == &mEngine);
	mEntity = std::exchange(other.mEntity, {});

	return *this;
}

} // namespace spatial::render