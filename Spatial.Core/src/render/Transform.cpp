#include <spatial/render/Transform.h>

using namespace spatial::math;

namespace spatial::render
{

Transform::Transform(filament::Engine& engine, utils::Entity entity)
	: mManager{engine.getTransformManager()}, mEntity{entity}
{
}

Transform::~Transform()
{
	if (isValid())
		mManager.destroy(mEntity);
}

Transform::Transform(Transform&& other) noexcept
	: mManager{other.mManager},
	  mEntity{std::exchange(other.mEntity, {})}
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

void Transform::setMatrix(const math::mat4f& matrix) noexcept
{
	mManager.setTransform(getInstance(), matrix);
}

mat4f Transform::getMatrix() const noexcept
{
	return mManager.getTransform(getInstance());
}

Transform::Instance Transform::getInstance() const noexcept
{
	return mManager.getInstance(mEntity);
}

} // namespace spatial