#pragma once

#include <boost/tti/has_member_function.hpp>
#include <entt/entity/registry.hpp>

namespace spatial
{

BOOST_TTI_HAS_MEMBER_FUNCTION(onConstruct);

BOOST_TTI_HAS_MEMBER_FUNCTION(onDestroy);

BOOST_TTI_HAS_MEMBER_FUNCTION(onUpdate);

template <typename T>
constexpr bool has_on_construct_v =
	has_member_function_onConstruct<T, void, boost::mpl::vector<entt::registry&, entt::entity>>::value;

template <typename T>
constexpr bool has_on_destroy_v =
	has_member_function_onDestroy<T, void, boost::mpl::vector<entt::registry&, entt::entity>>::value;

template <typename T>
constexpr bool has_on_update_v =
	has_member_function_onUpdate<T, void, boost::mpl::vector<entt::registry&, entt::entity>>::value;

template <typename Component, typename Listener>
void connect(entt::registry& registry, Listener& listener)
{
	if constexpr (has_on_construct_v<Listener>)
		registry.on_construct<Component>().template connect<&Listener::onConstruct>(listener);

	if constexpr (has_on_destroy_v<Listener>)
		registry.on_destroy<Component>().template connect<&Listener::onDestroy>(listener);

	if constexpr (has_on_update_v<Listener>)
		registry.on_update<Component>().template connect<&Listener::onUpdate>(listener);
}

template <typename Component, typename... Listeners>
void connect(entt::registry& registry, Listeners&... listener)
{
	(connect(registry, listener), ...);
}

template <typename Component, typename Listener>
void disconnect(entt::registry& registry, Listener& listener)
{
	if constexpr (has_on_construct_v<Listener>)
		registry.on_construct<Component>().template disconnect<&Listener::onConstruct>(listener);

	if constexpr (has_on_destroy_v<Listener>)
		registry.on_destroy<Component>().template disconnect<&Listener::onDestroy>(listener);

	if constexpr (has_on_update_v<Listener>)
		registry.on_update<Component>().template disconnect<&Listener::onUpdate>(listener);
}

template <typename Component, typename... Listeners>
void disconnect(entt::registry& registry, Listeners&... listener)
{
	(disconnect(registry, listener), ...);
}

template <typename Component>
struct RegistryConnector {
	std::reference_wrapper<entt::registry> registry;
};

template <typename Component, typename Listener>
RegistryConnector<Component> operator>>(RegistryConnector<Component> connector, Listener& listener)
{
	connect<Component>(connector.registry, listener);
	return connector;
}

} // namespace spatial::ecs