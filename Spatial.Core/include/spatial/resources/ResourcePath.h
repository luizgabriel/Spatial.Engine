#pragma once

#include <entt/core/hashed_string.hpp>
#include <entt/core/ident.hpp>
#include <filesystem>

namespace spatial
{

struct ResourcePath
{
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
		const auto hashed = entt::hashed_string{relativePath.string().c_str()};
		return hashed.value();
	}

	[[nodiscard]] bool isEmpty() const
	{
		return relativePath.empty();
	}
};

} // namespace spatial