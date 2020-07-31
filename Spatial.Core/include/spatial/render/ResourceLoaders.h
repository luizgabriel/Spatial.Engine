#pragma once

#include <spatial/render/Resources.h>

#include <cstdint>
#include <filesystem>
#include <unordered_map>

#include <filament/Box.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <spatial/render/Mesh.h>
#include <string_view>

namespace spatial
{

using materials_map_t = std::unordered_map<std::string, filament::MaterialInstance*>;

Material createMaterial(filament::Engine& engine, const std::vector<char>& resourceData);

Texture createTexture(filament::Engine& engine, const std::vector<char>& resourceData);

Mesh createMesh(filament::Engine& engine, const std::vector<char>& resourceData);

} // namespace spatial