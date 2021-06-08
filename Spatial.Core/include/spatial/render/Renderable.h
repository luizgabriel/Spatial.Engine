#pragma once

#include <filament/IndexBuffer.h>
#include <filament/MaterialInstance.h>
#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <spatial/common/Math.h>
#include <utils/Entity.h>

namespace spatial::render
{

class Renderable
{
  public:
	using Manager = filament::RenderableManager;
	using Instance = Manager::Instance;
	using PrimitiveType = Manager::PrimitiveType;
	using Bone = Manager::Bone;
	using Builder = Manager::Builder;

	Renderable(filament::Engine& engine, utils::Entity entity, std::size_t primitivesCount);

	~Renderable();

	Renderable(const Renderable& other) = delete;
	Renderable& operator=(const Renderable& other) = delete;

	Renderable(Renderable&& other);
	Renderable& operator=(Renderable&& other);

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

	void setMaterialInstanceAt(size_t primitiveIndex, filament::MaterialInstance const* materialInstance) noexcept;

	filament::MaterialInstance* getMaterialInstanceAt(size_t primitiveIndex) const noexcept;

	void setGeometryAt(size_t primitiveIndex, PrimitiveType type, filament::VertexBuffer* vertices,
					   filament::IndexBuffer* indices, size_t offset, size_t count) noexcept;

	void setGeometryAt(size_t primitiveIndex, PrimitiveType type, size_t offset, size_t count) noexcept;

	const filament::Box& getAxisAlignedBoundingBox() const noexcept;

	std::uint8_t getLayerMask() const noexcept;

	std::size_t getPrimitiveCount() const noexcept;

	void setBlendOrderAt(std::size_t primitiveIndex, uint16_t order) noexcept;

	filament::AttributeBitset getEnabledAttributesAt(std::size_t primitiveIndex) const noexcept;

	bool isValid() const noexcept;

  private:
	filament::Engine& mEngine;
	utils::Entity mEntity;

	Instance getInstance() const noexcept;

	const Manager& getManager() const noexcept;

	Manager& getManager() noexcept;
};

} // namespace spatial::render