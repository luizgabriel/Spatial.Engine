#pragma once

#include <any>
#include <set>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/resources/Resource.h>
#include <unordered_map>
#include <variant>

namespace spatial::ecs
{

struct Script
{
	static constexpr auto typeName = "script";

	Resource<ResourceType::Javascript> resource;

	Script() = default;

	static EntityConstHandle find(const ecs::Registry& registry, const std::filesystem::path& resourcePath);
	static EntityHandle findOrCreate(ecs::Registry& registry, const std::filesystem::path& resourcePath);
};

struct ScriptInfo
{
	struct Property {

		struct StringType
		{
			static constexpr auto typeName = "String";

			// std::string regexValidation;
			std::string defaultValue{};
		};

		struct FloatRangeType
		{
			static constexpr auto typeName = "FloatRange";

			float defaultValue{.0f};
			float min{std::numeric_limits<float>::min()};
			float max{std::numeric_limits<float>::max()};
		};

		using Type = std::variant<StringType, FloatRangeType>;

		std::string key;
		std::string name;
		Type type;

		bool operator<(const Property& rhs) const;

		[[nodiscard]] const char* getTypeName() const;
	};

	std::string name;
	std::set<Property> properties;
};

struct ScriptError
{
	std::string message;
};

struct ScriptInstance
{
	static constexpr auto typeName = "script_instance";

	using DataValueType = std::variant<float, std::string>;
	using DataContainer = std::unordered_map<std::string, DataValueType>;

	Entity source;
	DataContainer customData;
};

} // namespace spatial::ecs