#pragma once

#include <spatial/render/Resources.h>
#include <spatial/render/Mesh.h>

#include <filesystem>

namespace spatial
{

Material createMaterial(filament::Engine* engine, const std::filesystem::path& filePath);

Mesh createMesh(filament::Engine* engine, const std::filesystem::path& filePath, filament::MaterialInstance* material);

Texture createTexture(filament::Engine* engine, const std::filesystem::path& filePath);

} // namespace spatial