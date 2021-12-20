#pragma once

#include <filesystem>
#include <spatial/common/Math.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct SkyBoxColor
{
	math::float4 color{.0f};
	float intensity{30000};
};

struct SkyBoxTexture
{
	std::filesystem::path texturePath{};
	bool showSun{true};
	float intensity{30000};
};

} // namespace spatial::ecs