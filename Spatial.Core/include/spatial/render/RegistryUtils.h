#pragma once

#include <entt/entity/registry.hpp>
#include <boost/tti/has_member_function.hpp>

namespace spatial::ecs
{

BOOST_TTI_HAS_MEMBER_FUNCTION(onConstruct);

BOOST_TTI_HAS_MEMBER_FUNCTION(onDestroy);

BOOST_TTI_HAS_MEMBER_FUNCTION(onReplace);

template <typename T>
constexpr bool has_on_construct_v = has_member_function_onConstruct<T, void, boost::mpl::vector<entt::registry&, entt::entity>>::value;

template <typename T>
constexpr bool has_on_destroy_v = has_member_function_onDestroy<T, void, boost::mpl::vector<entt::registry&, entt::entity>>::value;

template <typename T>
constexpr bool has_on_replace_v = has_member_function_onReplace<T, void, boost::mpl::vector<entt::registry&, entt::entity>>::value;

template <typename Component, typename Listener>
void connect(entt::registry& registry, Listener& listener)
{
	if constexpr(has_on_construct_v<Listener>)
		registry.on_construct<Component>().template connect<&Listener::onConstruct>(listener);

	if constexpr(has_on_destroy_v<Listener>)
		registry.on_destroy<Component>().template connect<&Listener::onDestroy>(listener);

	if constexpr(has_on_replace_v<Listener>)
		registry.on_replace<Component>().template connect<&Listener::onReplace>(listener);
}

template <typename Component, typename Listener>
void disconnect(entt::registry& registry, Listener& listener)
{
	if constexpr(has_on_construct_v<Listener>)
		registry.on_construct<Component>().template disconnect<&Listener::onConstruct>(listener);

	if constexpr(has_on_destroy_v<Listener>)
		registry.on_destroy<Component>().template disconnect<&Listener::onDestroy>(listener);

	if constexpr(has_on_replace_v<Listener>)
		registry.on_replace<Component>().template disconnect<&Listener::onReplace>(listener);
}

}