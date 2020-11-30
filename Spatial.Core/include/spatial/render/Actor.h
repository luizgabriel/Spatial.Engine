#pragma once

#include <entt/entity/registry.hpp>
#include <memory>

namespace spatial
{

namespace detail
{

template <typename Registry>
class EntityAdapter
{
  private:
	Registry* mRegistry;
	entt::entity mEntity;

  public:
	EntityAdapter() : mRegistry{nullptr}, mEntity{}
	{
	}

	EntityAdapter(Registry* registry) : mRegistry{registry}, mEntity{mRegistry->create()}
	{
	}

	EntityAdapter(Registry* registry, entt::entity entity) : mRegistry{registry}, mEntity{entity}
	{
	}

	EntityAdapter(const EntityAdapter& other) : EntityAdapter(other.mRegistry, other.mEntity)
	{
	}

	EntityAdapter(EntityAdapter&& other) : EntityAdapter(other.mRegistry, other.mEntity)
	{
	}

	EntityAdapter& operator=(const EntityAdapter& other) noexcept
	{
		mRegistry = other.mRegistry;
		mEntity = other.mEntity;
		return *this;
	}

	EntityAdapter& operator=(EntityAdapter&& other) noexcept
	{
		mRegistry = other.mRegistry;
		mEntity = other.mEntity;
		return *this;
	}

	void destroy()
	{
		mRegistry->destroy(mEntity);
	}

	bool isValid() const noexcept
	{
		return mRegistry != nullptr && mRegistry->valid(mEntity);
	}

	template <typename Component, typename... Args>
	Component& getOrAddComponent(Args&&... args)
	{
		assert(isValid());
		return mRegistry->template get_or_emplace<Component>(mEntity, std::forward<Args>(args)...);
	}

	template <typename Component, typename... Args>
	Component& addComponent(Args&&... args)
	{
		assert(isValid());
		return mRegistry->template emplace<Component>(mEntity, std::forward<Args>(args)...);
	}

	template <typename Component>
	Component& getComponent()
	{
		assert(isValid());
		return mRegistry->template get<Component>(mEntity);
	}

	template <typename Component>
	const Component& getComponent() const
	{
		assert(isValid());
		return mRegistry->template get<const Component>(mEntity);
	}

	template <typename Component>
	bool hasComponent() const
	{
		assert(isValid());
		return mRegistry->template has<Component>(mEntity);
	}

	template <typename Component>
	void removeComponent()
	{
		assert(isValid());
		mRegistry->template remove<Component>(mEntity);
	}

	entt::entity getEntity() const noexcept
	{
		return mEntity;
	}

	bool operator==(const EntityAdapter& rhs) const
	{
		return mRegistry == rhs.mRegistry && mEntity == rhs.mEntity;
	}

	bool operator!=(const EntityAdapter& rhs) const
	{
		return !(rhs == *this);
	}
};

} // namespace detail

using ConstActor = detail::EntityAdapter<const entt::registry>;
using Actor = detail::EntityAdapter<entt::registry>;

} // namespace spatial