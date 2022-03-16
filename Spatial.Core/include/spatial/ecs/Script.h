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
	constexpr static auto typeName = "script";

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
			static constexpr const char* typeName = "StringType";

			// std::string regexValidation;
			std::string defaultValue{};
		};

		struct FloatRangeType
		{
			static constexpr const char* typeName = "FloatRange";

			float defaultValue{.0f};
			float min{std::numeric_limits<float>::min()};
			float max{std::numeric_limits<float>::max()};
		};

		std::string name;
		std::variant<StringType, FloatRangeType> type;

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
	constexpr static auto typeName = "script_instance";

	Entity script;
	std::unordered_map<std::string, std::any> customData;
};

} // namespace spatial::ecs