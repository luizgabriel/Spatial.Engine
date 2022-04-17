#pragma once

#include <entt/core/hashed_string.hpp>
#include <entt/core/ident.hpp>
#include <filesystem>

namespace spatial
{

enum ResourceType
{
	ImageTexture,
	CubeMapTexture,
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
		return entt::hashed_string{relativePath.c_str()}.value();
	}

	[[nodiscard]] bool isEmpty() const
	{
		return relativePath.empty();
	}
};

} // namespace spatial