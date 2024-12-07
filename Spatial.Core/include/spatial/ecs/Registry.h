#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/runtime_view.hpp>

namespace spatial::ecs
{

template <typename... Component>
constexpr auto Exclude = entt::exclude<Component...>;

template <typename... Component>
using ExcludeComponentsType = entt::exclude_t<Component...>;

constexpr auto NullEntity = entt::null;

using Entity = entt::entity;

using RuntimeView = entt::runtime_view;

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

	template <typename... Components>
	[[nodiscard]] bool hasAnyEntity() const noexcept
	{
		return this->isValid(getFirstEntity<Components...>());
	}

	template <typename Component>
	[[nodiscard]] auto getOnConstructSignal()
	{
		return this->on_construct<Component>();
	}

	template <typename Component>
	[[nodiscard]] auto getOnUpdateSignal()
	{
		return this->on_update<Component>();
	}

	template <typename Component>
	[[nodiscard]] auto getOnDestroySignal()
	{
		return this->on_destroy<Component>();
	}

	template <typename... Component, typename... Exclude>
	[[nodiscard]] auto getEntities(
		ExcludeComponentsType<Exclude...> excludes = ExcludeComponentsType<Exclude...>{}) const
	{
		return this->view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	[[nodiscard]] auto getEntities(ExcludeComponentsType<Exclude...> excludes = ExcludeComponentsType<Exclude...>{})
	{
		return this->view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	[[nodiscard]] Entity getFirstEntity(
		ExcludeComponentsType<Exclude...> excludes = ExcludeComponentsType<Exclude...>{}) const
	{
		auto view = getEntities<std::add_const_t<Component>...>(std::move(excludes));
		for (auto entity : view)
			return entity;

		return NullEntity;
	}

	template <typename Component>
	[[nodiscard]] const Component& getComponent(Entity entity) const
	{
		assert(isValid(entity));
		return this->get<Component>(entity);
	}

	template <typename Component>
	[[nodiscard]] Component& getComponent(Entity entity)
	{
		assert(hasComponent<Component>(entity));
		return this->get<Component>(entity);
	}

	template <typename... Component>
	size_t removeComponentFromEntities()
	{
		auto view = getEntities<Component...>();
		return this->remove<Component...>(view.begin(), view.end());
	}

	template <typename Component>
	size_t removeComponent(Entity entity)
	{
		assert(isValid(entity));
		return this->remove<Component>(entity);
	}

	template <typename Component, typename It>
	size_t removeComponent(It begin, It end)
	{
		return this->remove<Component>(begin, end);
	}

	template <typename Component, typename It>
	void insertComponent(It begin, It end, const Component& component = {})
	{
		return this->insert<Component>(begin, end, component);
	}

	template <typename Component>
	decltype(auto) addComponent(Entity entity, Component&& component)
	{
		assert(!hasComponent<Component>(entity));
		return this->emplace<Component>(entity, std::forward<Component>(component));
	}

	template <typename Component, typename... Args>
	decltype(auto) addComponent(Entity entity, Args&&... args)
	{
		assert(!hasComponent<Component>(entity));
		return this->emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component, typename... Args>
	decltype(auto) addOrReplaceComponent(Entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return this->emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) addOrReplaceComponent(Entity entity, Component&& component)
	{
		assert(isValid(entity));
		return this->emplace_or_replace<Component>(entity, std::forward<Component>(component));
	}

	template <typename Component, typename... Args>
	decltype(auto) getOrAddComponent(Entity entity, Args&&... args)
	{
		assert(isValid(entity));
		return this->get_or_emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) getOrAddComponent(Entity entity, Component&& component)
	{
		assert(isValid(entity));
		return this->get_or_emplace<Component>(entity, std::forward<Component>(component));
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
		this->destroy<It>(first, last);
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
		return isValid(entity) ? this->try_get<const Component>(entity) : nullptr;
	}

	template <typename Component>
	Component* tryGetComponent(Entity entity)
	{
		return isValid(entity) ? this->try_get<Component>(entity) : nullptr;
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
		this->destroy(view.begin(), view.end());
	}

	template <typename Component, typename Compare, typename Sort = entt::std_sort, typename... Args>
	void sortComponent(Compare compare, Sort algo = Sort{}, Args&&... args)
	{
		this->sort<Component>(std::forward<Compare>(compare), std::forward<Sort>(algo), std::forward<Args>(args)...);
	}

	template <typename Component>
	decltype(auto) getStorage()
	{
		return this->storage<Component>();
	}

	template <typename Component>
	decltype(auto) getStorage() const
	{
		return this->storage<Component>();
	}

	friend class SnapshotLoader;
};

} // namespace spatial::ecs