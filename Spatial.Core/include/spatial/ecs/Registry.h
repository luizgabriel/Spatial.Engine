#pragma once

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

	[[nodiscard]] bool isValid(Entity entity) const noexcept;

	[[nodiscard]] size_t getEntitiesCount() const noexcept;

	[[nodiscard]] const ecs::Entity* getEntities() const;

	template <typename Component>
	[[nodiscard]] size_t getEntitiesCount() const noexcept
	{
		return size<Component>();
	}

	template <typename... Components>
	[[nodiscard]] bool hasAnyEntity() const noexcept
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

	template <typename... Component>
	size_t removeComponentFromEntities()
	{
		auto view = getEntities<Component...>();
		return remove<Component...>(view.begin(), view.end());
	}

	template <typename Component>
	size_t removeComponent(Entity entity)
	{
		assert(isValid(entity));
		return remove<Component>(entity);
	}

	template <typename Component>
	void removeComponentIfExists(Entity entity)
	{
		assert(isValid(entity));
		remove_if_exists<Component>(entity);
	}

	template <typename Component, typename It>
	size_t removeComponent(It begin, It end)
	{
		return remove<Component>(begin, end);
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

	template <typename Component, typename... Args>
	decltype(auto) replaceComponent(entt::entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return this->replace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) replaceComponent(entt::entity entity, Component&& component)
	{
		assert(isValid(entity));
		return this->replace<Component>(entity, std::forward<Component>(component));
	}

	template <typename Component>
	[[nodiscard]] bool hasComponent(Entity entity) const
	{
		return isValid(entity) && all_of<Component>(entity);
	}

	template <typename... Component>
	[[nodiscard]] bool hasAllComponents(Entity entity) const
	{
		return isValid(entity) && all_of<Component...>(entity);
	}

	template <typename... Component>
	[[nodiscard]] bool hasAnyComponent(Entity entity) const
	{
		return isValid(entity) && any_of<Component...>(entity);
	}

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

	template <typename... Component>
	[[nodiscard]] bool existsAny() const
	{
		return isValid(getFirstEntity<Component...>());
	}

	template <typename... Component, typename... Exclude>
	void destroyEntitiesWith(ExcludeComponentsType<Exclude...> excludes = {})
	{
		auto view = getEntities<const Component...>(excludes);
		destroy(view.begin(), view.end());
	}

	template <typename Component, typename Compare, typename Sort = entt::std_sort, typename... Args>
	void sortComponent(Compare compare, Sort algo = Sort{}, Args&&... args)
	{
		sort<Component>(std::forward<Compare>(compare), std::forward<Sort>(algo), std::forward<Args>(args)...);
	}

	friend class SnapshotLoader;
};

} // namespace spatial::ecs