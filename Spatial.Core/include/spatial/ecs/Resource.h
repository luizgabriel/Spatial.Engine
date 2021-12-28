#pragma once

#include <entt/core/ident.hpp>
#include <filesystem>
#include <spatial/common/StringHelpers.h>

namespace spatial::ecs
{

enum ResourceType
{
	ImageTexture,
	CubeMapTexture,
	Text,
	FilaMesh,
};

template <ResourceType t>
struct Resource
{
	static constexpr ResourceType sType{t};
	std::filesystem::path relativePath{};

	[[nodiscard]] uint32_t getId() const
	{
		return HashedString{relativePath.c_str()}.value();
	}

	[[nodiscard]] uint32_t getTypeId() const
	{
		return sType;
	}
};

} // namespace spatial::ecs