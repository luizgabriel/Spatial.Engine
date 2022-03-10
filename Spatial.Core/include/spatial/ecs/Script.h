#pragma once

#include <any>
#include <spatial/ecs/Registry.h>
#include <spatial/resources/Resource.h>
#include <unordered_map>

namespace spatial::ecs
{

namespace tags
{

struct IsScriptLoaded
{
};

struct IsScriptInvalid
{
};

} // namespace tags

struct Script
{
	constexpr static auto typeName = "script";

	Resource<ResourceType::Javascript> resource;
};

struct ScriptInstance
{
	constexpr static auto typeName = "script_instance";

	Entity script;
	std::unordered_map<std::string, std::any> customData;
};

} // namespace spatial::ecs