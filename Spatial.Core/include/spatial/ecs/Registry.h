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

class Registry
{
  public:
	using VersionType = entt::registry::version_type;

	Registry();

	Entity createEntity();

	bool isValid(Entity entity) const;

	size_t getEntitiesCount() const;

	template <typename Component>
	size_t getEntitiesCount() const
	{
		return mRegistry.size<Component>();
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
	auto getEntities(entt::exclude_t<Exclude...> excludes = {}) const
	{
		return mRegistry.view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	auto getEntities(ExcludeComponentsType<Exclude...> excludes = {})
	{
		return mRegistry.view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	Entity getFirstEntity(ExcludeComponentsType<Exclude...> excludes = {})
	{
		auto view = mRegistry.view<Component...>(std::move(excludes));
		for (auto entity : view) {
			return entity;
		}

		return NullEntity;
	}

	template <typename Component>
	const Component& getComponent(Entity entity) const
	{
		return mRegistry.get<Component>(entity);
	}

	template <typename Component>
	Component& getComponent(Entity entity)
	{
		return mRegistry.get<Component>(entity);
	}

	template <typename Component>
	void removeComponent(Entity entity)
	{
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
		mRegistry.emplace<Component>(entity);
	}

	template <typename Component>
	Component& addComponent(Entity entity, Component&& component)
	{
		return mRegistry.emplace<Component>(entity, std::move(component));
	}

	template <typename Component, typename... Args>
	Component& addComponent(Entity entity, Args&&... args)
	{
		return mRegistry.emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	Component& addOrReplaceComponent(Entity entity, Component&& component)
	{
		return mRegistry.emplace_or_replace<Component>(entity, std::move(component));
	}

	template <typename Component, typename... Args>
	Component& addOrReplaceComponent(Entity entity, Args&&... args)
	{
		return mRegistry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename Component>
	Component& getOrAddComponent(Entity entity, Component&& component)
	{
		return mRegistry.get_or_emplace<Component>(entity, std::move(component));
	}

	template <typename Component, typename... Args>
	Component& getOrAddComponent(Entity entity, Args&&... args)
	{
		return mRegistry.get_or_emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <typename... Component>
	bool hasAllComponents(Entity entity) const
	{
		return mRegistry.all_of<Component...>(entity);
	}

	template <typename... Component>
	bool hasAnyComponent(Entity entity) const
	{
		return mRegistry.any_of<Component...>(entity);
	}

	VersionType getVersion(Entity entity) const;

	void destroy(Entity entity);

	operator const entt::registry&() const
	{
		return mRegistry;
	}

	operator entt::registry&()
	{
		return mRegistry;
	}

  private:
	entt::registry mRegistry;
};

} // namespace spatial::ecs