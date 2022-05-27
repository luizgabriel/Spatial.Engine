#include <spatial/graphics/MathConversionUtils.h>
#include <spatial/graphics/Transform.h>

namespace spatial::graphics
{

Transform::Transform(filament::Engine& engine, utils::Entity entity)
	: mManager{engine.getTransformManager()}, mEntity{entity}
{
}

Transform::Transform(Transform&& other) noexcept : mManager{other.mManager}, mEntity{std::exchange(other.mEntity, {})}
{
}

Transform& Transform::operator=(Transform&& other) noexcept
{
	mEntity = std::exchange(other.mEntity, {});
	return *this;
}

bool Transform::isValid() const noexcept
{
	return getInstance().isValid();
}

void Transform::setParent(const Transform& transform) noexcept
{
	mManager.setParent(getInstance(), transform.getInstance());
}

void Transform::setMatrix(const math::mat4& matrix) noexcept
{
	mManager.setTransform(getInstance(), toFilament(matrix));
}

math::dmat4 Transform::getMatrix() const noexcept
{
	return fromFilament(mManager.getTransform(getInstance()));
}

Transform::Instance Transform::getInstance() const noexcept
{
	return mManager.getInstance(mEntity);
}

} // namespace spatial::graphics