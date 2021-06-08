#pragma once

#include <string>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct EntityName
{
	std::string name;

	operator const std::string&() const
	{
		return name;
	}
};

} // namespace spatial::ecs