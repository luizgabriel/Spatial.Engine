#pragma once

#include <filament/Engine.h>
#include <filament/TransformManager.h>
#include <utils/Entity.h>
#include <spatial/common/Math.h>

namespace spatial
{

class Transform
{
  public:
	using Manager = filament::TransformManager;
	using Instance = Manager::Instance;

	Transform(filament::Engine& engine, utils::Entity entity);

	Transform(const Transform& other) = delete;
	Transform& operator=(const Transform& other) = delete;

	Transform(Transform&& other) noexcept;
	Transform& operator=(Transform&& other) noexcept;

	~Transform();

	bool isValid() const noexcept;

	const math::mat4f& get() const noexcept;

	const math::float3& getPosition() const noexcept;

	void setPosition(const math::float3& mPosition) noexcept;

	const math::float3& getScale() const noexcept;

	void setScale(const math::float3& mScale) noexcept;

	const math::float3& getRotation() const noexcept;

	void setRotation(const math::float3& rotation) noexcept;

	void rotate(const math::float3& rotation) noexcept;

	void translate(const math::float3 translation) noexcept;

  private:
	Manager& mManager;
	utils::Entity mEntity;

	math::float3 mPosition{.0f};
	math::float3 mScale{1.0f};
	math::float3 mRotation{.0f};

	void refreshTransformMatrix() noexcept;
	void set(const math::mat4f& localTransform) noexcept;

	Instance getInstance() const noexcept;
};

} // namespace spatial