#pragma once

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

class EntityHandle
{
  public:
	constexpr EntityHandle(Registry& registry, Entity entity) : mRegistry{registry}, mEntity{entity}
	{
	}

	constexpr EntityHandle(const EntityHandle& other) : EntityHandle(other.mRegistry, other.mEntity)
	{
	}

	EntityHandle& operator=(const EntityHandle& other)
	{
		assert(&mRegistry == &other.mRegistry);
		mEntity = other.mEntity;

		return *this;
	}

	void destroy()
	{
		mRegistry.destroyEntity(mEntity);
	}

	[[nodiscard]] bool isValid() const
	{
		return mRegistry.isValid(mEntity);
	}

	template <typename Component, typename... Args>
	decltype(auto) add(Args&&... args)
	{
		return mRegistry.addComponent<Component>(mEntity, std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) add(Component&& component)
	{
		return mRegistry.addComponent<Component>(mEntity, std::forward<Component>(component));
	}

	template <typename Component>
	Component& get()
	{
		assert(isValid());
		return mRegistry.getComponent<Component>(mEntity);
	}

	template <typename Component>
	const Component& get() const
	{
		assert(isValid());
		return mRegistry.getComponent<const Component>(mEntity);
	}

	template <typename... Component>
	[[nodiscard]] bool has() const
	{
		assert(isValid());
		return mRegistry.hasAllComponents<Component...>(mEntity);
	}

	template <typename Component>
	void remove()
	{
		assert(isValid());
		mRegistry.removeComponent<Component>(mEntity);
	}

	auto& getRegistry()
	{
		return mRegistry;
	}

	[[nodiscard]] const auto& getRegistry() const
	{
		return mRegistry;
	}

	bool operator==(Entity rhs) const
	{
		return mEntity == rhs;
	}

	bool operator!=(Entity rhs) const
	{
		return mEntity != rhs;
	}

	operator bool() const
	{
		return isValid();
	}

	operator Entity() const
	{
		return mEntity;
	}

	operator Entity&()
	{
		return mEntity;
	}

  private:
	Registry& mRegistry;
	Entity mEntity;
};

class EntityConstHandle
{
  public:
	constexpr EntityConstHandle(const Registry& registry, Entity entity) : mRegistry{registry}, mEntity{entity}
	{
	}

	constexpr explicit EntityConstHandle(const EntityHandle& other) : EntityConstHandle(other.getRegistry(), other)
	{
	}

	constexpr EntityConstHandle(const EntityConstHandle& other) : EntityConstHandle(other.mRegistry, other.mEntity)
	{
	}

	EntityConstHandle& operator=(const EntityConstHandle& other)
	{
		assert(&mRegistry == &other.mRegistry);
		mEntity = other.mEntity;

		return *this;
	}

	EntityConstHandle& operator=(const EntityHandle& other)
	{
		assert(&mRegistry == &other.getRegistry());
		mEntity = other;

		return *this;
	}

	[[nodiscard]] bool isValid() const
	{
		return mRegistry.isValid(mEntity);
	}

	template <typename Component>
	const Component& get() const
	{
		assert(isValid());
		return mRegistry.getComponent<const Component>(mEntity);
	}

	template <typename... Component>
	[[nodiscard]] bool has() const
	{
		assert(isValid());
		return mRegistry.hasAllComponents<Component...>(mEntity);
	}

	bool operator==(Entity rhs) const
	{
		return mEntity == rhs;
	}

	bool operator!=(Entity rhs) const
	{
		return mEntity != rhs;
	}

	operator bool() const
	{
		return isValid();
	}

	operator Entity() const
	{
		return mEntity;
	}

	[[nodiscard]] const auto& getRegistry() const
	{
		return mRegistry;
	}

  private:
	const Registry& mRegistry;
	Entity mEntity;
};

template <typename... Components>
constexpr auto handleOf(const Registry& registry)
{
	return EntityConstHandle{registry, registry.template getFirstEntity<Components...>()};
}

template <typename... Components>
constexpr auto handleOf(Registry& registry)
{
	return EntityHandle{registry, registry.template getFirstEntity<Components...>()};
}

constexpr auto handleOf(Registry& registry, Entity instance)
{
	return EntityHandle{registry, instance};
}

constexpr auto handleOf(const Registry& registry, Entity instance)
{
	return EntityConstHandle{registry, instance};
}

} // namespace spatial::ecs