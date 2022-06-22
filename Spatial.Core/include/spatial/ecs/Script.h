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

struct ScriptModule {
	struct Property {
		struct StringType {
			static constexpr auto typeName = "String";

			// std::string regexValidation;
			std::string defaultValue{};
		};

		struct NumberRangeType {
			static constexpr auto typeName = "NumberRange";

			double defaultValue{.0};
			double min{std::numeric_limits<double>::min()};
			double max{std::numeric_limits<double>::max()};
		};

		using Type = std::variant<StringType, NumberRangeType>;

		std::string name;
		Type type;

		bool operator<(const Property& rhs) const;

		[[nodiscard]] const char* getTypeName() const;
	};

	std::string name;
	std::unordered_map<std::string, Property> properties;

	bool operator<(const ScriptModule& rhs) const
	{
		return name < rhs.name;
	}

	bool operator>(const ScriptModule& rhs) const
	{
		return rhs < *this;
	}

	bool operator<=(const ScriptModule& rhs) const
	{
		return !(rhs < *this);
	}

	bool operator>=(const ScriptModule& rhs) const
	{
		return !(*this < rhs);
	}
};

struct ScriptSystem
{
	std::string name;
	std::set<std::string> requiredComponents;
	std::set<std::string> excludedComponents;

	bool operator<(const ScriptSystem& rhs) const
	{
		return name < rhs.name;
	}

	bool operator>(const ScriptSystem& rhs) const
	{
		return rhs < *this;
	}

	bool operator<=(const ScriptSystem& rhs) const
	{
		return !(rhs < *this);
	}

	bool operator>=(const ScriptSystem& rhs) const
	{
		return !(*this < rhs);
	}
};

struct ScriptInfo
{
	std::set<ScriptModule> components;
	std::set<ScriptSystem> systems;

	bool operator<(const ScriptInfo& rhs) const
	{
		if (components < rhs.components)
			return true;
		if (rhs.components < components)
			return false;
		return systems < rhs.systems;
	}

	bool operator>(const ScriptInfo& rhs) const
	{
		return rhs < *this;
	}

	bool operator<=(const ScriptInfo& rhs) const
	{
		return !(rhs < *this);
	}

	bool operator>=(const ScriptInfo& rhs) const
	{
		return !(*this < rhs);
	}
};

} // namespace spatial::ecs