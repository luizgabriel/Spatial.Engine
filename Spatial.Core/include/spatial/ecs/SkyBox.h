#pragma once

#include <filesystem>
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>
#include <spatial/common/StringHelpers.h>

namespace spatial::ecs
{

struct SkyBoxColor
{
	math::float4 color{.0f};
};

struct SkyBoxTexture
{
	std::filesystem::path texturePath;
	bool showSun{true};

	explicit SkyBoxTexture(std::filesystem::path texturePath = {}) : texturePath{std::move(texturePath)}
	{
	}

	[[nodiscard]] uint32_t getResourceId() const
	{
		return HashedString{texturePath.c_str()}.value();
	}
};

} // namespace spatial::ecs