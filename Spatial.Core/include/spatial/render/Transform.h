#pragma once

#include <filament/Engine.h>
#include <filament/TransformManager.h>
#include <spatial/common/Math.h>
#include <utils/Entity.h>

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

	void setPosition(const math::float3& position) noexcept;

	void setScale(const math::float3& scale) noexcept;

	/**
	 * @param rotation (pitch, yaw, roll)
	 */
	void setRotation(const math::float3& rotation) noexcept;

	auto getForward() const noexcept
	{
		return math::forwardVector(getMatrix());
	}

	const auto& getPosition() const noexcept
	{
		return mPosition;
	}

	const auto& getScale() const noexcept
	{
		return mScale;
	}

	const auto& getRotation() const noexcept
	{
		return mRotation;
	}

	math::mat4f getMatrix() const noexcept;

	void refresh() noexcept;

  private:
	Manager& mManager;
	utils::Entity mEntity;

	math::float3 mPosition{.0f};
	math::float3 mScale{1.0f};
	math::float3 mRotation{.0f};

	Instance getInstance() const noexcept;
};

} // namespace spatial