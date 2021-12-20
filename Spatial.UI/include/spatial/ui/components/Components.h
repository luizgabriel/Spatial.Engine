#pragma once

#include <filament/Texture.h>
#include <filesystem>
#include <fmt/format.h>
#include <imgui.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Transform.h>

namespace spatial::ui
{

bool inputText(std::string_view label, std::string& value);

bool inputPath(const std::string_view label, std::filesystem::path& path);

void spacing(std::uint32_t times = 1);

template <typename Component>
void componentInput(ecs::Registry& registry, ecs::Entity entity);

void image(const filament::Texture& texture, math::float2 size = math::float2{0, 0},
		   math::float4 uv = math::float4{0, 0, 1, 1});

bool imageButton(const filament::Texture& texture, math::float2 size = math::float2{0, 0},
				 math::float4 uv = math::float4{0, 0, 1, 1});

} // namespace spatial::ui