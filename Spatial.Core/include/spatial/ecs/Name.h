#pragma once

#include <string>

namespace spatial::ecs
{

struct Name
{
	constexpr static auto typeName = "name";

	std::string name;

	[[nodiscard]] const char* c_str() const
	{
		return name.c_str();
	}
};

} // namespace spatial::ecs