#pragma once

#include <filament/IndexBuffer.h>
#include <filament/MaterialInstance.h>
#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <set>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/common/Math.h>
#include <spatial/graphics/MaterialInstance.h>
#include <spatial/graphics/Resources.h>
#include <utils/Entity.h>

namespace spatial::graphics
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

	Renderable(Renderable&& other) noexcept;
	Renderable& operator=(Renderable&& other) noexcept;

	void setAxisAlignedBoundingBox(const math::AxisAlignedBoundingBox& aabb) noexcept;

	void setLayerMask(uint8_t select, uint8_t values) noexcept;

	void setPriority(uint8_t priority) noexcept;

	void setCulling(bool enable) noexcept;

	void setCastShadows(bool enable) noexcept;

	void setReceiveShadows(bool enable) noexcept;

	void setScreenSpaceContactShadows(bool enable) noexcept;

	[[nodiscard]] bool isShadowCaster() const noexcept;

	[[nodiscard]] bool isShadowReceiver() const noexcept;

	void setMaterialInstanceAt(size_t primitiveIndex, const SharedMaterialInstance& materialInstance) noexcept;

	void resetMaterialInstance(size_t primitiveIndex) noexcept;

	[[nodiscard]] filament::MaterialInstance* getMaterialInstanceAt(size_t primitiveIndex) const noexcept;

	void setGeometryAt(size_t primitiveIndex, PrimitiveType type, const SharedVertexBuffer& vertices,
					   const SharedIndexBuffer& indices, size_t offset, size_t count) noexcept;

	[[nodiscard]] math::AxisAlignedBoundingBox getAxisAlignedBoundingBox() const noexcept;

	[[nodiscard]] std::uint8_t getLayerMask() const noexcept;

	[[nodiscard]] std::size_t getPrimitiveCount() const noexcept;

	void setBlendOrderAt(std::size_t primitiveIndex, uint16_t order) noexcept;

	[[nodiscard]] filament::AttributeBitset getEnabledAttributesAt(std::size_t primitiveIndex) const noexcept;

	[[nodiscard]] bool isValid() const noexcept;

	[[nodiscard]] utils::Entity release() noexcept;

	void reset();

  private:
	template <typename T>
	struct PrimitiveIndex
	{
		size_t primitiveIndex;
		T value;

		PrimitiveIndex(size_t primitiveIndex, T value) : primitiveIndex{primitiveIndex}, value{std::forward<T>(value)}
		{
		}

		bool operator<(const PrimitiveIndex& rhs) const
		{
			return primitiveIndex < rhs.primitiveIndex;
		}

		bool operator==(const PrimitiveIndex& rhs) const
		{
			return primitiveIndex == rhs.primitiveIndex;
		}
	};

	filament::Engine& mEngine;
	utils::Entity mEntity;
	std::set<PrimitiveIndex<SharedMaterialInstance>> mMaterialInstances;
	std::set<PrimitiveIndex<SharedVertexBuffer>> mVertexBuffers;
	std::set<PrimitiveIndex<SharedIndexBuffer>> mIndexBuffers;

	[[nodiscard]] Instance getInstance() const noexcept;

	[[nodiscard]] const Manager& getManager() const noexcept;

	Manager& getManager() noexcept;
};

} // namespace spatial::graphics