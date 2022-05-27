#include <spatial/graphics/Entity.h>

namespace spatial::graphics
{

Entity::Entity(filament::Engine& engine) : mEngine{engine}, mEntity{}
{
}

Entity::~Entity()
{
	reset();
}

Entity::Entity(filament::Engine& engine, utils::Entity entity) : mEngine{engine}, mEntity{entity}
{
}

Entity::Entity(Entity&& other) noexcept : mEngine{other.mEngine}, mEntity{other.release()}
{
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	reset();
	mEntity = other.release();

	return *this;
}

utils::Entity Entity::release() noexcept
{
	return std::exchange(mEntity, utils::Entity{});
}

void Entity::reset()
{
	if (!isValid())
		return;

	utils::EntityManager::get().destroy(mEntity);
	mEntity.clear();
}

bool Entity::isValid() const noexcept
{
	return !mEntity.isNull();
}

} // namespace spatial::graphics