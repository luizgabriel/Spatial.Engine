#include <spatial/render/Entity.h>

namespace spatial::render
{

Entity::Entity(filament::Engine& engine) : mEngine{engine}, mEntity{}
{
	mEntity.clear();
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
	auto null = utils::Entity{};
	null.clear();

	return std::exchange(mEntity, null);
}

void Entity::reset()
{
	if (!isValid())
		return;

	utils::EntityManager::get().destroy(mEntity);
	mEngine.destroy(mEntity);
	mEntity.clear();
}

bool Entity::isValid() const noexcept
{
	return !mEntity.isNull();
}

} // namespace spatial::render