#pragma once

#include <entt/entity/registry.hpp>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/Resources.h>
#include <string_view>

namespace spatial
{

template <typename... Component>
constexpr auto ExcludeComponents = entt::exclude<Component...>;

constexpr auto NullInstance = entt::null;

using Instance = entt::entity;

class Stage
{
  public:
	using Registry = entt::registry;

	explicit Stage(filament::Engine& engine);

	Instance createInstance();

	Instance createInstance(const std::string_view name);

	void render(filament::Renderer& renderer) const;

	void onCreateRenderable(Stage::Registry& registry, Instance instance);

	void onDestroyRenderable(Stage::Registry& registry, Instance instance);

	void onStart();

	auto& getView() noexcept
	{
		return mView;
	}

	auto& getScene() noexcept
	{
		return mScene.ref();
	}

	bool isValid(Instance instance) const
	{
		return mRegistry.valid(instance);
	}

	void setMainCamera(Instance entity);

	Instance getMainCamera() noexcept
	{
		return mCameraInstance;
	}

	size_t getInstancesCount() const noexcept
	{
		return mRegistry.size();
	}

	const auto& getEngine() const noexcept
	{
		return mEngine;
	}

	auto& getEngine() noexcept
	{
		return mEngine;
	}

	const auto& getRegistry() const noexcept
	{
		return mRegistry;
	}

	auto& getRegistry() noexcept
	{
		return mRegistry;
	}

	template <typename... Component, typename... Exclude>
	auto getInstances(entt::exclude_t<Exclude...> excludes = {}) const
	{
		return std::as_const(mRegistry).view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	auto getInstances(entt::exclude_t<Exclude...> excludes = {})
	{
		return mRegistry.view<Component...>(std::move(excludes));
	}

	template <typename... Component, typename... Exclude>
	Instance getFirstInstance(entt::exclude_t<Exclude...> excludes = {})
	{
		auto view = mRegistry.view<Component...>(std::move(excludes));
		return *view.begin();
	}

	template<typename Component>
	const Component& getComponent(Instance instance) const
	{
		return mRegistry.get<Component>(instance);
	}

	template<typename Component>
	Component& getComponent(Instance instance)
	{
		return mRegistry.get<Component>(instance);
	}

	template<typename Component>
	Component& removeComponent(Instance instance)
	{
		return mRegistry.remove<Component>(instance);
	}

  private:
	filament::Engine& mEngine;
	Scene mScene;
	View mView;

	Registry mRegistry;
	Instance mCameraInstance;
};

} // namespace spatial
