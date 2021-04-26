#include <spatial/render/Transform.h>

using namespace spatial::math;

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

void Transform::setPosition(const float3& position) noexcept
{
	mPosition = position;
	refresh();
}

void Transform::setScale(const float3& scale) noexcept
{
	mScale = scale;
	refresh();
}

void Transform::setRotation(const float3& rotation) noexcept
{
	mRotation = rotation;
	refresh();
}

mat4f Transform::getMatrix() const noexcept
{
	const auto translation = mat4f::translation(mPosition);
	const auto rotation = mat4f::eulerZYX(mRotation.z, mRotation.y, mRotation.x);
	const auto scale = mat4f::scaling(mScale);

	return translation * rotation * scale;
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

TransformComponent toComponent(const Transform& transform)
{
	return {
		transform.getRotation(),
		transform.getScale(),
		transform.getRotation()
	};
}

Transform fromComponent(const TransformComponent& component, filament::Engine& engine, utils::Entity entity)
{
	auto transform = Transform{engine, entity};
	transform.setPosition(component.position);
	transform.setScale(component.scale);
	transform.setRotation(component.rotation);

	return transform;
}

} // namespace spatial