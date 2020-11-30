#pragma once

#include <filament/RenderableManager.h>
#include <spatial/common/Math.h>
#include <utils/Entity.h>

namespace spatial
{

class Renderable
{
  public:
	using Manager = filament::RenderableManager;
	using Instance = Manager::Instance;
	using PrimitiveType = Manager::PrimitiveType;
	using Bone = Manager::Bone;

	Renderable(filament::Engine& engine, utils::Entity entity, std::size_t count);

	~Renderable();

	void setAxisAlignedBoundingBox(const filament::Box& aabb) noexcept;

	void setLayerMask(uint8_t select, uint8_t values) noexcept;

	void setPriority(uint8_t priority) noexcept;

	void setCulling(bool enable) noexcept;

	void setCastShadows(bool enable) noexcept;

	void setReceiveShadows(bool enable) noexcept;

	void setScreenSpaceContactShadows(bool enable) noexcept;

	bool isShadowCaster() const noexcept;

	bool isShadowReceiver() const noexcept;

	void setBones(Bone const* transforms, std::size_t boneCount = 1, std::size_t offset = 0) noexcept;

	void setBones(math::mat4f const* transforms, std::size_t boneCount = 1, std::size_t offset = 0) noexcept;

	void setMorphWeights(math::float4 const& weights) noexcept;

	const filament::Box& getAxisAlignedBoundingBox() const noexcept;

	std::uint8_t getLayerMask() const noexcept;

	std::size_t getPrimitiveCount() const noexcept;

	void setMaterialInstanceAt(std::size_t primitiveIndex, filament::MaterialInstance const* materialInstance) noexcept;

	filament::MaterialInstance* getMaterialInstanceAt(std::size_t primitiveIndex) const noexcept;

	void setGeometryAt(std::size_t primitiveIndex, PrimitiveType type, filament::VertexBuffer* vertices,
					   filament::IndexBuffer* indices, std::size_t offset, std::size_t count) noexcept;

	void setGeometryAt(std::size_t primitiveIndex, PrimitiveType type, std::size_t offset, std::size_t count) noexcept;

	void setBlendOrderAt(std::size_t primitiveIndex, uint16_t order) noexcept;

	filament::AttributeBitset getEnabledAttributesAt(std::size_t primitiveIndex) const noexcept;

	bool isValid() const noexcept;

  private:
	Manager& mManager;
	utils::Entity mEntity;

	Instance getInstance() const noexcept;
};

} // namespace spatial