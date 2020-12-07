#pragma once

#include <entt/entity/registry.hpp>
#include <memory>

namespace spatial
{

class Actor
{
  private:
	entt::registry* mRegistry;
	entt::entity mEntity;

  public:
	Actor() : mRegistry{nullptr}, mEntity{}
	{
	}

	Actor(entt::registry* registry, entt::entity entity) : mRegistry{registry}, mEntity{entity}
	{
	}

	void destroy()
	{
		assert(isValid());
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

	bool operator==(const Actor& rhs) const
	{
		return mRegistry == rhs.mRegistry && mEntity == rhs.mEntity;
	}

	bool operator!=(const Actor& rhs) const
	{
		return !(rhs == *this);
	}

	operator bool() const noexcept
	{
		return isValid();
	}

	uint32_t getId() const noexcept
	{
		return static_cast<uint32_t>(mEntity);
	}

	operator uint32_t() const noexcept
	{
		return getId();
	}
};

} // namespace spatial