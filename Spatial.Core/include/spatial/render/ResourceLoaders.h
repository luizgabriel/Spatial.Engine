#pragma once

#include <spatial/render/Resources.h>

#include <filesystem>
#include <unordered_map>
#include <cstdint>

#include <spatial/render/Mesh.h>
#include <filament/Box.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>

namespace spatial
{

using materials_map_t = std::unordered_map<std::string, filament::MaterialInstance*>;

Material createMaterial(filament::Engine& engine, const std::filesystem::path& filePath);

Texture createTexture(filament::Engine& engine, const std::filesystem::path& filePath);

Mesh createMesh(filament::Engine& engine, const std::filesystem::path& path);

} // namespace spatial