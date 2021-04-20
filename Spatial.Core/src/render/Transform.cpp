#include <spatial/render/Transform.h>

namespace spatial
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
	  mEntity{std::exchange(other.mEntity, {})},
	  mPosition{std::move(other.mPosition)},
	  mScale{std::move(other.mScale)},
	  mRotation{std::move(other.mRotation)}
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

const math::float3& Transform::getPosition() const noexcept
{
	return mPosition;
}

void Transform::setPosition(const math::float3& position) noexcept
{
	mPosition = position;
}

const math::float3& Transform::getScale() const noexcept
{
	return mScale;
}

void Transform::setScale(const math::float3& scale) noexcept
{
	mScale = scale;
}

const math::float3& Transform::getRotation() const noexcept
{
	return mRotation;
}

void Transform::setRotation(const math::float3& rotation) noexcept
{
	mRotation = rotation;
}

math::mat4f Transform::getMatrix() noexcept
{
	using namespace math;

	const auto translation = mat4f::translation(mPosition);
	const auto scale = mat4f::scaling(mScale);
	const auto rotateZ = mat4f::rotation(mRotation.z, math::axisZ);
	const auto rotateX = mat4f::rotation(mRotation.x, math::axisX);
	const auto rotateY = mat4f::rotation(mRotation.y, math::axisY);

	return translation * (rotateX * rotateY * rotateZ) * scale;
}

void Transform::rotate(const math::float3& rotation) noexcept
{
	setRotation(getRotation() + rotation);
}

void Transform::translate(const math::float3 translation) noexcept
{
	setPosition(getPosition() + translation);
}

Transform::Instance Transform::getInstance() const noexcept
{
	return mManager.getInstance(mEntity);
}

void Transform::refresh() noexcept
{
	const auto localTransform = getMatrix();
	mManager.setTransform(getInstance(), localTransform);
}

} // namespace spatial