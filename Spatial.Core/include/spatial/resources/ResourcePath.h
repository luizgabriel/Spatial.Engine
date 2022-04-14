#pragma once

#include <entt/core/ident.hpp>
#include <filesystem>
#include <spatial/common/StringHelpers.h>

namespace spatial
{

enum ResourceType
{
	ImageTexture,
	CubeMapTexture,
	FilaMesh,
	Javascript,
	FilaMat,
	Mat,
};

template <ResourceType t>
struct ResourcePath
{
	static constexpr ResourceType type{t};

	std::filesystem::path relativePath;

	constexpr ResourcePath() = default;

	ResourcePath(std::filesystem::path path) : relativePath{std::move(path)} // NOLINT(google-explicit-constructor)
	{
	}

	[[nodiscard]] std::string filename() const
	{
		return relativePath.filename().string();
	}

	[[nodiscard]] uint32_t getId() const
	{
		return HashedString{relativePath.c_str()}.value();
	}

	[[nodiscard]] bool isEmpty() const
	{
		return relativePath.empty();
	}
};

} // namespace spatial