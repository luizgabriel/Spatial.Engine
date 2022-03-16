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

class SnapshotLoader;

class Registry : private entt::registry
{
  public:
	using VersionType = entt::registry::version_type;

	Registry();

	Entity createEntity();

	bool isValid(Entity entity) const noexcept;

	size_t getEntitiesCount() const noexcept;

	const ecs::Entity* getEntities() const;

	ecs::Entity getLastDestroyedEntity() const;

	template <typename Component>
	size_t getEntitiesCount() const noexcept
	{
		return size<Component>();
	}

	template <typename... Components>
	bool hasAnyEntity() const noexcept
	{
		return isValid(getFirstEntity<Components...>());
	}

	template <typename Component>
	auto getOnConstructSignal()
	{
		return on_construct<Component>();
	}

	template <typename Component>
	auto getOnUpdateSignal()
	{
		return on_update<Component>();
	}

	template <typename Component>
	auto getOnDestroySignal()
	{
		return on_destroy<Component>();
	}

	template <typename... Component, typename... Exclude>
	auto getEntities(ExcludeComponentsType<Exclude...> excludes = {}) const
	{
		return view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	auto getEntities(ExcludeComponentsType<Exclude...> excludes = {})
	{
		return view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	Entity getFirstEntity(ExcludeComponentsType<Exclude...> excludes = {}) const
	{
		auto view = getEntities<std::add_const_t<Component>...>(std::move(excludes));
		for (auto entity : view)
			return entity;

		return NullEntity;
	}

	template <typename Component>
	const Component& getComponent(Entity entity) const
	{
		assert(isValid(entity));
		return get<Component>(entity);
	}

	template <typename Component>
	Component& getComponent(Entity entity)
	{
		assert(isValid(entity));
		return get<Component>(entity);
	}

	template <typename Component>
	void removeComponentFromEntities()
	{
		auto view = getEntities<Component>();
		remove<Component>(view.begin(), view.end());
	}

	template <typename Component>
	void removeComponent(Entity entity)
	{
		assert(isValid(entity));
		remove<Component>(entity);
	}

	template <typename Component>
	void removeComponentIfExists(Entity entity)
	{
		assert(isValid(entity));
		remove_if_exists<Component>(entity);
	}

	template <typename Component, typename It>
	void removeComponent(It begin, It end)
	{
		remove<Component>(begin, end);
	}

	template <typename Component, typename It>
	void insertComponent(It begin, It end, const Component& component = {})
	{
		return insert<Component>(begin, end, component);
	}

	template <typename Component>
	decltype(auto) addComponent(Entity entity, Component&& component)
	{
		assert(isValid(entity));
		return emplace<Component>(entity, std::forward<Component>(component));
	}

	template <typename Component, typename... Args>
	decltype(auto) addComponent(Entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component, typename... Args>
	decltype(auto) addOrReplaceComponent(Entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) addOrReplaceComponent(Entity entity, Component&& component)
	{
		assert(isValid(entity));
		return emplace_or_replace<Component>(entity, std::forward<Component>(component));
	}

	template <typename Component, typename... Args>
	decltype(auto) getOrAddComponent(Entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return get_or_emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) getOrAddComponent(Entity entity, Component&& component)
	{
		assert(isValid(entity));
		return get_or_emplace<Component>(entity, std::forward<Component>(component));
	}

	template <typename... Component>
	bool hasAllComponents(Entity entity) const
	{
		return isValid(entity) && all_of<Component...>(entity);
	}

	template <typename... Component>
	bool hasAnyComponent(Entity entity) const
	{
		return isValid(entity) && any_of<Component...>(entity);
	}

	static VersionType getVersion(Entity entity) noexcept;

	void destroyEntity(Entity entity);

	template <typename It>
	void destroyEntities(It first, It last)
	{
		destroy<It>(first, last);
	}

	template <typename... Component, typename... Exclude>
	void clone(Registry& destination, ExcludeComponentsType<Exclude...> excludes = {}) const
	{
		auto view = getEntities<Component...>(std::move(excludes));
		destination.insert(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
	}

	template <typename Component>
	const Component* tryGetComponent(Entity entity) const
	{
		return isValid(entity) ? try_get<const Component>(entity) : nullptr;
	}

	template <typename Component>
	Component* tryGetComponent(Entity entity)
	{
		return isValid(entity) ? try_get<Component>(entity) : nullptr;
	}

	void destroyOrphans();

	friend class SnapshotLoader;
};

} // namespace spatial::ecs