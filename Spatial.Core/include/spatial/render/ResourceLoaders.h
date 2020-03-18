#pragma once

#include <spatial/render/CommonResources.h>
#include <spatial/render/Mesh.h>
#include <spatial/render/ImageBasedLight.h>

#include <filesystem>

namespace spatial::render
{

Material createMaterial(filament::Engine* engine, const std::filesystem::path& filePath);

Mesh createMesh(filament::Engine* engine, filament::MaterialInstance* material, const std::filesystem::path& filePath);

Texture createTexture(filament::Engine* engine, const std::filesystem::path& filePath);

Texture createKtxTexture(filament::Engine* engine, const std::filesystem::path& filePath);

ImageBasedLight createIBLFromKtx(filament::Engine* engine, const std::filesystem::path& folder, const std::string& name);

} // namespace spatial::render