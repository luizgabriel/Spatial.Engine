#pragma once

#include <filament/Engine.h>
#include <filament/TransformManager.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Transform.h>
#include <utils/Entity.h>

namespace spatial::graphics
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

	[[nodiscard]] bool isValid() const noexcept;

	[[nodiscard]] math::dmat4 getMatrix() const noexcept;

	void setMatrix(const math::mat4& matrix) noexcept;

	void setParent(const Transform& transform) noexcept;

  private:
	Manager& mManager;
	utils::Entity mEntity;

	[[nodiscard]] Instance getInstance() const noexcept;
};

} // namespace spatial::graphics