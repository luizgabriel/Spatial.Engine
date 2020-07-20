#pragma once

#include <spatial/render/Resources.h>

#include <filesystem>
#include <unordered_map>
#include <cstdint>

#include <filament/Box.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <string_view>
#include <spatial/render/Mesh.h>

namespace spatial
{

using materials_map_t = std::unordered_map<std::string, filament::MaterialInstance*>;

Material createMaterial(filament::Engine& engine, const std::string_view resourceData);

Texture createTexture(filament::Engine& engine, const std::string_view resourceData);

Mesh createMesh(filament::Engine& engine, const std::string_view resourceData);

} // namespace spatial