#include <filament/Engine.h>
#include <spatial/render/Renderable.h>
#include <utils/Entity.h>

namespace spatial
{

Renderable::Renderable(filament::Engine& engine, utils::Entity entity, std::size_t count)
	: mManager{engine.getRenderableManager()}, mEntity{entity}
{
	filament::RenderableManager::Builder(count).build(engine, entity);
}

Renderable::~Renderable()
{
	if (isValid())
		mManager.destroy(mEntity);
}

bool Renderable::isValid() const noexcept
{
	return getInstance().isValid();
}

void Renderable::setAxisAlignedBoundingBox(const filament::Box& aabb) noexcept
{
	mManager.setAxisAlignedBoundingBox(getInstance(), aabb);
}

void Renderable::setLayerMask(uint8_t select, uint8_t values) noexcept
{
	mManager.setLayerMask(getInstance(), select, values);
}

void Renderable::setPriority(uint8_t priority) noexcept
{
	mManager.setPriority(getInstance(), priority);
}

void Renderable::setCulling(bool enable) noexcept
{
	mManager.setCulling(getInstance(), enable);
}

void Renderable::setCastShadows(bool enable) noexcept
{
	mManager.setCastShadows(getInstance(), enable);
}

void Renderable::setReceiveShadows(bool enable) noexcept
{
	mManager.setReceiveShadows(getInstance(), enable);
}

void Renderable::setScreenSpaceContactShadows(bool enable) noexcept
{
	mManager.setScreenSpaceContactShadows(getInstance(), enable);
}

bool Renderable::isShadowCaster() const noexcept
{
	return mManager.isShadowCaster(getInstance());
}

bool Renderable::isShadowReceiver() const noexcept
{
	return mManager.isShadowCaster(getInstance());
}

void Renderable::setBones(const Bone* transforms, size_t boneCount, size_t offset) noexcept
{
	mManager.setBones(getInstance(), transforms, boneCount, offset);
}

void Renderable::setBones(const math::mat4f* transforms, size_t boneCount, size_t offset) noexcept
{
	mManager.setBones(getInstance(), transforms, boneCount, offset);
}

void Renderable::setMorphWeights(const math::float4& weights) noexcept
{
	mManager.setMorphWeights(getInstance(), weights);
}

const filament::Box& Renderable::getAxisAlignedBoundingBox() const noexcept
{
	return mManager.getAxisAlignedBoundingBox(getInstance());
}

uint8_t Renderable::getLayerMask() const noexcept
{
	return mManager.getLayerMask(getInstance());
}

size_t Renderable::getPrimitiveCount() const noexcept
{
	return mManager.getPrimitiveCount(getInstance());
}

void Renderable::setMaterialInstanceAt(size_t primitiveIndex,
									   const filament::MaterialInstance* materialInstance) noexcept
{
	mManager.setMaterialInstanceAt(getInstance(), primitiveIndex, materialInstance);
}

filament::MaterialInstance* Renderable::getMaterialInstanceAt(size_t primitiveIndex) const noexcept
{
	return mManager.getMaterialInstanceAt(getInstance(), primitiveIndex);
}

void Renderable::setGeometryAt(size_t primitiveIndex, PrimitiveType type, filament::VertexBuffer* vertices,
							   filament::IndexBuffer* indices, size_t offset, size_t count) noexcept
{
	mManager.setGeometryAt(getInstance(), primitiveIndex, type, vertices, indices, offset, count);
}

void Renderable::setGeometryAt(size_t primitiveIndex, PrimitiveType type, size_t offset,
							   size_t count) noexcept
{
	mManager.setGeometryAt(getInstance(), primitiveIndex, type, offset, count);
}

void Renderable::setBlendOrderAt(size_t primitiveIndex, uint16_t order) noexcept
{
	mManager.setBlendOrderAt(getInstance(), primitiveIndex, order);
}

filament::AttributeBitset Renderable::getEnabledAttributesAt(size_t primitiveIndex) const noexcept
{
	return mManager.getEnabledAttributesAt(getInstance(), primitiveIndex);
}

Renderable::Instance Renderable::getInstance() const noexcept
{
	return mManager.getInstance(mEntity);
}

} // namespace spatial