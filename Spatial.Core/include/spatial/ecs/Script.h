#pragma once

#include <any>
#include <set>
#include <spatial/ecs/Registry.h>
#include <unordered_map>
#include <variant>

namespace spatial::ecs
{

namespace tags
{

struct IsScript
{
	static constexpr auto typeName = "is_script_tag";
};

} // namespace tags

struct ScriptComponent
{
	struct StringProperty
	{
		static constexpr auto typeName = "String";

		// std::string regexValidation;
		std::string defaultValue{};
	};

	struct NumberProperty
	{
		static constexpr auto typeName = "NumberRange";

		double defaultValue{.0};
		double min{std::numeric_limits<double>::min()};
		double max{std::numeric_limits<double>::max()};
	};

	using Property = std::variant<StringProperty, NumberProperty>;

	std::unordered_map<std::string, Property> properties;

	static const char* getPropertyType(const Property& property);
};

struct ScriptSystem
{
	std::set<std::string> requiredComponents;
	std::set<std::string> excludedComponents;
};

struct ScriptModule
{
	std::unordered_map<std::string, ScriptComponent> components;
	std::unordered_map<std::string, ScriptSystem> systems;
};

} // namespace spatial::ecs