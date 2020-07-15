#include <spatial/render/Entity.h>

namespace spatial
{

Entity::Entity(filament::Engine& engine) : m_engine{engine}, m_entity{}
{
	m_entity.clear();
}

Entity::~Entity()
{
	reset();
}

Entity::Entity(filament::Engine& engine, utils::Entity entity) : m_engine{engine}, m_entity{entity}
{
}

Entity::Entity(Entity&& other) noexcept : m_engine{other.m_engine}, m_entity{other.release()}
{
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	reset();
	m_entity = other.release();

	return *this;
}

utils::Entity Entity::release()
{
	auto null = utils::Entity{};
	null.clear();

	return std::exchange(m_entity, null);
}

void Entity::reset()
{
	if (m_entity.isNull())
		return;

	utils::EntityManager::get().destroy(m_entity);
	m_engine.destroy(m_entity);
	m_entity.clear();
}

} // namespace spatial