#pragma once

#include <memory>
#include <spatial/ecs/Registry.h>
#include <type_traits>

namespace spatial::ecs
{

class EntityHandle
{
  public:
	constexpr EntityHandle(Registry& registry, Entity entity) : mRegistry{registry}, mEntity{entity}
	{
	}

	EntityHandle(const EntityHandle& other) : EntityHandle(other.mRegistry, other.mEntity)
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
		assert(isValid());
		mRegistry.destroy(mEntity);
	}

	bool isValid() const
	{
		return mRegistry.isValid(mEntity);
	}

	template <typename Component>
	Component& add(Component&& component)
	{
		assert(isValid());
		return mRegistry.addComponent<Component>(mEntity, std::move(component));
	}


	template <typename Component, typename... Args>
	Component& add(Args&&... args)
	{
		assert(isValid());
		return mRegistry.addComponent<Component>(mEntity, std::forward<Args>(args)...);
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

	template <typename Component, typename... Args>
	Component& addOrReplace(Args&&... args)
	{
		assert(isValid());
		return mRegistry.addOrReplaceComponent<Component>(mEntity, std::forward<Args>(args)...);
	}

	template <typename Component, typename... Args>
	Component& getOrAdd(Args&&... args)
	{
		assert(isValid());
		return mRegistry.getOrAddComponent<Component>(mEntity, std::forward<Args>(args)...);
	}

	template <typename... Component>
	bool has() const
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

constexpr auto handleOf(Registry& stage, Entity instance)
{
	return EntityHandle{stage, instance};
}

} // namespace spatial