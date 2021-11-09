#pragma once

#include <GLFW/glfw3native.h>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace spatial::ecs
{

template <typename... Component>
constexpr auto ExcludeComponents = entt::exclude<Component...>;

template <typename... Component>
using ExcludeComponentsType = entt::exclude_t<Component...>;

constexpr auto NullEntity = entt::null;

using Entity = entt::entity;

class Registry
{
  public:
	using VersionType = entt::registry::version_type;

	Registry();

	Entity createEntity();

	bool isValid(Entity entity) const noexcept;

	size_t getEntitiesCount() const noexcept;

	template <typename Component>
	size_t getEntitiesCount() const noexcept
	{
		return mRegistry.size<Component>();
	}

	template <typename... Components>
	bool hasAnyEntity() const noexcept
	{
		return isValid(getFirstEntity<Components...>());
	}

	template <typename Component>
	auto getOnConstructSignal()
	{
		return mRegistry.on_construct<Component>();
	}

	template <typename Component>
	auto getOnUpdateSignal()
	{
		return mRegistry.on_update<Component>();
	}

	template <typename Component>
	auto getOnDestroySignal()
	{
		return mRegistry.on_destroy<Component>();
	}

	template <typename... Component, typename... Exclude>
	auto getEntities(ExcludeComponentsType<Exclude...> excludes = {}) const
	{
		return mRegistry.view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	auto getEntities(ExcludeComponentsType<Exclude...> excludes = {})
	{
		return mRegistry.view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	Entity getFirstEntity(ExcludeComponentsType<Exclude...> excludes = {}) const
	{
		auto view = mRegistry.view<const Component...>(std::move(excludes));
		for (auto entity : view) {
			return entity;
		}

		return NullEntity;
	}

	template <typename Component>
	const Component& getComponent(Entity entity) const
	{
		assert(isValid(entity));
		return mRegistry.get<Component>(entity);
	}

	template <typename Component>
	Component& getComponent(Entity entity)
	{
		assert(isValid(entity));
		return mRegistry.get<Component>(entity);
	}

	template <typename Component>
	void removeComponent(Entity entity)
	{
		assert(isValid(entity));
		mRegistry.remove<Component>(entity);
	}

	template <typename Component, typename It>
	void removeComponent(It begin, It end)
	{
		mRegistry.remove<Component>(begin, end);
	}

	template <typename Component>
	void addComponent(Entity entity)
	{
		assert(isValid(entity));
		mRegistry.emplace<Component>(entity);
	}

	template <typename Component>
	Component& addComponent(Entity entity, Component&& component)
	{
		assert(isValid(entity));
		return mRegistry.emplace<Component>(entity, std::forward<Component>(component));
	}

	template <typename Component, typename... Args>
	Component& addComponent(Entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return mRegistry.emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename... Component>
	bool hasAllComponents(Entity entity) const
	{
		return isValid(entity) && mRegistry.all_of<Component...>(entity);
	}

	template <typename... Component>
	bool hasAnyComponent(Entity entity) const
	{
		return mRegistry.any_of<Component...>(entity);
	}

	static VersionType getVersion(Entity entity) noexcept;

	void destroy(Entity entity);

	operator const entt::registry&() const
	{
		return mRegistry;
	}

	operator entt::registry&()
	{
		return mRegistry;
	}

	template <typename... Component, typename... Exclude>
	void clone(Registry& destination, ExcludeComponentsType<Exclude...> excludes = {}) const
	{
		auto view = getEntities<Component...>(std::move(excludes));
		destination.mRegistry.insert(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
	}

	template <typename Component>
	const Component* getComponentIfExists(Entity entity) const
	{
		return mRegistry.template try_get<const Component>(entity);
	}

	template <typename Component>
	Component* getComponentIfExists(Entity entity)
	{
		return mRegistry.template try_get<Component>(entity);
	}

  private:
	entt::registry mRegistry;
};

} // namespace spatial::ecs