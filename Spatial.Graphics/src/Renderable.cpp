#include <filament/Engine.h>
#include <spatial/graphics/MathConversionUtils.h>
#include <spatial/graphics/Renderable.h>
#include <spatial/graphics/Resources.h>

namespace spatial::graphics
{

Renderable::Renderable(filament::Engine& engine, utils::Entity entity, std::size_t primitivesCount)
	: mEngine{engine}, mEntity{entity}, mMaterialInstances{}, mVertexBuffers{}, mIndexBuffers{}
{
	if (Builder(primitivesCount).build(engine, entity) != Builder::Result::Success)
		throw std::runtime_error("Could not create renderable");
}

Renderable::~Renderable()
{
	reset();
}

void Renderable::reset()
{
	if (isValid())
		getManager().destroy(mEntity);

	mEntity = utils::Entity{};
}

bool Renderable::isValid() const noexcept
{
	return getInstance().isValid();
}

void Renderable::setAxisAlignedBoundingBox(const math::AxisAlignedBoundingBox& aabb) noexcept
{
	getManager().setAxisAlignedBoundingBox(getInstance(), toFilament(aabb));
}

void Renderable::setLayerMask(uint8_t select, uint8_t values) noexcept
{
	getManager().setLayerMask(getInstance(), select, values);
}

void Renderable::setPriority(uint8_t priority) noexcept
{
	getManager().setPriority(getInstance(), priority);
}

void Renderable::setCulling(bool enable) noexcept
{
	getManager().setCulling(getInstance(), enable);
}

void Renderable::setCastShadows(bool enable) noexcept
{
	getManager().setCastShadows(getInstance(), enable);
}

void Renderable::setReceiveShadows(bool enable) noexcept
{
	getManager().setReceiveShadows(getInstance(), enable);
}

void Renderable::setScreenSpaceContactShadows(bool enable) noexcept
{
	getManager().setScreenSpaceContactShadows(getInstance(), enable);
}

bool Renderable::isShadowCaster() const noexcept
{
	return getManager().isShadowCaster(getInstance());
}

bool Renderable::isShadowReceiver() const noexcept
{
	return getManager().isShadowReceiver(getInstance());
}

math::AxisAlignedBoundingBox Renderable::getAxisAlignedBoundingBox() const noexcept
{
	return fromFilament(getManager().getAxisAlignedBoundingBox(getInstance()));
}

void Renderable::setMaterialInstanceAt(size_t primitiveIndex, const SharedMaterialInstance& materialInstance) noexcept
{
	mMaterialInstances.emplace(primitiveIndex, materialInstance);
	return getManager().setMaterialInstanceAt(getInstance(), primitiveIndex, materialInstance->get());
}

filament::MaterialInstance* Renderable::getMaterialInstanceAt(size_t primitiveIndex) const noexcept
{
	return getManager().getMaterialInstanceAt(getInstance(), primitiveIndex);
}

void Renderable::setGeometryAt(size_t primitiveIndex, Renderable::PrimitiveType type,
							   const SharedVertexBuffer& vertices, const SharedIndexBuffer& indices, size_t offset,
							   size_t count) noexcept
{
	mVertexBuffers.emplace(primitiveIndex, vertices);
	mIndexBuffers.emplace(primitiveIndex, indices);
	getManager().setGeometryAt(getInstance(), primitiveIndex, type, vertices.get(), indices.get(), offset, count);
}

void Renderable::setGeometryAt(size_t primitiveIndex, Renderable::PrimitiveType type, size_t offset,
							   size_t count) noexcept
{
	getManager().setGeometryAt(getInstance(), primitiveIndex, type, offset, count);
}

uint8_t Renderable::getLayerMask() const noexcept
{
	return getManager().getLayerMask(getInstance());
}

size_t Renderable::getPrimitiveCount() const noexcept
{
	return getManager().getPrimitiveCount(getInstance());
}

void Renderable::setBlendOrderAt(size_t primitiveIndex, uint16_t order) noexcept
{
	getManager().setBlendOrderAt(getInstance(), primitiveIndex, order);
}

filament::AttributeBitset Renderable::getEnabledAttributesAt(size_t primitiveIndex) const noexcept
{
	return getManager().getEnabledAttributesAt(getInstance(), primitiveIndex);
}

Renderable::Instance Renderable::getInstance() const noexcept
{
	return getManager().getInstance(mEntity);
}

const Renderable::Manager& Renderable::getManager() const noexcept
{
	return mEngine.getRenderableManager();
}

Renderable::Manager& Renderable::getManager() noexcept
{
	return mEngine.getRenderableManager();
}

utils::Entity Renderable::release() noexcept
{
	return std::exchange(mEntity, {});
}

Renderable::Renderable(Renderable&& other) noexcept : mEngine{other.mEngine}, mEntity(other.release())
{
}

Renderable& Renderable::operator=(Renderable&& other) noexcept
{
	assert(&other.mEngine == &mEngine);
	reset();
	mEntity = other.release();

	return *this;
}

void Renderable::resetMaterialInstance(size_t primitiveIndex) noexcept
{
	getManager().setMaterialInstanceAt(getInstance(), primitiveIndex,
									   mEngine.getDefaultMaterial()->getDefaultInstance());
}

} // namespace spatial::graphics